#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"

class Location;
class Server;
typedef void (Server::*setFunc) (std::string&, int);

class Server
{
private:
    int port;
    int serverFd;
    std::string host; //address
    std::string root;
    std::string index;
    unsigned long maxBodySize;
    
    std::vector<Location*> locationList;
    // struct sockaddr_in serverAddr;

public:
    Server();
    ~Server();
    Server(const Server& s);
    Server& operator=(const Server s);

    // Server(int port,
	// 			std::string host,
	// 			std::string serverName,
	// 			std::string root,
    //             std::string index,
	// 			unsigned long maxBodySize);

    int getPort() const;
    std::string getHost() const;
    std::string getRoot() const;
    std::string getIndex() const;
    unsigned long getMaxBodySize() const;
    std::vector<Location> getLocationList() const;
    int getServerFd() const;
    struct sockaddr_in* getSocketAddr() const;
    //std::vector<Location> getLocationList() const;
    bool matchesHostAndPort(const std::string &host, int port) const;
    
    void setPort(std::string& cont, int key);
    void setHost(std::string& cont);
    // void setServerFd(int fd);
    void setRoot(std::string& cont, int key);
    void setIndex(std::string& cont, int key);
    void setMaxBodySize(std::string& cont, int key);
    setFunc func[4] {&Server::setPort, &Server::setRoot, &Server::setIndex, &Server::setMaxBodySize};

    void setServerVar(std::stringstream& iss);

    // void setPort(int port);
    // void setServerFd(int fd);
    // void setServerName(std::string serverName);
    // void setRoot(std::string root);
    // void setIndex(std::string index);
    // void setHost(std::string host);
    // void setMaxBodySize(unsigned long maxBodySize);


    // void createSocket();
    // void setSocketAddr();
    // void setSocketOption();
    // void bindSocket();
    // void listenSocket();
    // int acceptConnection();
};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif