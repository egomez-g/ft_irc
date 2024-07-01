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

# include "client.hpp"
# include "channel.hpp"

class Client;
class Channel;

class Server
{
    private:
		/* Server conf */
		int			port;		/* Port of the server */
		int			server_fd;	/* Server comunication */
		std::string	password;	/* Server password */

		/* Clients data */
		std::vector<pollfd> poll_fds;	/* Conecctions fds */
		std::map<int, Client*> clients;	/* Clients information */

		/* Chats data */
		std::vector<Channel>	chats;

		sockaddr_in	adress;		/* Server socket */
		int			addrlen;	
		int			new_socket;
		//char		buffer[1024];


    public:
        int initServer(char **argv);
        int listenLoop();
        void closeServer();
		void acceptNewClient();
		void handleClientMessage(int client_socket);
		void removeClient(int client_socket);
		void checkPassword(int client_socket);
		bool addChannel(std::string nameChat, std::string nickname, Client *inviter);

		void addClient(std::string nameChat, Client *inviter);
		Client *getClientByNickname(std::string nickname);
		int Server::checkCmd(char *cmd, int client_socket);
};

