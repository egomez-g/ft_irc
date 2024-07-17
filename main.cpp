
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
todo: 
Communication between client and server has to be done via TCP/IP (v4 or v6),

probar a meter ctl z datos a la mitad
-> Ctrl+D buguea todo. ¿Se puede gestionar? (nope)
*/
