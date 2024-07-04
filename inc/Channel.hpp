#pragma once

#include "Server.hpp"
#include "Client.hpp"

class Client;

class Channel
{
	private:
		std::vector<Client> _clients;
		std::string			_ChannelName;
		std::string			_topic;
	public:
		Channel(std::string ChannelName);
		~Channel();
		void				setChannelName(std::string ChannelName);
		void				setTopic(std::string topic);
		void				setClient(Client klaient);
		void				eraseClient(Client klaient);
		std::string			getTopic();
		std::string			getChannelName();
		std::vector<Client>	getClients();

		bool	operator==(std::vector<Channel>::iterator other);

};

