#include "../inc/Channel.hpp"

Channel::Channel(std::string ChannelName)
{
	_ChannelName = ChannelName;
	_topic = "";
}

Channel::~Channel()
{
}

void	Channel::setChannelName(std::string ChannelName)
{
	_ChannelName = ChannelName;
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void	Channel::setClient(Client klaient)
{
	_clients.push_back(klaient);
}

void	Channel::eraseClient(Client klaient)
{
	std::vector<Client>::iterator i;

	for (i = _clients.begin(); i != _clients.end(); i++)
	{
		if (klaient == i)
		{
			_clients.erase(i);
			return ;
		}
	}
}

std::string	Channel::getTopic()
{
	return (_topic);
}

std::string	Channel::getChannelName()
{
	return(_ChannelName);
}

std::vector<Client>	Channel::getClients()
{
	return (_clients);
}

bool	Channel::operator==(std::vector<Channel>::iterator other)
{
	if (this->getChannelName() == other->getChannelName())
		return (true);
	return(false);
}