
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Webserv.hpp"

// A function to assign values to server's members. (port, serverName etc.)


class Server;

class Config
{
private:
    int nbServer;
    std::vector<Server> servers;

public:
    Config();
    ~Config();
    Config(Config& a);
    
    //functions that we need (??)
    
    //initServer(); -declare single virtualserver
    //configServer(); -assign values(after parsing) in a loop


};

std::ostream operator<<(std::ostream& out, const Config& config);

#endif