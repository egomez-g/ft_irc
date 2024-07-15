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
		std::string			_password;
		bool				_invite;
		bool				_clearanceTopic;

		
	public:
		Channel(std::string ChannelName);
		~Channel();
		void				setName(std::string ChannelName);
		void				setTopic(std::string topic);
		void				setClient(Client klaient);
		void				setPassword(std::string password);
		void				setInvite(bool invite);
		void				setClearanceTopic(bool clearanceTopic);

		void				eraseClient(Client klaient);
		std::string			getTopic();
		std::string			getName();
		std::vector<Client>	getClients();
		std::string			getPassword();
		bool				getInvite();
		bool				getClearanceTopic();

		bool	operator==(std::vector<Channel>::iterator other);

};

