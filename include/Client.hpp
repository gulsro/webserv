#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Webserv.hpp"

class Server;

class Client
{
private:
    Server& server;
    int clientFd;
//        Request&
//         Response&

public:
    Client(Server& server, int fd);
    ~Client();

    int getClientFd();

};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif


//    Client* client = new Client(server, clientFd);
