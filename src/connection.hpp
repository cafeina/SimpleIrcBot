#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <thread>

const int BUFFER_SIZE = 8192;

class Connection
{
public:

	Connection(const int);
	Connection(const Connection &c) { this->sock = c.sock; }
	~Connection();

	void read_data();
	//void write_data(const std::string&);

private:
	
	int sock;
	bool keep_alive;
	std::thread reader;

	bool received_ping(const std::string&);
	void send_pong(const std::string&);
};

#endif //CONNECTION_HPP