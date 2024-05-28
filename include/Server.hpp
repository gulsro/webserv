
#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"

class Location;

class Server
{
private:
    int port;
    int serverFd;
    std::string serverName;
    std::string root;
    std::string index;

    unsigned long maxBodySize;
    std::vector<Location> locationList;


public:
    Server();
    ~Server();
    Server(Server& a);
    Server& operator=(const Server a);


};

#endif