#include "server.hpp"

int ft_stoi(const std::string& str)
{
	int num;
	std::stringstream ss(str);

	ss >> num;
	return (num);
}

int Server::initServer(char **argv)
{
	port = ft_stoi(argv[1]);
	password = argv[2];
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return (std::cout << "socket failed" << std::endl, 1);
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		return (perror("setsockopt"), 1);

	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) 
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");

	sockaddr_in serv_address = {};
	bzero((char *) &serv_address, sizeof(serv_address));
	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = INADDR_ANY;
	adress.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&adress, sizeof(adress)) < 0)
	{
		perror("Binding");
		return(std::cout << "bind failed\n", 1);
	}
	addrlen = sizeof(adress);

	if (listen(server_fd, 3) < 0)
		throw std::runtime_error("Error while listening on socket.");

	pollfd srv = {server_fd, POLLIN, 0};
	poll_fds.push_back(srv);
	return (0);
}

int Server::listenLoop()
{
	while (1)
	{
        int poll_count = poll(poll_fds.data(), poll_fds.size(), 500000);
        if (poll_count == -1)
		{
            perror("poll");
            exit(1);
        }

        for (size_t i = 0; i < poll_fds.size(); ++i)
		{
            if (poll_fds[i].revents & POLLIN)
			{
                if (poll_fds[i].fd == server_fd)
                    acceptNewClient();
				else
                    handleClientMessage(poll_fds[i].fd);
		    }
		}
	}
    return (0);
}

void Server::acceptNewClient()
{
	std::string msgBuffer;
    int client_socket = accept(server_fd, NULL, NULL);

    if (client_socket == -1)
	{
        perror("accept");
        return;
    }

    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    Client* new_client = new Client(client_socket);
    clients[client_socket] = new_client;

    pollfd client_pollfd;
    client_pollfd.fd = client_socket;
    client_pollfd.events = POLLIN;
    poll_fds.push_back(client_pollfd);

    std::cout << "New client connected: " << client_socket << std::endl;

	msgBuffer = "Insert Password\n";
	send(client_socket, msgBuffer.c_str(), msgBuffer.length(), 0);
}

void Server::removeClient(int client_socket)
{
	std::vector<pollfd>::iterator it;

	for (it = poll_fds.begin(); it != poll_fds.end(); it++)
	{
		if (it->fd == client_socket)
			break ;
	}
	if (it != poll_fds.end())
		poll_fds.erase(it);
}

void Server::handleClientMessage(int client_socket)
{
	if (clients.find(client_socket) == clients.end())
		return ;

    char buffer[512] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

	if (bytes_received <= 0) 
	{
        if (bytes_received == 0)
            std::cout << "Client disconnected: " << client_socket << std::endl;
        else
            perror("recv");
        removeClient(client_socket);
        return;
    }

	if (buffer[bytes_received - 1] == '\n')
		buffer[bytes_received - 1] = '\0';
	else
		buffer[bytes_received] = '\0';
	
	//check input spaces tal noseque
	int i = 0;
	while (buffer[i] == ' ')
		++i;
	if (buffer[i] == '\n')
		return;

	if (clients.find(client_socket)->second->getPassword() == "")
	{
		if (buffer != password)
		{
			std::cout << client_socket << " INCORRECT PASSWORD" << std::endl;;
			removeClient(client_socket);
			exit (1);
		}
		else
		{
			clients.find(client_socket)->second->setPassword(buffer);
			std::cout << client_socket << " CORRECT PASSWORD: " << clients.find(client_socket)->second->getPassword() << std::endl;

			std::string msgBuffer;
			msgBuffer = "Insert Username\n";
			send(client_socket, msgBuffer.c_str(), msgBuffer.length(), 0);
		}
	}
	else if (clients.find(client_socket)->second->getUsername() == "")
	{
		/*TODO: no puede haber 2 nicknames iguales*/
		clients.find(client_socket)->second->setUsername(buffer);
		clients.find(client_socket)->second->setNickname(buffer);
		std::cout << client_socket << " USERNAME SET TO " <<  clients.find(client_socket)->second->getUsername() << std::endl;
	}
	else
	{
		std::cout << "Received message from " << clients.find(client_socket)->second->getUsername() << ": " << buffer << std::endl;
		addChannel("Chat", "ua", clients.find(client_socket)->second);
		if (chats.empty())
			std::cout << "Obv bb" << std::endl;
		std::cout << "chat bien?" << chats.begin()->getName() << std::endl;
	}
}

bool Server::addChannel(std::string nameChat, std::string nickname, Client *inviter)
{
	std::vector<Channel>::iterator it_chat;
	std::map<int, Client*>::iterator it_nick;
	

	/*Buscamos si existe el cliente*/
	
	for(it_nick = clients.begin(); it_nick != clients.end(); it_nick++)
	{
		if (it_nick->second->getNickname() == nickname)
			break ;
	}
	if (it_nick == clients.end())
		return (false);

	/*Miramos si existe el canal, sino existe lo creamos si existe lo añadimos*/
	for (it_chat = chats.begin(); it_chat != chats.end(); it_chat++)
	{
		if (it_chat->getName() == nameChat)
			break ;
	}
	if (it_chat == chats.end())
	{
		//lo creamos
		std::cout << it_nick->second->getNickname() << std::endl;
		chats.push_back(Channel(nameChat));
		addClient(nameChat, inviter);
		addClient(nameChat, getClientByNickname(nickname));
	}
	else
	{
		//lo annadimos
		addClient(nameChat, inviter);
		addClient(nameChat, getClientByNickname(nickname));
	}

	return (true);
}

void Server::closeServer()
{
	close(new_socket);
	close(server_fd);
}


void Server::addClient(std::string nameChat, Client *inviter)
{
	std::vector<Channel>::iterator it;

	for (it = chats.begin(); it != chats.end(); it++)
	{
		if (it->getName() == nameChat)
		{
			it->addFd(inviter);
		}
	}
}

Client *Server::getClientByNickname(std::string nickname)
{
	std::map<int, Client*>::iterator it;

	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname().compare(nickname) == 0)
			return (it->second);
	}
	return (0);
}