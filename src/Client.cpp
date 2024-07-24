#include "Webserv.hpp"


Client::Client(int fd, int readyTo)
{
    this->clientFd = fd;
    this->readyTo = readyTo;
	this->Request = NULL;
	this->Response = NULL;
	this->cgi = NULL;
}

Client::~Client()
{
	delete cgi;
    delete this->Response;
	delete this->Request; 
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
	#ifdef CGI
		std::cout << PINK << "[ Client ] handleCgiRequest" << DEFAULT << std::endl; 
	#endif
    for (auto& pfd : pollfds) {
        if (pfd.fd == clientFd) {
            pfd.events = events;
            break;
        }
    }
}


void  Client::handleCgiRequest(ServerManager *serverManager)
{
	#ifdef CGI
		std::cout << PINK << "[ Client ] handleCgiRequest" << DEFAULT << std::endl; 
	#endif
	Cgi *cgi = new Cgi((*this->Request), this->Request->getReqLocation(), this->Request->getReqServer(), *serverManager);
	this->cgi = cgi; // Assigning client cgi
 
	std::string	cgiFilename = this->cgi->getCgiFile();
	std::ifstream file(cgiFilename);

	// if it's not allowed method, it'll throw an error.
	this->Request->checkAllowedMethods(this->Request->getMethod());

	if (file.is_open())
	{
		// pass full request to the cgiInput to execute cgi.
		if (this->getRequest()->getMethod() == "POST")
		{
			this->getCgi()->setCgiInput(this->getRequest()->getBody());
		}
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

void	Client::finishCgi()
{
	#ifdef CGI
		std::cout << PINK << "[ Client ] finishCgi" << DEFAULT << std::endl; 
	#endif
	std::vector<char> cgiResponse = this->cgi->getCgiOutput();
	this->Response->setContent(std::string(cgiResponse.begin(), cgiResponse.end()));
	this->Response->setCompleted(true);
	close (this->cgi->getPipeRead());
    if (this->getCgi()->isRunningCgi() == true)
    {
        kill(this->getCgi()->getChildPid(), SIGKILL);
    }
	delete this->cgi;
	this->cgi = nullptr;
}