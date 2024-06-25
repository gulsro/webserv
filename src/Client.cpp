#include "Webserv.hpp"

Client::Client(Server& server, int fd)
    :server(server), clientFd(fd)
{
    this->server = server;
    this->clientFd = fd; 
    std::cout << "Client constructor is called" << std::endl;
}

Client::~Client()
{
    std::cout << "Client destructor is called" << std::endl;
}

int Client::getClientFd()
{
    return this->clientFd;
}