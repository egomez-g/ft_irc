#include "../inc/Server.hpp"

int Server::initServer(char **argv)
{
	port = std::atoi(argv[1]);
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
				{
					client_socket = poll_fds[i].fd;
					handleClientMessage();
				}
		    }
		}
	}
    return (0);
}

void Server::handleClientMessage()
{
	if (getClientByFd(client_socket) == clientes.end())
		return ;

    char buffer[512] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

	if (bytes_received <= 0) 
	{
        if (bytes_received == 0)
            std::cout << "Client disconnected: " << client_socket << std::endl;
        else
            perror("recv");
        removeClient();
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
	if (getClientByFd(client_socket).getPassword() == "")
	{
		if (buffer != password)
		{
			std::cout << client_socket << " INCORRECT PASSWORD" << std::endl;
			removeClient();
			exit (1); /*TODO no se puede*/
		}
		else
		{
			getClientByFd(client_socket).setPassword(buffer);
			send(client_socket, "Insert Username\n", 16, 0);
		}
	}
	else if (getClientByFd(client_socket).getUsername() == "")
	{
		if(getClientByName(buffer) == clientes.end())
			getClientByFd(client_socket).setUsername(buffer);
		else
			send(client_socket, "Username already exists, try again\n", 35, 0);
	}
	else
	{
		//TODO: parsear los mensajes
		parseCmd(buffer);
	}
}


void Server::closeServer()
{

}