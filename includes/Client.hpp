#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Webserv.hpp"

class Server;
class HttpRequest;
class HttpResponse;

enum e_readyTo
{
	READ,
	WRITE
};

class Client
{
private:
    //Server& server;
    int clientFd;
	int readyTo;
	HttpRequest		*Request;
	HttpResponse	*Response;
	Cgi				*cgi;

public:
    Client(int fd, int readyTo);
    ~Client();

    int 			getClientFd();
	HttpRequest		*getRequest() const { return Request; }
	HttpResponse	*getResponse() const { return Response; }
	int getReadyToFlag() const;

	void	setRequest(HttpRequest	*request) { Request = request; }
	void	setResponse(HttpResponse	*response) { Response = response; }	
	void 	setReadyToFlag(int readyTo);
	void	setClientFdEvent(std::vector<struct pollfd>& pollfds, short events);

	void  handleCgiRequest();
};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif


//    Client* client = new Client(server, clientFd);
