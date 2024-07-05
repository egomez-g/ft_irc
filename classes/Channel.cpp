#include "../inc/Channel.hpp"

Channel::Channel(std::string ChannelName)
{
	_channel_name = ChannelName;
	_topic = "";
}

Channel::~Channel()
{
}

void	Channel::setName(std::string ChannelName)
{
	_channel_name = ChannelName;
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

std::string	Channel::getName()
{
	return(_channel_name);
}

std::vector<Client>	Channel::getClients()
{
	return (_clients);
}

bool	Channel::operator==(std::vector<Channel>::iterator other)
{
	if (this->getName() == other->getName())
		return (true);
	return(false);
}