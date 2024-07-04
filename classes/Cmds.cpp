#include "../inc/Server.hpp"

void Server::Kick(std::string clientName, std::string channelName)
{
	//TODO - cambiar todos los couts por sends

	std::string	msg;
	if (getChannelByName(channelName) == channels.end())
	{
		msg = "error: wrong channel name: " + channelName + "\n";
		send(client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (getClientByName(clientName) == clientes.end())
	{
		msg = "error: wrong client name: " + clientName + "\n";
		send(client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}

	getChannelByName(channelName).eraseClient(getClientByName(clientName));
	std::cout << clientName << "erased from: " << channelName << std::endl;
}

void Server::Invite(std::string clientName, std::string channelName)
{
	std::string	msg;

	if (getClientByName(clientName) == clientes.end())
	{
		msg = "error: wrong client name: " + clientName + "\n";
		send(client_socket, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (getChannelByName(channelName) == channels.end())
		addChannel(channelName);
	getChannelByName(channelName).setClient(getClientByName(clientName));
	msg = clientName + " added to: " + channelName + "\n";
	send(client_socket, msg.c_str(), msg.length(), 0);
}

void Server::Topic()
{
	std::string	msg;
	msg = "topic\n";
	send(client_socket, msg.c_str(), msg.length(), 0);

	// TODO el channel del cliente que lo ha escirto print
}

void Server::Topic(std::string topicName)
{
	topicName +="";
	// TODO el channel del cliente que lo ha escirto set
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
	send(client_socket, msg.c_str(), msg.length(), 0);
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
	send(client_socket, msg.c_str(), msg.length(), 0);
}
// void Server::Priv()
// {
// 	TODO - flags / lo que sea
// }
