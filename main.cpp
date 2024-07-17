
#include "inc/Client.hpp"
#include "inc/Server.hpp"
#include "inc/Channel.hpp"

int main(int argc, char **argv)
{
	Server server;

	if (argc != 3)
	{
		std::cout << "Error, wrong number of arguments.\n";
		return (1);
	}
	if (std::strchr(argv[2], ' '))
	{
		std::cout << "Error, invalid password.\n";
		return (1);
	}
	if (server.initServer(argv) == 1)
		return (1);

	if (server.listenLoop() == 1)
		return (1);

	server.closeServer();

	return (0);
}

/*
todo: set nickname,
Communication between client and server has to be done via TCP/IP (v4 or v6),
probar a meter ctl z datos a la mitad
-> Ctrl+D buguea todo. ¿Se puede gestionar?
-> Dos clientes en el mismo chat. Ctrl+C en uno y en el otro envias mensajes
-> Si hacer Ctrl+C, pero te vuelves a meter con el mismo usuario tambien lo da
-> Hecho a uno, no le llegan los mensajes pero los puede mandar
-> Si echo al admin, nadie tiene permisos (xd xd xd)
-> Si metes texto muy grande, peta:
	Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam metus sapien, dictum a dui id, malesuada hendrerit neque. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Pellentesque dignissim urna tortor, vel scelerisque risus ultrices id. Donec lobortis efficitur tempus. Cras vehicula lacus a dui pretium, sit amet volutpat massa euismod. Sed faucibus vitae risus eu vehicula. Praesent pharetra vestibulum eros, id egestas elit scelerisque vitae. Morbi efficitur aliquet felis, malesuada tempor felis tristique id. Donec sed sapien leo. Nulla facilisi. Nulla sit amet est volutpat, aliquam nisi eu, posuere felis. Praesent consequat est ac nulla tristique, mollis fermentum leo feugiat. Quisque molestie lectus vitae lorem semper, lobortis cursus ante efficitur.
*/