#include "Webserv.hpp"

Client::Client(int fd, int readyTo)
//    :clientFd(fd)
{
    //this->server = server;
    this->clientFd = fd;
    this->readyTo = readyTo;
	this->Request = NULL;
	this->Response = NULL;
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

int Client::getReadyToFlag() const
{
    return this->readyTo;
}

void Client::setReadyToFlag(int readyTo)
{
    this->readyTo = readyTo;
}

void	Client::setClientFdEvent(std::vector<struct pollfd>& pollfds, short events)
{
    for (auto& pfd : pollfds) {
        if (pfd.fd == clientFd) {
            pfd.events = events;
            break;
        }
    }
}
