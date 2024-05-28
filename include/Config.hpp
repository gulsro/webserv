
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Webserv.hpp"

class Server;

class Config
{
private:
    std::string configFile;
    int nbServer;
    std::vector<Server> serverList;

public:
    Config();
    ~Config();
    Config(Config& a);
    Config& operator=(const Config a);

    void    setConfigFile(std::string file);
    void    checkConfig();
    bool    isWhitespace(std::fstream& file);
    void    eraseWhitespace(std::fstream& file);


};

#endif