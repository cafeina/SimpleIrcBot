#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <vector>

#include "connection.hpp"

class ConnectionManager
{
public:

	ConnectionManager();
	void add_connection(const Connection&);

private:
	
	std::vector<Connection> connections;
};

#endif //CONNECTION_MANAGER_HPP