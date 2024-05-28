
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Webserv.hpp"

// A function to assign values to server's members. (port, serverName etc.)


class Server;

class Config
{
private:
    int nbServer;
    std::vector<Server> serverList;

public:
    Config();
    ~Config();
    Config(Config& a);
    Config& operator=(const Config a);

    initServer
};

#endif