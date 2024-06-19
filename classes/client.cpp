#include "client.hpp"

Client::Client(int socket) : socket(socket) {nickname = ""; username = ""; password= "";}

int Client::getSocket() const
{
    return (socket);
}

const std::string &  Client::getNickname()
{
    return (nickname);
}

const std::string &  Client::getUsername()
{
    return (username);
}

const std::string &  Client::getPassword()
{
    return (password);
}


void  Client::setNickname(std::string const & nick)
{
    nickname = nick;
}

void  Client::setUsername(std::string const & user)
{
    username = user;
}

void  Client::setPassword(std::string const & pass)
{
    password = pass;
}
