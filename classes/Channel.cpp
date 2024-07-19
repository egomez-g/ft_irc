#include "../inc/Channel.hpp"

Channel::Channel(std::string ChannelName)
{
	_channel_name = ChannelName;
	_topic = "";
	_password = "";
	_invite = false;
	_clearanceTopic = false;
	_clientSize = INT_MAX;
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

void	Channel::setClient(Client client)
{
	_clients.push_back(client);
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

void	Channel::addAdmin(Client client)
{
	_admins.push_back(client);
}

void	Channel::rmAdmin(Client client)
{
	std::vector<Client>::iterator i;

	for (i = _admins.begin(); i != _admins.end(); i++)
	{
		if (client == i)
		{
			_admins.erase(i);
			return ;
		}
	}
}

void	Channel::eraseClient(Client client)
{
	std::vector<Client>::iterator i;

	for (i = _clients.begin(); i != _clients.end(); i++)
	{
		if (client == i)
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

std::vector<Client>	Channel::getAdmins()
{
	return (_admins);
}

int	Channel::getClientSize()
{
	return (_clientSize);
}
void	Channel::setClientSize(int new_size)
{
	_clientSize = new_size;
}

bool	Channel::isAdmin(std::string client)
{
	std::vector<Client>::iterator i;

	for (i = _admins.begin(); i != _admins.end(); i++)
	{
		if (i->getUsername() == client)
			return (true);
	}
	return (false);
}

Client	* Channel::getChannelClientByName(std::string name)
{
	int clients_size = _clients.size();

	for(int i = 0; i < clients_size; i++)
	{
		if (_clients[i].getUsername() == name)
			return (&_clients[i]);
	}
	return (NULL);
}

bool	Channel::operator == (std::vector<Channel>::iterator other)
{
	if (this->getName() == other->getName())
		return (true);
	return(false);
}
