#include "channel.hpp"

Channel::Channel(std::string name) : name(name)
{
}

Channel::~Channel()
{
}

bool	Channel::addClient(Client klaient)
{
	std::vector<Client>::iterator it;

	for (it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (it->getSocket() == klaient.getSocket())
			return (false);
	}
	clients.push_back(klaient);
	return (true);
}

bool	Channel::removeClient(Client klaient)
{
	std::vector<Client>::iterator it;

	for (it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (it->getSocket() == klaient.getSocket())
		{
			this->clients.erase(it);
			return (true);
		}
	}
	return (false);
}

std::string Channel::getName() const
{
	return (this->name);
}

void	Channel::addFd(Client *klaient)
{
	std::vector<Client>::iterator it;

	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getNickname().compare(klaient->getNickname()) == 0)
			return ;
	}
	clients.push_back(*klaient);
}