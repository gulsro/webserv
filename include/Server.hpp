
#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"

class ServerManager;
class Server;
class Location;

typedef void (Server::*setFunc) (std::string&, int);


// The server will use the Host header from incoming HTTP requests
// to determine which virtual host configuration to apply.
// When trying to determine which server block to send a request to, Nginx will first try to decide based on 
// the specificity of the listen directive using the following rules:
//     -Nginx translates all “incomplete” listen directives by substituting missing values with their default values
//     so that each block can be evaluated by its IP address and port.

// config parser gives values to port, serverName, etc.

class Server
{
private:
    int port;
    int serverFd;
    std::string host; //address
    std::string root;
    std::string index;
    unsigned long maxBodySize; // in bytes
    int nbLocation;
    
    std::vector<Location*> locationList;
    std::vector<std::string> locationCont;
    struct sockaddr_in serverAddr;
    setFunc func[4] {&Server::setPort, &Server::setRoot, &Server::setIndex, &Server::setMaxBodySize};

public:
    Server();
    ~Server();
    std::vector<int> connectedClientFds;

    Server(const Server& s);
    Server& operator=(const Server s);

//**********GETTERS*********************
    int getPort() const;
    std::string getHost() const;
    std::string getRoot() const;
    std::string getIndex() const;
    int getServerFd() const;
    unsigned long getMaxBodySize() const;
    std::vector<Location*> getLocationList() const;
    struct sockaddr_in* getSocketAddr() const;
    //std::vector<Location> getLocationList() const;

    
//**********SETTERS*********************

    void setPort(std::string& cont, int key);
    void setHost(std::string& cont);
    // void setServerFd(int fd);
    void setRoot(std::string& cont, int key);
    void setIndex(std::string& cont, int key);
    void setMaxBodySize(std::string& cont, int key);

    void splitLocation(std::string cont);
    std::size_t skipLocationPath(std::string cont, std::size_t found);
    void setServerVar(std::stringstream& iss);
    void initLocation(std::string serverCont);

    // void setPort(int port);
    // void setServerFd(int fd);
    // void setServerName(std::string serverName);
    // void setRoot(std::string root);
    // void setIndex(std::string index);
    // void setHost(std::string host);
    // void setMaxBodySize(unsigned long maxBodySize);


    void createSocket();
    void setSocketAddr();
    void setSocketOption();
    void bindSocket();
    void listenSocket();
    int acceptConnection();

    bool matchesHostAndPort(const std::string &host, int port) const;

    void printConnectedClientFds() const;

};


std::ostream& operator<<(std::ostream& out, const Server& server);

#endif