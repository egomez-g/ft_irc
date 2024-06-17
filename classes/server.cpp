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

	// Configurar el socket para reutilizar la dirección y el puerto
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
    int client_socket = accept(server_fd, NULL, NULL);
    if (client_socket == -1) {
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
}

void Server::handleClientMessage(int client_socket)
{
    char buffer[512];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        if (bytes_received == 0)
            std::cout << "Client disconnected: " << client_socket << std::endl;
        else
            perror("recv");
        //removeClient(client_socket);
        return;
    }

    buffer[bytes_received] = '\0';
    std::cout << "Received message from " << client_socket << ": " << buffer << std::endl;

    // Handle the received message (e.g., parse and execute commands)
    // ...
}

void Server::closeServer()
{
	close(new_socket);
	close(server_fd);
}
