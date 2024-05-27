
#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>

#include "Location.hpp"

class Location;

class Server
{
private:
    int port;
    std::string server_name;
    std::string root;
    std::string index;

    unsigned long max_body_size;
    std::vector<Location> location_list;


public:
    Server();
    ~Server();
    Server(Server& a);
    Server& operator=(const Server a);


};

#endif