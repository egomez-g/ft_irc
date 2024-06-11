#include "client.hpp"

Client::Client(std::string name, std::string password, std::string server)
{
	_name = name;
	_password = password;
	_server = server;
}

Client::~Client()
{

}
