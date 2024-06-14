
#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"

//class Location;
class ServerManager;

// The server will use the Host header from incoming HTTP requests
// to determine which virtual host configuration to apply.
// When trying to determine which server block to send a request to, Nginx will first try to decide based on 
// the specificity of the listen directive using the following rules:
//     -Nginx translates all “incomplete” listen directives by substituting missing values with their default values
//     so that each block can be evaluated by its IP address and port.


//SUBSERVER???
//struct addrinfo

// config parser gives values to port, serverName, etc.

class Server
{
private:
    int port;
    int serverFd;
    std::string host; //address
    std::vector<std::string> serverNames;
    std::string root;
    std::string index;
    unsigned long maxBodySize;
    //ServerManager& serverManager;
    
   // struct pollfd pollfd;
  //  std::vector<Location> locationList;
    struct sockaddr_in serverAddr;

public:
    Server();
    ~Server();
    Server(int port,
				std::string host,
				std::vector<std::string> serverNames,
				std::string root,
                std::string index,
				unsigned long maxBodySize);
			//	std::vector<std::unique_ptr<Location>> locations,
    //or alternative to member default constructor belove,
    //we could use below;
    //Server(Config& a);

    std::vector<int> connectedClientFds;
 
 
    //***********GETTERSSS************
    int getPort() const;
    std::string getHost() const;
    std::string getRoot() const;
    std::string getIndex() const;
    std::vector<std::string> getServerNames() const;
    int getServerFd() const;
    struct sockaddr_in* getSocketAddr() const;
    //std::vector<Location> getLocationList() const;
    //std::vector<int> getConnectedClientFds() const;
    bool matchesHostAndPort(const std::string &host, int port) const;


    //*************SETTERS***********
    void setPort(int port);
    //void setServerFd(int fd);
    void setServerName(std::string serverName);
    void setRoot(std::string root);
    void setIndex(std::string index);
    void setHost(std::string host);
    void setMaxBodySize(unsigned long maxBodySize);

    //*********SOCKET OPERATIONS******
    void createSocket();
    void setSocketAddr();
    void setSocketOption();
    void bindSocket();
    void listenSocket();

    //*******PRINT**********
    void printConnectedClientFds() const;

};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif