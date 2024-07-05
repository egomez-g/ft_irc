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
	Channel * channel =  getChannelByClientSocket(_client_socket);

	if (!channel)
	{
		msg = "Error: user isn't in any channel\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return;
	}
	msg = getChannelByClientSocket(_client_socket)->getTopic();
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Topic(std::string topic)
{
	std::string	msg;
	Channel *channel =  getChannelByClientSocket(_client_socket);

	if (!channel)
	{
		msg = "Error: user isn't in any channel\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return;
	}
	channel->setTopic(topic);
	msg = "Channel name changed to: ";
	msg += topic;
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Mode(std::string flag)
{
	std::string	msg;
	if (flag == "i") //Set/remove Invite-only channel
	{}
	else if (flag == "t") // Set/remove the restrictions of the TOPIC command to channel operators
	{}
	else if (flag == "k") //Set/remove the channel key (password)
	{}
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
	msg += "\"[HELP]\" \n";
	send(_client_socket, msg.c_str(), msg.length(), 0);
}

void	Server::Priv(std::string name, std::vector<std::string> msgs)
{
	std::string	msg = "";
	int			socketSend;

	if (getClientByName(name) == NULL)
	{
		msg = "error: Client not found\n";
		send(_client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	for (int i = 2; i < msgs.size(); i++)
		msg += msgs[i];
	socketSend = getClientByName(name)->getPollfd().fd;
	send(socketSend, msg.c_str(), msg.length(), 0);
}
