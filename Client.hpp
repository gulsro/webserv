#ifndef CLIENT
# define CLIENT_HPP

#include "Webserv.hpp"

class Server;

class Client
{
private:
    const Server& server;

public:
    Client(const Server&);
    ~Client();
    std::ostream& operator<<(std::ostream& out, const Client& client);

};

#endif