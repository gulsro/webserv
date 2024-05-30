
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Webserv.hpp"

class Server;
class ServerManager;
//class Location;

// A function to assign values to server's members. (port, serverName etc.)

struct server_ {
    int port;
    int serverFd;
    std::string host; //address
    std::vector<std::string> serverNames;
    std::string root;
    std::string index;
    unsigned long maxBodySize;
    
  //  std::vector<Location> locationList;
};



// class Config
// {
// private:
//     int nbServer;
//     std::vector<Server> servers;

// public:
//     Config();
//     ~Config();
//     Config(Config& a);
    
//     //functions that we need (??)
    
//     //initServer(); -declare single virtualserver
//     //configServer(); -assign values(after parsing) in a loop


// };

//std::ostream& operator<<(std::ostream& out, const Config& config);

void tempConfigServer(std::vector<Server>& servers, ServerManager& serverManager); //before getting parsed value i ll init server with this func

#endif