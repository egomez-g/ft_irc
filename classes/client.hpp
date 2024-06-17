#pragma once

#include "server.hpp"

#include <string>

class Client
{
	private:
		int socket;
		std::string nickname;
		std::string username;
	public:
		Client(int socket);
		int getSocket() const;
};
