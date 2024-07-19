
#include "inc/Client.hpp"
#include "inc/Server.hpp"
#include "inc/Channel.hpp"

Server server;

void	int_handler(int sig)
{
	if (sig == SIGINT)
		server.SetStop(true);
}

int main(int argc, char **argv)
{

	if (argc != 3)
	{
		std::cout << "Error, wrong number of arguments.\n";
		return (1);
	}
	if( std::atoi(argv[1]) < 1)
	{
		std::cout << "Error, invalid port.\n";
		return (1);
	}
	if (std::strchr(argv[2], ' '))
	{
		std::cout << "Error, invalid password.\n";
		return (1);
	}
	server.SetStop(false);
	signal(SIGINT, int_handler);
	if (server.initServer(argv) == 1)
		return (1);
	if (server.listenLoop() == 1)
		return (1);
	server.closeServer();
	return (0);
}


/*
-> Parsear bien los datos
-> Ctrl+D buguea todo. ¿Se puede gestionar? (nope)


erease client solo funciona en kick¿?¿?¿?¿?¿?¿?
join bugg
no te pide la passs random
*/
