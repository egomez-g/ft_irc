#include "../inc/Server.hpp"

std::vector<std::string> split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}
	res.push_back (s.substr (pos_start));
	return (res);
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
			send(client_socket, msg.c_str(), msg.length(), 0);
		}
		else
			Kick(aux[1], aux[2]);
	}
	else if (aux[0] == "INV")
	{
		if (aux[1].empty() || aux[2].empty())
		{
			msg = "invite client failed, type HELP for help\n";
			send(client_socket, msg.c_str(), msg.length(), 0);
		}
		else
			Invite(aux[1], aux[2]);		
	}
	else if (aux[0] == "TOPIC")
	{
		if (aux[1].empty())
			Topic();
		else
			Topic(aux[1]);
	}
	else if (aux[0] == "MODE")
	{
		if (aux[1].empty())
			Mode(aux[1]);
	}
	// else if (aux[0] == "PRIV")
	// {
	// 	if (aux[1].empty() || aux[2].empty())
	// 		Priv(aux[1], aux[2]);
	// }
	else if (aux[0] == "HELP")
	{
		Help();
	}
	else
		std::cout << "Type HELP for help" << std::endl;


	return (0);
}