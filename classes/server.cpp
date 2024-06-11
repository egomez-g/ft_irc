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

	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = INADDR_ANY;
	adress.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&adress, sizeof(adress)) < 0)
	{
		perror("Binding");
		return(std::cout << "bind failed\n", 1);
	}
	addrlen = sizeof(adress);
	// buffer[1024] = {0};

	return (0);
}

int Server::listenLoop()
{
    while (1)
	{
		if (listen(server_fd, 3) < 0)
			return (std::cout << "listen failed\n", 1);

		if ((new_socket = accept(server_fd, (struct sockaddr *)&adress, (socklen_t*)&addrlen)) < 0)
			return (std::cout << "accept failed\n",1);

		recv(new_socket, buffer, 1024, 0);
		std::cout << "Mensaje recibido: "  << buffer << std::endl;

		std::string hello = "Hola desde el servidor";
		send(new_socket, hello.c_str(), strlen(hello.c_str()), 0);
		// std::cout << "Mensaje enviado\n";
	}
    return (0);
}

void Server::closeServer()
{
	close(new_socket);
	close(server_fd);
}
