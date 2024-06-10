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


int ft_stoi(const std::string& str)
{
    int num;
    std::stringstream ss(str);

    ss >> num;
    return (num);
}

int main(int argc, char **argv)
{
	int			port;
	int			server_fd;
	std::string	password;

	sockaddr_in	adress;
	int 		addrlen = sizeof(adress);
	int			new_socket;
    char 		buffer[1024] = {0};
	std::string	lpastor;

	if (argc != 3)
	{
		std::cout << "Error, wrong number of arguments.\n";
		return (1);
	}
	port = ft_stoi(argv[1]);
	password = argv[2];
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return (std::cout << "socket failed" << std::endl, 1);

	/******************************************************/

	// Configurar el socket para reutilizar la dirección y el puerto
	int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        return 1;
    }

	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = INADDR_ANY;
	adress.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&adress, sizeof(adress)) < 0)
	{
		perror("Binding");
		return(std::cout << "bind failed\n",1);
	}

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
	/******************************************************/

	close(new_socket);
    close(server_fd);

	return (0);
}