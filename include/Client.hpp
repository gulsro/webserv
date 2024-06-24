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
    Client(Server&, int fd);
    ~Client();

};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif
