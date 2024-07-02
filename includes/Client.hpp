#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Webserv.hpp"

class Server;
class HttpRequest;
class HttpResponse;

class Client
{
private:
    //Server& server;
    int clientFd;
	HttpRequest	*Request;
	HttpResponse	*Response;
//         Response&

public:
    Client(int fd);
    ~Client();

    int 		getClientFd();
	HttpRequest		*getRequest() const { return Request; }
	HttpResponse	*getResponse() const { return Response; }
	void	setRequest(HttpRequest	*request) { Request = request; }
	void	setResponse(HttpResponse	*response) { Response = response; }	
};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif


//    Client* client = new Client(server, clientFd);
