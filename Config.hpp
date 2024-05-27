
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <iostream>
#include "Server.hpp"

class Server;

class Config
{
private:
    int nb_server;
    std::vector<Server> server_list;

public:
    Config();
    ~Config();
    Config(Config& a);
    Config& operator=(const Config a);


};

#endif