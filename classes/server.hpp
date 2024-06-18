#pragma once

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

#include "client.hpp"

class Client;

class Server
{
    private:
		int			port;
		int			server_fd;
		std::string	password;

		std::vector<pollfd> poll_fds;
		std::map<int, Client*> clients;

		sockaddr_in	adress;
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
};

