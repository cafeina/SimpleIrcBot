#include <algorithm>
#include <arpa/inet.h>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <regex>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "plugbot.hpp"
#include "connection.hpp"

using namespace std;

PlugBot::PlugBot()
{
	keep_reading = true;
	plugin_manager.scan_plugins();
    //reader = thread(&PlugBot::read_data, this);
    cout << "started" << endl;
}

PlugBot::~PlugBot()
{
    keep_reading = false;
    //reader.join();
}

const string PlugBot::parse_message(const string &msg)
{
    vector<string> lines;
    boost::split(lines, msg, boost::is_any_of("\r\n"));

    for(auto line : lines) {

        if(line.empty() || line[0] != ':')
        	continue;
        
        size_t prefix_end = line.find(" ");
        string prefix {};
        if (prefix_end != string::npos)
        	prefix = line.substr(1, prefix_end-1);

        size_t command_end = line.find(" ", prefix_end+1);
        string command {};
        if (command_end != string::npos)
        	command = line.substr(prefix_end+1, command_end-prefix_end-1);

        size_t trail_begin = line.find(":", 1);
        string trail {};
        if (trail_begin != string::npos)
        	trail = line.substr(trail_begin+1);
        
        string parameters = line.substr(command_end+1, trail_begin-command_end-2);

        vector<string> vec_parameters;
        split(vec_parameters, parameters, boost::is_any_of(""));
        IRCMessage message = { prefix, command, vec_parameters, trail };

        if(command == "PRIVMSG")
            process_request(trail);
    }

    return "";
}

void PlugBot::add_server(string server_nickname, const string hostname, const string port, const string nick, const string user)
{
    std::transform(server_nickname.begin(), server_nickname.end(), server_nickname.begin(), ::tolower);

    server_info[server_nickname] = { server_nickname, hostname, port, nick, user, 0 };

    cout << "Server added: " << hostname << " - " << port << endl;
}

void PlugBot::exec_command(const string &server_nickname, const Command &cmd)
{
    string command_def = commands.at(cmd.command);
    int pos = 0;
    int count = 0;

    pos = command_def.find("%s");
    while(pos != string::npos) {
        ++count;
        pos = command_def.find("%s", pos+2);
    }

    if(cmd.parameters.size() != count) {
    	cout << "Wrong number of parameters for command " << cmd.command << endl;
    	return;
    }

    for(string argument : cmd.parameters) {
        size_t p = command_def.find("%s");
        command_def = (command_def.substr(0, p) + argument + command_def.substr(p+2));
    }
        
    if(cmd.command == "JOIN") {
    	if(list_of_channels.count(server_nickname) == 0)
    		list_of_channels[server_nickname] = {};
    	list_of_channels[server_nickname].push_back(cmd.parameters[0]);
    }

    if(cmd.command == "PART") {
    	list_of_channels[server_nickname].erase(
    			std::remove(list_of_channels[server_nickname].begin(), list_of_channels[server_nickname].end(), cmd.parameters[0]),
    			list_of_channels[server_nickname].end());
	}

    string new_s = command_def;
    //write_data(server_nickname, new_s);
}

int PlugBot::connect_to_server(string server_nickname)
{
    struct addrinfo hints, *addresses, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    std::transform(server_nickname.begin(), server_nickname.end(), server_nickname.begin(), ::tolower);
    string hostname = server_info.at(server_nickname).host;
    string port = server_info.at(server_nickname).port;
    int &sock = server_info.at(server_nickname).sock;

    getaddrinfo(hostname.c_str(), port.c_str(), &hints, &addresses);
    for(rp = addresses; rp != nullptr; rp = rp->ai_next) {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if(sock == -1)
        	continue;

        if(connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) {
            struct sockaddr_in *host_addr = (struct sockaddr_in*)rp->ai_addr;
            struct in_addr *host_ip = &host_addr->sin_addr;
            char ip_str[20];
            memset(ip_str, 0, 20);
            inet_ntop(rp->ai_family, host_ip, ip_str, 20);

            std::cout << "Connected to " << hostname
                              << ", ip: " << ip_str << std::endl;
            break;
        }
    }

    freeaddrinfo(addresses);

    sleep(3);
    string connection_string =
                        "NICK " + server_info.at(server_nickname).nick + "\r\n" + 
                        "USER " + server_info.at(server_nickname).user + "\r\n";

    Connection c(sock);
    connection_manager.add_connection(c);

    write(sock, connection_string.c_str(), connection_string.length());

    string j = "JOIN #67ajutasd\r\n";
    write(sock, j.c_str(), j.length());

    string p = "PRIVMSG #67ajutasd :YO! MR. WHITE!\r\n";
    write(sock, p.c_str(), p.length());

    sleep(25);

    string pp = "PART #67ajutasd";
    write(sock, pp.c_str(), pp.length());

    sleep(5);

}

string PlugBot::fill_in_parameters(const vector<string> &parameters, string &parameter_string)
{
    cout << "fmt_string: " << parameter_string << endl;
    
    for(auto parameter : parameters) {
        string formated_parameter = "";

        for(int j = 0; j < parameter_string.size()-1; ++j) {
        	if (parameter_string[j] == '%') {
        		if (parameter_string[j+1] == 's') { // string parameter

					formated_parameter += '\"';
					for(auto c : parameter) {
						formated_parameter += (c == '\"')?"\\"+c:""+c;
					}
					formated_parameter += '\"';
				}
				else if (parameter_string[j+1] == 'p') { // non-string parameter
					formated_parameter = parameter;
				}

        		parameter_string = parameter_string.substr(0, j) + formated_parameter + parameter_string.substr(j+2);
        	}
        }
    }
    
    return parameter_string;
}

void PlugBot::process_request(string received_command)
{
	boost::trim(received_command);

    int parameters_begin = received_command.find("(");

    if(received_command.substr(received_command.size()-1) == ")")
		received_command = received_command.substr(0, received_command.size()-1);

    string command {};
    if(parameters_begin != std::string::npos)
    	command = received_command.substr(0, parameters_begin);
    else
		command = received_command;

    if(not plugin_manager.has_plugin(command))
    	return;
    
    vector<string> parameters;
    string x = received_command.substr(parameters_begin+1);
    boost::split(parameters, x, boost::is_any_of(","));
    string parameter_str {};
    for(auto p : parameters) {
        boost::trim(p);
    	parameter_str += " " + p;
    }

    cout << "Received command: " << command << endl;

    PluginInfo p_info = plugin_manager.get_plugin(command);

    string final_string = p_info.path + p_info.name + parameter_str;

    system(final_string.c_str());
}




