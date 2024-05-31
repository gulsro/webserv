#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Webserv.hpp"

class Server;

class Client
{

public:
    Client();
    ~Client();

};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif