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
    // HttpRequest&
    // HttpResponse&

public:
    Client(int fd);
    ~Client();

    int getClientFd();

};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif


//    Client* client = new Client(server, clientFd);
