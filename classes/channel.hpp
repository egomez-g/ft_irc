#pragma once

#include "server.hpp"
#include "client.hpp"

class Client;

class Channel
{
	private:
		std::string name;
		std::vector<Client>	clients;

	public:
		Channel(std::string name);
		~Channel();
		bool	addClient(Client klaient);
		bool	removeClient(Client klaient);
		std::string getName() const;
		void	addFd(Client *klaient);
};
