#include <iostream>
#include <map>
#include <stdarg.h>
#include <string>
#include <vector>

#include <plugin_manager.hpp>
#include <connection_manager.hpp>

struct Command
{
	std::string command;
	std::vector<std::string> parameters;
};

struct ServerDetails
{
	std::string name;
    std::string host;
    std::string port;
    std::string nick;
    std::string user;
    int sock;
};

struct IRCMessage
{
    std::string prefix;
    std::string command;
    std::vector<std::string> parameters;
    std::string trail;

private:
    friend std::ostream& operator<<(std::ostream &out, const IRCMessage &im)
    {
    	out << im.prefix << std::endl;
    	out << im.command << std::endl;
    	for(auto p : im.parameters)
    		out << "\t" << p << std::endl;
    	out << im.trail << std::endl;
    }
};

class PlugBot
{
public:
    PlugBot();
    ~PlugBot();

    void add_server(std::string, const std::string, const std::string, const std::string, const std::string);
    int connect_to_server(std::string);
    void exec_command(const std::string&, const Command&);//const std::string&, ...);
    void process_request(std::string);

private:
    std::map<std::string, std::vector<std::string>> list_of_channels {};
    std::map<std::string, struct ServerDetails> server_info {};
    bool keep_reading;
    //std::thread reader;
    PluginManager plugin_manager {};
    ConnectionManager connection_manager {};

    const std::string parse_message(const std::string&);
    std::string fill_in_parameters(const std::vector<std::string>&, std::string&);
};

static const std::map<std::string, std::string> commands {
{ "PRIVMSG",    "PRIVMSG %s :%s"    }, //PRIVMSG destination :message
{ "JOIN",       "JOIN %s %s"        }, //JOIN #channel [password]
{ "NAMES",      "NAMES %s"          }, //NAMES [channel]
{ "MODES",      "MODES %s %s"       }, //MODES
{ "PART",      "PART %s"       		}  //PART
};


