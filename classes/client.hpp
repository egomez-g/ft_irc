#pragma once

#include "server.hpp"

#include <string>

class Client
{
	private:
		int socket;
		std::string nickname;
		std::string username;
		std::string password;
	public:
		Client(int socket);
		int getSocket() const;


		const std::string & getNickname();
		const std::string & getUsername();
		const std::string & getPassword();

		void setNickname(std::string const & nick);
		void setUsername(std::string const & user);
		void setPassword(std::string const & pass);
};
