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
	if (!getClientByUsername(clientName))
	{
		msg = "error: wrong client name: " + clientName + "\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (!getChannelByName(channelName)->getChannelClientByName(getClientByFd(_client_socket)->getUsername()))
	{
		msg = "error: you are not in <" + channelName + ">\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}

	if (!getChannelByName(channelName)->isAdmin(clientName) ||
		(getChannelByName(channelName)->isAdmin(clientName) &&
		 getChannelByName(channelName)->getAdmins().size() > 1))
	{
		if (getClientByFd(_client_socket)->getLoc() == channelName)
			getClientByUsername(clientName)->setLoc("");
		getChannelByName(channelName)->eraseClient(*getClientByUsername(clientName));
		msg = clientName + " erased from: " + channelName + "\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
	}
	else
	{
		msg = "Can't KICK ["+ clientName +"], Channel " + channelName + " needs an Admin\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
	}

}

void Server::Invite(std::string clientName, std::string channelName)
{
	std::string	msg;
	bool created = false;


	if (!getClientByUsername(clientName))
	{
		msg = "error: wrong client name: " + clientName + "\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (!getChannelByName(channelName))
	{
		created = true;
		addChannel(channelName);
		getClientByFd(_client_socket)->setLoc(channelName);
		msg = "<" + getClientByFd(_client_socket)->getUsername() + "> moved to: [" + channelName + "]\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
	}
	if (getChannelByName(channelName)->getClients().size() < getChannelByName(channelName)->getClientSize())
	{
		if (clientName != getClientByFd(_client_socket)->getUsername())
		{
			if (!getChannelByName(channelName)->getChannelClientByName(clientName))
			{
				getChannelByName(channelName)->setClient(*getClientByUsername(clientName));
				getClientByUsername(clientName)->setLoc(channelName);
				msg = "<" + clientName + ">" + " added to: [" + channelName + "]\n";
			}
			else
				msg = "<" + clientName + ">" + " was already in [" + channelName + "]\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
		}
		else if (!created)
		{
			msg = clientName + " was already in [" + channelName + "]\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
		}
	}
	else
	{
		msg = "[" + channelName + "] is full\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
	}
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
	if (getChannelByClientSocket(_client_socket)->getTopic().empty())
		msg = "[" + getChannelByClientSocket(_client_socket)->getName() + "]: No topic setted\n";
	else
		msg = getChannelByClientSocket(_client_socket)->getTopic() + "\n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Topic(std::vector<std::string> msgs)
{
	std::string	msg;
	Channel *channel =  getChannelByName(getClientByFd(_client_socket)->getLoc());
	
	if (!channel)
	{
		msg = "Error: user isn't in any channel\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return;
	}
	if (!channel->isAdmin(getClientByFd(_client_socket)->getUsername()) && channel->getClearanceTopic())
	{
		msg = "[" + channel->getName() + "]: You have no permission\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return;
	}
	for (unsigned long i = 1; i < msgs.size(); i++)
		msg += msgs[i] + " ";
	channel->setTopic(msg);
	msg = "Channel topic changed to: " + msg + "\n";
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
		if (!channel->isAdmin(getClientByFd(_client_socket)->getUsername()))
			msg = "[" + channel->getName() + "]: You have no permission\n";
		else if (!channel->getInvite())
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
		if (!channel->isAdmin(getClientByFd(_client_socket)->getUsername()))
			msg = "[" + channel->getName() + "]: You have no permission\n";
		else if (!channel->getClearanceTopic())
		{
			channel->setClearanceTopic(true);
			msg = "[" + getClientByFd(_client_socket)->getLoc() + "]: Topic clearance active\n";
		}
		else
		{
			channel->setClearanceTopic(false);
			msg = "[" + getClientByFd(_client_socket)->getLoc() + "]: Topic clearance unactive\n";
		}
	}
	else if (flag == "k") //Set/remove the channel key (password)
	{
		if (!channel->isAdmin(getClientByFd(_client_socket)->getUsername()))
			msg = "[" + channel->getName() + "]: You have no permission\n";
		else if (msgs.size() < 3 || msgs[2].empty())
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
	{
		if (!channel->getChannelClientByName(msgs[2]))
			msg = "Client doesn't belong to [" + channel->getName() + "]\n";
		else if (!channel->isAdmin(getClientByFd(_client_socket)->getUsername()))
			msg = "[" + channel->getName() + "]: You have no permission\n";
		else
		{
			if (channel->isAdmin(msgs[2]))
			{
				channel->addAdmin(*getClientByUsername(msgs[2]));
				msg = "[" + channel->getName() + "]: Deleted admin <" + msgs[2] + ">\n";
			}
			else
			{
				channel->rmAdmin(*getClientByUsername(msgs[2]));
				msg = "[" + channel->getName() + "]: New admin <" + msgs[2] + ">\n";
			}
		}
	}
	else if (flag == "l") //Set/Remove the user limit
	{
		if (!channel->isAdmin(getClientByFd(_client_socket)->getUsername()))
			msg = "[" + channel->getName() + "]: You have no permission\n";
		else
		{
			if (std::atol(msgs[3].c_str()) < 1 ||
				std::atol(msgs[3].c_str()) > (long int)channel->getClients().size())
				msg = "Not a valid size [MODE] [l] [size]\n";
			else
			{
				msg = "[" + channel->getName() + "] size setted to: " + msgs[3] + "\n";
				channel->setClientSize(std::atoi(msgs[3].c_str()));
			}
		}
	}
	else
		msg = "error: wrong flag\n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Help()
{
	std::string	msg;
	msg = "Available commands:\n";
	msg += "[KICK]  [username] [channel_name]	Kick user from channel\n";
	msg += "[INV]   [username] [channel_name]	Invite user to channel\n";
	msg += "[TOPIC] 							Print channel topic\n";
	msg += "[TOPIC] [new topic]					Set channel topic\n";
	msg += "[MODE]  [i]							Set/remove Invite-only channel \n";
	msg += "[MODE]  [t] 						Set/remove TOPIC restrictions \n";
	msg += "[MODE]  [k]							Set/remove the channel key \n";
	msg += "[MODE]  [o] [username]				Give/take channel operator privilege\n";
	msg += "[MODE]  [l] [size]					Set/remove the user limit to channel\n";
	msg += "[PRIV]  [username_msg] 				Send private msg to user\n";
	msg += "[MOVE]  [channel_name] 				Move to a channel\n";
	msg += "[JOIN]  [channel_name] 				Join to a channel\n";
	msg += "[NICK]  [new nickname] 				Set nickname\n";
	msg += "[HELP]\" \n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void	Server::Priv(std::string name, std::vector<std::string> msgs)
{
	std::string	msg = "<" + getClientByFd(_client_socket)->getNickname() + ">: ";
	int			socketSend;

	if (getClientByUsername(name) == NULL)
	{
		msg = "error: Client not found\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	for (unsigned long i = 2; i < msgs.size(); i++)
		msg += msgs[i] + " ";
	msg += "\n";
	socketSend = getClientByUsername(name)->getPollfd().fd;
	send(socketSend, msg.c_str(), msg.length(), 0);
}

void	Server::Move(std::string name)
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

				std::string	msg = "<" + cliente->getNickname() +"> moved to: [" + name + "]\n";
				send(_client_socket, msg.c_str(), msg.length(), 0);
				break ;
			}
		}
		if (i == klientes.end())
			send(_client_socket, "You don't belong to this group\n", 31, 0);
	}
}

void	Server::Join(std::string name)
{
	Channel *channel= getChannelByName(name);
	if (!channel)
	{
		send(_client_socket, "Channel doesn't exist\n", 22, 0);
		return ;
	}
	if (channel->getInvite())
		send(_client_socket, "Channel on invite only\n", 23, 0);
	else
	{
		std::string msg;

		if (channel->getClients().size() < channel->getClientSize())
		{
			if (channel->getChannelClientByName(name) == NULL)
			{
				getClientByFd(_client_socket)->setLoc(name);
				channel->setClient(*getClientByFd(_client_socket));
				msg = "Succesfully joined to: [" + name + "]\n";
			}
			else
				msg = name + " was already in [" + channel->getName() + "]\n";
		}
		else
			msg = "No space in [" + channel->getName() + "]\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
	}
}

void	Server::Join(std::string name, std::string password)
{
	Channel *channel= getChannelByName(name);
	if (!channel)
	{
		send(_client_socket, "Channel doesn't exist\n", 22, 0);
		return ;
	}
	if (channel->getInvite())
		send(_client_socket, "Channel on invite only\n", 23, 0);
	else
	{
		std::string msg;

		if (password != channel->getPassword())
		{
			msg = "Wrong password, try again\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
			return ;
		}

		getClientByFd(_client_socket)->setLoc(name);
		msg = "Succesfully joined to: [" + name + "]\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
	}
}

void	Server::Nick(std::string nickname)
{
	std::string	msg;
	if (nickname == "")
		send(_client_socket, "Can't set NULL nickname\n", 35, 0);
	else if (getClientByNickname(nickname))
		send(_client_socket, "Nickname already in use, try again\n", 35, 0);
	else
	{
		getClientByFd(_client_socket)->setNickname(nickname);
		msg = "Nickname changed to <" + nickname + ">\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
	}
}

void	Server::sendToAll(std::vector<std::string> msgs)
{
	Channel *canal = getChannelByName(getClientByFd(_client_socket)->getLoc());
	std::vector<Client>	clientes = canal->getClients();
	if (!canal)
		return ;
	std::string msg = "[" + canal->getName() + "]" + "<" + getClientByFd(_client_socket)->getNickname() + ">: ";
	for (unsigned long j = 0; j < msgs.size(); j++)
		msg += msgs[j] + " ";
	msg += "\n";
	for (std::vector<Client>::iterator i = clientes.begin(); i != clientes.end(); i++)
	{
		if (i->getPollfd().fd != _client_socket && getClientByFd(i->getPollfd().fd)->getLoc() == canal->getName())
			send(i->getPollfd().fd, msg.c_str(), msg.length(), 0);
	}	
}