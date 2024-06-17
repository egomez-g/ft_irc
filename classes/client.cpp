#include "client.hpp"

Client::Client(int socket) : socket(socket) {}

int Client::getSocket() const
{
    return (socket);
}
