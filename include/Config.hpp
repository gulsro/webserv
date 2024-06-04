
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Webserv.hpp"

class Server;

class Config
{
private:
    std::string configFile;
    std::string content;
    int nbServer;
    std::vector<Server> serverList;
    std::vector<std::string> serverCont;


public:
    Config();
    ~Config();
    Config(Config& a);
    Config& operator=(const Config a);

    void    setConfigFile(std::string file);
    void    parseConfig();
    void    checkConfig(std::ifstream& file);
    bool    isBlank(std::ifstream& file);
    void    readConfig(std::ifstream& file);
    void    cntServer();
    void    splitServer();
};

#endif