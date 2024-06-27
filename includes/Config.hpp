
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Webserv.hpp"

class Server;

class Config
{
private:
    std::string configFile; // config file name
    std::string content; // content of config file in one string
    int nbServer;
    std::vector<Server*> serverList; // list for Server class
    std::vector<std::string> serverCont; //splited contents inside each server {}


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
    void    splitServer();

    void    parseServer();
    void    parseLocation();

    const std::vector<Server*>  getServerList() { return this->serverList; }

};

std::size_t findScopeBegin(std::string& content, size_t found);
std::size_t findScopeEnd(std::string& content, size_t begin);

#endif