#include "Webserv.hpp"


Client::Client(int fd, int readyTo)
//    :clientFd(fd)
{
    //this->server = server;
    this->clientFd = fd;
    this->readyTo = readyTo;
	this->Request = NULL;
	this->Response = NULL;
	this->cgi = NULL;
    std::cout << "Client constructor is called" << std::endl;
}

Client::~Client()
{
    std::cout << "Client destructor is called" << std::endl;
	delete cgi;
    // if (this->cgi != nullptr)
    //     delete this->cgi;
    // if (this->Request != nullptr)
    //     delete this->Request;
    // if (this->Response != nullptr)
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
	// HttpResponse *response = this->Response;
	Cgi *cgi = new Cgi((*this->Request), this->Request->getReqLocation(), this->Request->getReqServer(), *serverManager);
	this->cgi = cgi; // Assigning client cgi
 
	std::cout << YEL << this->cgi->getCgiFile() << RES << std::endl;

	std::string	cgiFilename = this->cgi->getCgiFile();
	std::ifstream file(cgiFilename);

	// if it's not allowed method, it'll throw an error.
	this->Request->checkAllowedMethods(this->Request->getMethod());

	// this->cgi->writeToCgi();
	if (file.is_open())
	{
		// Pass the full request to the cgiInput to execute cgi.
		if (this->getRequest()->getMethod() == "POST")
		{
			// setPostBody
			// this->getCgi()->setPostBody(*this->getRequest());
			this->getCgi()->setCgiInput(this->getRequest()->getBody());
		}
		this->cgi->execCGI();
		// this->  Request->setIsCgi(false);
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
	std::string cont(cgiResponse.begin(), cgiResponse.end());
	this->Response->setContent(cont);
	// this->Response->setContent(this->cgi->getCgiOutput().data());
	this->Response->setCompleted(true);
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!revove pipe from pollfs
	close (this->cgi->getPipeRead());
    if (this->getCgi()->isRunningCgi() == true)
    {
        kill(this->getCgi()->getChildPid(), SIGKILL);
    }
	//this->cgi->rmPipesFromPollfd();
	delete this->cgi;
	this->cgi = nullptr;
}