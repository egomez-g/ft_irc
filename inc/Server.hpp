#pragma once

# include <iostream>
# include <cstring>
# include <sstream>
# include <vector>
# include <map>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include <iomanip>
# include <stdio.h>
# include <stdlib.h>

#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server
{
	private:

		/* Server conf */
		int			_port;		/* Port of the server */
		int			_server_fd;	/* Server comunication */
		std::string	_password;	/* Server password */

		std::vector<pollfd> _poll_fds;

		sockaddr_in	_adress;		/* Server socket */
		int			_addrlen;	
		int			_new_socket;

		std::vector<Channel>	_channels;
		std::vector<Client> 	_clients;

		int			_client_socket;

	public:
		int		initServer(char **argv);
		int		listenLoop();
		void	handleClientMessage();
		void 	acceptNewClient();
		void	removeClient();
		void	closeServer();
		
		//utils
		Client	*getClientByFd(int fd);
		Client	*getClientByName(std::string name);
		Channel	*getChannelByName(std::string name);
		Channel	*getChannelByClientSocket(int socket);
		void	addChannel(std::string channelName);
		
		//parse
		int		parseCmd(char *cmd);

		//cmds
		void	Kick(std::string clientName, std::string channelName);
		void	Invite(std::string clientName, std::string channelName);
		void	Topic();
		void	Topic(std::vector<std::string> msgs);
		void	Mode(std::string flag, std::vector<std::string> msgs);
		void	Help();
		void	Priv(std::string name, std::vector<std::string> msgs);
		void	move(std::string name);
		void	sendToAll(std::vector<std::string> msgs);
};