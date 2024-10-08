
#include "../inc/Server.hpp"

int Server::initServer(char **argv)
{
	_port = std::atoi(argv[1]);
	_password = argv[2];
	_validmsgflag = false;
	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return (std::cout << "socket failed" << std::endl, 1);
	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		return (perror("setsockopt"), 1);

	if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) == -1) 
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");

	sockaddr_in serv_address = {};
	bzero((char *) &serv_address, sizeof(serv_address));
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(_port);

	if (bind(_server_fd, (struct sockaddr *)&_adress, sizeof(_adress)) < 0)
	{
		perror("Binding");
		return(std::cout << "bind failed\n", 1);
	}
	_addrlen = sizeof(_adress);

	if (listen(_server_fd, 3) < 0)
		throw std::runtime_error("Error while listening on socket.");

	pollfd srv = {_server_fd, POLLIN, 0};
	srv.revents = 0;
	_poll_fds.clear();
	_poll_fds.push_back(srv);
	_sizeOfClients = _poll_fds.size();
	return (0);
}

int Server::listenLoop()
{
	while (!_stop)
	{
		int poll_count = poll(&(_poll_fds[0]), _poll_fds.size(), 100);
		if (poll_count == -1)
			return (0);
		for (size_t i = 0; i < _poll_fds.size(); ++i)
		{
			if (_poll_fds[i].revents & POLLIN)
			{
				if (_poll_fds[i].fd == _server_fd)
					acceptNewClient();
				else
				{
					_client_socket = _poll_fds[i].fd;
					handleClientMessage();
				}
		    }
		}
	}
    return (0);
}

int Server::handleClientMessage()
{
	if (!getClientByFd(_client_socket))
		return (0);

	std::string msg;

	int flag = 0;

    char buffer[513] = {0};
    int bytes_received = 0;

	while (flag == 0)
	{
		bytes_received = recv(_client_socket, buffer, sizeof(buffer), 0);
		if (std::strchr(buffer, '\n'))
			flag = 1;
	}

	if (bytes_received <= 0)
	{
        if (bytes_received == 0)
            std::cout << "Client disconnected: " << _client_socket << std::endl;
        else
            perror("recv");
        removeClient();
        return (0);
    }

	int asd = 0;
	while (asd < bytes_received)
	{
		if (buffer[asd] == '\r')
			buffer[asd] = '\0';
		asd++;
	}

	// if (buffer[bytes_received - 2] != '\n' && buffer[bytes_received - 1] != '\n')
	// {
	// 	msg += buffer;
	// 	return (0);
	// }

	if (buffer[bytes_received - 2] == '\n')
		buffer[bytes_received - 2] = '\0';
	else
		buffer[bytes_received - 1] = '\0';

	int i = 0;
	while (buffer[i] == ' '  || buffer[i] == '\0')
		++i;
	if (buffer[i] == '\n' || buffer[i] == '\0')
		return (0);

	if (bytes_received == 513)
	{
		if (!_validmsgflag)
			send(_client_socket, "Error: message too long max 512 char\n", 37, 0);
		_validmsgflag = true;
		return (0);
	}
	if (_validmsgflag)
	{
		_validmsgflag = false;
		return(0);
	}
	msg += buffer;
	if (msg == "EXIT")
	{
		removeClient();
		std::cout << "Client disconnected: " << _client_socket << std::endl;
		return (0);
	}
	if (getClientByFd(_client_socket)->getPassword() == "")
	{
		if (msg != _password)
		{
			if (std::strchr(msg.c_str(), ' '))
				return (0);
			send(_client_socket, "Incorrect password try again\n", 29, 0);
			return (0);
		}
		else
		{
			getClientByFd(_client_socket)->setPassword(buffer);
			send(_client_socket, "Insert Username\n", 16, 0);
		}
	}
	else if (getClientByFd(_client_socket)->getUsername() == "")
	{
		if (!(*buffer))
			return (0);
		if(!getClientByUsername(buffer))
		{
			getClientByFd(_client_socket)->setUsername(buffer);
			getClientByFd(_client_socket)->setNickname(buffer);
			msg =  "Welcome to our server, " + getClientByFd(_client_socket)->getUsername() + "\n";
			send(_client_socket, msg.c_str(), msg.length(), 0);
		}
		else
			send(_client_socket, "Username already exists, try again\n", 35, 0);
	}
	else
		parseCmd(buffer);

	return (0);
}

void Server::closeServer()
{
	for (size_t i = 0; i < _poll_fds.size(); ++i)
        close(_poll_fds[i].fd);
    _poll_fds.clear();
    close(_server_fd);
}
