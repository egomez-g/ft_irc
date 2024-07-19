#include "../inc/Server.hpp"

Client	* Server::getClientByFd(int fd)
{
	int clients_size = _clients.size();

	for(int i = 0; i < clients_size; i++)
	{
		if (_clients[i].getPollfd().fd == fd)
			return (&_clients[i]);
	}
	return (NULL);
}

Client	* Server::getClientByUsername(std::string name)
{
	int clients_size = _clients.size();

	for(int i = 0; i < clients_size; i++)
	{
		if (_clients[i].getUsername() == name)
			return (&_clients[i]);
	}
	return (NULL);
}

Client	* Server::getClientByNickname(std::string name)
{
	int clients_size = _clients.size();

	for(int i = 0; i < clients_size; i++)
	{
		if (_clients[i].getNickname() == name)
			return (&_clients[i]);
	}
	return (NULL);
}

Channel	* Server::getChannelByName(std::string name)
{
	int channels_size = _channels.size();

	for(int i = 0; i < channels_size; i++)
	{
		if (_channels[i].getName() == name)
			return (&_channels[i]);
	}
	return (NULL);
}

Channel *Server::getChannelByClientSocket(int socket)
{
	int channels_size = _channels.size();
	for (int i = 0; i < channels_size; i++)
	{
		int clients_size = _channels[i].getClients().size();

		for (int j = 0; j < clients_size; j++)
		{
			if (_channels[i].getClients()[j].getPollfd().fd == socket)
				return (&_channels[i]);
		}		
	}
	return (NULL);
}

void Server::acceptNewClient()
{
    _client_socket = accept(_server_fd, NULL, NULL);

    if (_client_socket == -1)
	{
        perror("accept");
        return ;
    }

    fcntl(_client_socket, F_SETFL, O_NONBLOCK);

    pollfd client_pollfd;
    client_pollfd.fd = _client_socket;
    client_pollfd.events = POLLIN;
    _poll_fds.push_back(client_pollfd);

    Client new_client = Client(client_pollfd);
    _clients.push_back(new_client);

    std::cout << "New client connected: " << _client_socket << std::endl;
	send(_client_socket, "Insert Password\n", 16, 0);
}

void Server::removeClient()
{
	std::vector<pollfd>::iterator it;
	std::vector<Client>::iterator aux;

	std::vector<Channel>::iterator pingo;
	
	for (pingo = _channels.begin(); pingo != _channels.end(); pingo++)
	{
		if (pingo->getChannelClientByName(getClientByFd(_client_socket)->getUsername()))
			pingo->eraseClient(*getClientByFd(_client_socket));
	}

	for (it = _poll_fds.begin(); it != _poll_fds.end(); it++)
	{
		if (it->fd == _client_socket)
			break ;
	}
	for (aux = _clients.begin(); aux != _clients.end(); aux++)
	{
		if (aux->getPollfd().fd == _client_socket)
			break ;
	}
	if (it != _poll_fds.end())
		_poll_fds.erase(it);
	if (aux != _clients.end())
		_clients.erase(aux);
	close(_client_socket);
}

void	Server::addChannel(std::string channelName)
{
	Channel newChannel(channelName);

	newChannel.setClient(*getClientByFd(_client_socket));
	newChannel.addAdmin(*getClientByFd(_client_socket));
	_channels.push_back(newChannel);
}

void Server::SetStop(bool state)
{
	_stop = state;
}
