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


void  Client::handleCgiRequest()
{
	HttpResponse *response = this->Response;
	Cgi cgi((*this->Request), this->Request->getReqLocation(), this->Request->getReqServer());
	this->cgi = &cgi; // Assigning client cgi
	std::cout << YEL << this->cgi->getCgiFile() << RES << std::endl;

	std::string	cgiFilename = this->cgi->getCgiFile();
	std::ifstream file(cgiFilename);

	// if it's not allowed method, it'll throw an error.
	this->Request->checkAllowedMethods(this->Request->getMethod());

	if (file.is_open())
	{
		// this->cgi->write;
		this->cgi->execCGI();
		file.close();	
	}
	else
	{
		if (access(cgiFilename.c_str(), F_OK) != 0)
			throw ErrorCodeException(STATUS_NOT_FOUND);
		else
			throw ErrorCodeException(STATUS_FORBIDDEN);
	}
}