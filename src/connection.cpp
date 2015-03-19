#include <cstring>
#include <iostream>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <connection.hpp>

using namespace std;

Connection::Connection(const int socket)
{
    sock = socket;
	keep_alive = true;
	fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK); // set socket to non-blocking
	reader = thread(&Connection::read_data, this);
}

Connection::~Connection() 
{
    keep_alive = false;
    reader.join();
}

void Connection::read_data()
{
    char buffer[BUFFER_SIZE];
    while (keep_alive) {
    	memset(buffer, 0, BUFFER_SIZE);
		read(sock, buffer, BUFFER_SIZE);

        string msg = buffer;
        if (not msg.empty())
            cout << msg << endl; // DEBUG

        if(received_ping(msg))
            send_pong(msg);
    }
}

bool Connection::received_ping(const string& msg)
{
	if(msg.substr(0, 4) == "PING") 
		return true;

	return false;
}

void Connection::send_pong(const string& msg)
{
    string pong_response = "PONG" + msg.substr(4);

    write(sock, pong_response.c_str(), pong_response.length());
}