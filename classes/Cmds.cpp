#include "../inc/Server.hpp"

void Server::Kick(std::string clientName, std::string channelName)
{
	std::string	msg;
	if (!getChannelByName(channelName))
	{
		msg = "error: wrong channel name: " + channelName + "\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (!getClientByName(clientName))
	{
		msg = "error: wrong client name: " + clientName + "\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}

	getChannelByName(channelName)->eraseClient(*getClientByName(clientName));
	std::cout << clientName << "erased from: " << channelName << std::endl;
}

void Server::Invite(std::string clientName, std::string channelName)
{
	std::string	msg;

	if (!getClientByName(clientName))
	{
		msg = "error: wrong client name: " + clientName + "\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (!getChannelByName(channelName))
		addChannel(channelName);
	getChannelByName(channelName)->setClient(*getClientByName(clientName));
	msg = clientName + " added to: " + channelName + "\n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Topic()
{
	std::string	msg;

	if (getClientByFd(_client_socket)->getLoc().empty())
	{
		msg = "Error: user isn't in any channel\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return;
	}
	msg = getChannelByClientSocket(_client_socket)->getTopic() + "\n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Topic(std::vector<std::string> msgs)
{
	std::string	msg;
	Channel *channel =  getChannelByName(getClientByFd(_client_socket)->getLoc());

	if (getClientByFd(_client_socket)->getLoc().empty())
	{
		msg = "Error: user isn't in any channel\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return;
	}
	for (unsigned long i = 1; i < msgs.size(); i++)
		msg += msgs[i] + " ";
	channel->setTopic(msg);
	msg = "Channel name changed to: " + msg + "\n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Mode(std::string flag, std::vector<std::string> msgs)
{
	std::string	msg;
	Channel	*channel = getChannelByName(getClientByFd(_client_socket)->getLoc());
	if (!channel)
	{
		send(_client_socket, "Client isn't on a channel\n", 26, 0);
		return ;
	}
	if (flag == "i") //Set/remove Invite-only channel
	{
		if (!channel->getInvite())
		{
			channel->setInvite(true);
			msg = "[" + getClientByFd(_client_socket)->getLoc() + "]: Invite only enabled\n";
		}
		else
		{
			channel->setInvite(false);
			msg = "[" + getClientByFd(_client_socket)->getLoc() + "]: Invite only disabled\n";
		}
	}
	else if (flag == "t") // Set/remove the restrictions of the TOPIC command to channel operators
	{
		if (!channel->getInvite())
		{
			channel->setInvite(true);
			msg = "[" + getClientByFd(_client_socket)->getLoc() + "]: Topic clearance active\n";
		}
		else
		{
			channel->setInvite(false);
			msg = "[" + getClientByFd(_client_socket)->getLoc() + "]: Topic clearance unactive\n";
		}
	}
	else if (flag == "k") //Set/remove the channel key (password)
	{
		if (msgs.size() < 3 || msgs[2].empty())
		{
			channel->setPassword("");
			msg = "Password unseted\n";
		}
		else
		{
			channel->setPassword(msgs[2]);
			msg = "[" + channel->getName() + "]: Password setted to:" + channel->getPassword() + "\n";
		}
	}
	else if (flag == "o") //Give/take channel operator privilege
	{}
	else
		msg = "error: wrong flag\n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Help()
{
	std::string	msg;
	msg = "Available commands:\n";
	msg += "\"[KICK] [username] [channel name]\" \n";
	msg += "\"[INV] [username] [channel name]\" \n";
	msg += "\"[TOPIC]\" \n";
	msg += "\"[TOPIC] [new topic]\" \n";
	msg += "\"[MODE] [flag]\" \n";
	msg += "\"[PRIV] [username msg]\" \n";
	msg += "\"[MOVE] [channel name]\" \n";
	msg += "\"[HELP]\" \n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void	Server::Priv(std::string name, std::vector<std::string> msgs)
{
	std::string	msg = "<" + getClientByFd(_client_socket)->getUsername() + ">: ";
	int			socketSend;

	if (getClientByName(name) == NULL)
	{
		msg = "error: Client not found\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	for (unsigned long i = 2; i < msgs.size(); i++)
		msg += msgs[i] + " ";
	msg += "\n";
	socketSend = getClientByName(name)->getPollfd().fd;
	send(socketSend, msg.c_str(), msg.length(), 0);
}

void	Server::move(std::string name)
{
	Client	*cliente = getClientByFd(_client_socket);
	Channel	*canal = getChannelByName(name);
	std::vector<Client>	klientes;
	std::vector<Client>::iterator i;

	if (!canal)
		send(_client_socket, "error: Channel not found\n", 25, 0);
	else
	{
		klientes = canal->getClients();
		for (i = klientes.begin(); i != klientes.end(); i++)
		{
			if (i->getUsername() == cliente->getUsername())
			{
				cliente->setLoc(name);
				break ;
			}
		}
		if (i == klientes.end())
			send(_client_socket, "You don't belong to this group\n", 31, 0);
	}
}

void	Server::sendToAll(std::vector<std::string> msgs)
{
	Channel *canal = getChannelByName(getClientByFd(_client_socket)->getLoc());
	std::vector<Client>	clientes = canal->getClients();
	if (!canal)
		return ;
	std::string msg = "[" + canal->getName() + "]" + "<" + getClientByFd(_client_socket)->getUsername() + ">: ";
	for (unsigned long j = 0; j < msgs.size(); j++)
		msg += msgs[j] + " ";
	msg += "\n";
	for (std::vector<Client>::iterator i = clientes.begin(); i != clientes.end(); i++)
	{
		if (i->getPollfd().fd != _client_socket)
			send(i->getPollfd().fd, msg.c_str(), msg.length(), 0);
	}
}