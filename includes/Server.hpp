
#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"

class ServerManager;
class Server;
class Location;
class Client;
class ErrorCodeException;

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
    unsigned long long maxBodySize; // in bytes
    int nbLocation;
    bool    autoindex;
    std::map<std::string, int> methods;
    std::map<int, std::string> errorPage;
    
    std::vector<Location*> locationList;
    std::vector<std::string> locationCont;
    struct sockaddr_in serverAddr;
    setFunc func[7] {&Server::setPort, &Server::setRoot, &Server::setIndex, &Server::setMaxBodySize, &Server::setAutoindex, &Server::setMethods, &Server::setErrorPage};

public:
    friend class ErrorCodeException;
    Server();
    ~Server();
    std::vector<int> connectedClientFds;
    std::vector<Client*> clientList;

    Server(const Server& s);
    Server& operator=(const Server s);

//**********GETTERS*********************
    int                 getPort() const { return port; }
    std::string         getHost() const { return host; }
    std::string         getRoot() const { return root; }
    std::string         getIndex() const { return index; }
    int                 getServerFd() const { return serverFd; }
    unsigned long long  getMaxBodySize() const  { return maxBodySize; }
    std::vector<Location*> getLocationList() const  { return locationList; }
    bool                getAutoindex() const { return autoindex; }
    std::map<std::string, int> getMethods() const  { return methods; }
    std::map<int, std::string> getErrorPage() const { return errorPage; }
    struct sockaddr_in* getSocketAddr() const;

    
//**********SETTERS*********************

    void setPort(std::string& cont, int key);
    void setHost(std::string& cont);
    void setRoot(std::string& cont, int key);
    void setIndex(std::string& cont, int key);
    void setMaxBodySize(std::string& cont, int key);
    void setAutoindex(std::string&cont, int key);
    void setMethods(std::string& cont, int key);
    void setErrorPage(std::string& cont, int key);


//********PARSING************************
    void splitLocation(std::string cont);
    std::size_t skipLocationPath(std::string cont, std::size_t found);
    void setServerVar(std::stringstream& iss);
    void initLocation();

//*******SOCKET***************************
    void createSocket();
    void setSocketAddr();
    void setSocketOption();
    void bindSocket();
    void listenSocket();
    int acceptConnection();

//not sure if this is necessary anymore
 //   bool matchesHostAndPort(const std::string &host, int port) const;
    
    // param of the func might be changed as Client obj later.
//    void removeClientFd(int clientFd); //THIS FUNC REPLACED WITH A TEMPLATE FUNC

//*********PRINT**************************
    void printConnectedClientFds() const;

};


std::ostream& operator<<(std::ostream& out, const Server& server);

#endif