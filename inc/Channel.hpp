#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "limits.h"

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
		unsigned long int	_clientSize;

	public:
		Channel(std::string ChannelName);
		~Channel();
		void				setName(std::string ChannelName);
		void				setTopic(std::string topic);
		void				setClient(Client client);
		void				setPassword(std::string password);
		void				setInvite(bool invite);
		void				setClearanceTopic(bool clearanceTopic);
		void				addAdmin(Client client);
		void				rmAdmin(Client client);

		void				eraseClient(Client client);
		std::string			getTopic();
		std::string			getName();
		std::vector<Client>	getClients();
		std::string			getPassword();
		std::vector<Client>	getAdmins();
		unsigned long int	getClientSize();
		void				setClientSize(unsigned long int new_size);


		bool				getInvite();
		bool				getClearanceTopic();
		bool				isAdmin(std::string client);
		Client				*getChannelClientByName(std::string name);

		bool	operator==(std::vector<Channel>::iterator other);

};

