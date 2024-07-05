#include "../inc/Server.hpp"

std::vector<std::string> split(std::string str, std::string delimiter)
{
	size_t pos_start = 0;
	size_t delim_len = delimiter.length();
	size_t pos_end;

	std::string aux;
	std::vector<std::string> splitted;

	while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos)
	{
		aux = str.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		
		splitted.push_back(aux);
	}

	splitted.push_back (str.substr (pos_start));
	return (splitted);
}

int	Server::parseCmd(char * cmd)
{
	std::vector<std::string> aux = split(cmd, " ");
	std::string	msg;

	if (aux[0] == "KICK")
	{
		if (aux[1].empty() || aux[2].empty())
		{
			msg = "kick client failed, type HELP for help\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
		}
		else
			Kick(aux[1], aux[2]);
		return (1);
	}
	else if (aux[0] == "INV")
	{
		if (aux[1].empty() || aux[2].empty())
		{
			msg = "invite client failed, type HELP for help\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
		}
		else
			Invite(aux[1], aux[2]);
		return (1);
	}
	else if (aux[0] == "TOPIC")
	{
		if (aux[1].empty())
			Topic();
		else
			Topic(aux[1]);
		return (1);
	}
	else if (aux[0] == "MODE")
	{
		if (aux[1].empty())
		{
			msg = "wrong use of [MODE], type HELP for help\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
		}
		else
			Mode(aux[1]);
		return (1);
	}
	else if (aux[0] == "PRIV")
	{
		if (aux[1].empty() || aux[2].empty())
		{
			msg = "wrong use of [MODE], type HELP for help\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
		}
		else
			Priv(aux[1], aux);
		return (1);
	}
	else if (aux[0] == "HELP")
	{
		Help();
		return (1);
	}
	return (0);
}