#pragma once

#include "Server.hpp"
#include "Client.hpp"

class Client;

class Channel
{
	private:
		std::vector<Client> _clients;
		std::vector<Client>	_admins;
		std::string			_channel_name;
		std::string			_topic;
	public:
		Channel(std::string ChannelName);
		~Channel();
		void				setName(std::string ChannelName);
		void				setTopic(std::string topic);
		void				setClient(Client klaient);
		void				eraseClient(Client klaient);
		std::string			getTopic();
		std::string			getName();
		std::vector<Client>	getClients();

		bool	operator==(std::vector<Channel>::iterator other);

};

