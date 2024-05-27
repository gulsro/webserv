
#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"

class Location;

// The server will use the Host header from incoming HTTP requests
// to determine which virtual host configuration to apply.

// When trying to determine which server block to send a request to, Nginx will first try to decide based on 
// the specificity of the listen directive using the following rules:
//     -Nginx translates all “incomplete” listen directives by substituting missing values with their default values
//     so that each block can be evaluated by its IP address and port.

class Server
{
private:
    int port;
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