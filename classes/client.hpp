#pragma once

#include "server.hpp"

class Client
{
	private:
		std::string _name;
		std::string _password;
		std::string _server;
	public:
		Client(std::string name, std::string password, std::string server);
		~Client();
};
