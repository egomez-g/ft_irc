#include "../inc/Server.hpp"

Client	& Server::getClientByFd(int fd)
{
	std::vector<Client>::iterator i;

	for (i = clientes.begin(); i != clientes.end(); i++)
	{
		if (i->getPollfd().fd == fd)
			return (*i);
	}
	return (*i);
}

Client	& Server::getClientByName(std::string name)
{
	std::vector<Client>::iterator i;

	for (i = clientes.begin(); i != clientes.end(); i++)
	{
		if (i->getUsername() == name)
			return (*i);
	}
	return (*i);
}

Channel	& Server::getChannelByName(std::string name)
{
	std::vector<Channel>::iterator i;

	for (i = channels.begin(); i != channels.end(); i++)
	{
		if (i->getChannelName() == name)
			return (*i);
	}
	return (*i);
}

void Server::acceptNewClient()
{
    client_socket = accept(server_fd, NULL, NULL);

    if (client_socket == -1)
	{
        perror("accept");
        return;
    }

    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    pollfd client_pollfd;
    client_pollfd.fd = client_socket;
    client_pollfd.events = POLLIN;
    poll_fds.push_back(client_pollfd);

    Client new_client = Client(client_pollfd);
    clientes.push_back(new_client);

    std::cout << "New client connected: " << client_socket << std::endl;
	send(client_socket, "Insert Password\n", 16, 0);
}

void Server::removeClient()
{
	std::vector<pollfd>::iterator it;
	std::vector<Client>::iterator aux;

	for (it = poll_fds.begin(); it != poll_fds.end(); it++)
	{
		if (it->fd == client_socket)
			break ;
	}
	for (aux = clientes.begin(); aux != clientes.end(); aux++)
	{
		if (aux->getPollfd().fd == client_socket)
			break ;
	}
	if (it != poll_fds.end())
		poll_fds.erase(it);
	if (aux != clientes.end())
		clientes.erase(aux);
}

void	Server::addChannel(std::string channelName)
{
	Channel newChannel(channelName);

	channels.push_back(newChannel);
}
