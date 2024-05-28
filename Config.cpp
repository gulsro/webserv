#include "Config.hpp"

void initialize_server_info(const Server& vServer)
{
    vServer.port = 8080;
    vServer.host = "0.0.0.0";
    vServer.serverName = "";
    vServer.root = "";
    vServer.maxBodySize = 500000;
    vServer.locationList = std::vector<std::unique_ptr<Location>>();
}




    int port;
    int serverFd;
    std::string serverName;
    std::string root;
    std::string index;

    unsigned long maxBodySize;
        std::vector<Location> locationList;
