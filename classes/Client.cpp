#include "../inc/Client.hpp"

Client::Client(pollfd fd)
{
	_username = "";
	_nickname = "";
	_password = "";
	_loc 	  = "";
	_poll_fd  = fd;
}

Client::~Client()
{
}

pollfd Client::getPollfd()
{
	return (_poll_fd);
}

void Client::setUsername(std::string username)
{
	_username = username;
}

void Client::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void Client::setLoc(std::string loc)
{
	_loc = loc;
}

std::string Client::getUsername()
{
	return (_username);
}

std::string Client::getNickname()
{
	return (_nickname);
}

void Client::setPassword(std::string password)
{
	_password = password;
}

std::string Client::getPassword()
{
	return (_password);
}

std::string Client::getLoc()
{
	return (_loc);
}

bool	Client::operator==(std::vector<Client>::iterator other)
{
	if (this->getPollfd().fd == other->getPollfd().fd)
		return (true);
	return(false);
}