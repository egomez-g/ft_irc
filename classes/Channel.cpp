#include "../inc/Channel.hpp"

Channel::Channel(std::string ChannelName)
{
	_channel_name = ChannelName;
	_topic = "";
	_password = "";
	_invite = false;
	_clearanceTopic = false;
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

void	Channel::setPassword(std::string password)
{
	_password = password;
}

void	Channel::setInvite(bool invite)
{
	_invite = invite;
}

void	Channel::setClearanceTopic(bool clearanceTopic)
{
	_clearanceTopic = clearanceTopic;
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

std::string	Channel::getPassword()
{
	return (_password);
}

bool	Channel::getInvite()
{
	return (_invite);
}

bool	Channel::getClearanceTopic()
{
	return (_clearanceTopic);
}

bool	Channel::operator==(std::vector<Channel>::iterator other)
{
	if (this->getName() == other->getName())
		return (true);
	return(false);
}
