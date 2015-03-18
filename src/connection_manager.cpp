#include "connection_manager.hpp"

ConnectionManager::ConnectionManager() {};
	
void ConnectionManager::add_connection(const Connection &c) 
{ 
	connections.push_back(c); 
}