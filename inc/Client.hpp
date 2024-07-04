#pragma once
#include "Server.hpp"

class Client
{
	private:
		std::string _username;
		std::string _password;
		pollfd _pollfd;
		Client();

	public:
		Client(pollfd fd);
		~Client();

		void 		setUsername(std::string username);
		std::string	getUsername();
		void 		setPassword(std::string password);
		std::string	getPassword();
		pollfd 		getPollfd();

		bool	operator==(std::vector<Client>::iterator other);

};