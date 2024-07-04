#include "../inc/Client.hpp"

Client::Client(pollfd fd)
{
	_username = "";
	_password = "";
	_pollfd = fd;
}

Client::~Client()
{
}

pollfd Client::getPollfd()
{
	return (_pollfd);
}

void Client::setUsername(std::string username)
{
	_username = username;
}

std::string Client::getUsername()
{
	return (_username);
}

void Client::setPassword(std::string password)
{
	_password = password;
}

std::string Client::getPassword()
{
	return (_password);
}

bool	Client::operator==(std::vector<Client>::iterator other)
{
	if (this->getPollfd().fd == other->getPollfd().fd)
		return (true);
	return(false);
}