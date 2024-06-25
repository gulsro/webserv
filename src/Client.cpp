#include "Webserv.hpp"

Client::Client(int fd)
    :clientFd(fd)
{
    //this->server = server;
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