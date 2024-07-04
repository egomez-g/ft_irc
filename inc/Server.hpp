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
		int			port;		/* Port of the server */
		int			server_fd;	/* Server comunication */
		std::string	password;	/* Server password */

		std::vector<pollfd> poll_fds;

		sockaddr_in	adress;		/* Server socket */
		int			addrlen;	
		int			new_socket;

		std::vector<Channel>	channels;
		std::vector<Client> 	clientes;

		int			client_socket;
	public:
		int		initServer(char **argv);
		int		listenLoop();
		void	handleClientMessage();
		void 	acceptNewClient();
		void	removeClient();
		void	closeServer();
		
		//utils
		Client &getClientByFd(int fd);
		Client &getClientByName(std::string name);
		Channel &getChannelByName(std::string name);
		void	addChannel(std::string channelName);
		//parse
		int		parseCmd(char *cmd);

		//cmds
		void	Kick(std::string clientName, std::string channelName);
		void	Invite(std::string clientName, std::string channelName);
		void	Topic();
		void	Topic(std::string topicName);
		void	Mode(std::string flag);
		void	Help();
		void	Priv(std::string name, std::string msg);
};