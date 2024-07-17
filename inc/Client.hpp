#pragma once
#include "Server.hpp"

class Client
{
	private:
		std::string _username;
		std::string _nickname;
		std::string _password;
		std::string	_loc;
		pollfd _poll_fd;
		Client();

	public:
		Client(pollfd fd);
		~Client();

		void 		setUsername(std::string username);
		void 		setNickname(std::string nickname);
		void 		setPassword(std::string password);
		void 		setLoc(std::string loc);
		std::string	getUsername();
		std::string	getPassword();
		std::string getLoc();
		pollfd 		getPollfd();

		bool	operator==(std::vector<Client>::iterator other);
};