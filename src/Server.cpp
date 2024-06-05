#include "../include/Server.hpp"

Server::Server() : port(0), root(""), index(""), maxBodySize(0)
{
    std::cout << "Server constructor is called" << std::endl;
}

// Server::Server(int port,
// 				std::string host,
// 				std::vector<std::string> serverNames,
// 				std::string root,
//                 std::string index,
// 				unsigned long maxBodySize
// )
//         : port(port), host(host), serverNames(serverNames),
//         root(root), index(index), maxBodySize(maxBodySize)
// {
//     std::cout << "Server constructor is called" << std::endl;
// }

Server::~Server()
{
    std::cout << "Server destructor is called" << std::endl;
}


Server::Server(const Server& s){
    operator=(s);
}

Server& Server::operator=(const Server s){
    if (this == &s)
        return (*this);
    this->port = s.getPort();
    this->serverFd = s.getServerFd();
    this->host = s.getHost();
    this->root = s.getRoot();
    this->index = s.getIndex();
    this->maxBodySize = s.getMaxBodySize();
    return (*this);
}

//Not necessary because createSock() assigns value for serverFd
// void Server::setServerFd(int fd)
// {
//     this->serverFd = fd;
// }

int Server::getPort() const
{
    return this->port;
}

std::string Server::getHost() const
{
    return this->host;
}

int Server::getServerFd() const
{
    return this->serverFd;
}

std::string Server::getRoot() const
{
    return this->root;
}

std::string Server::getIndex() const
{
    return this->index;
}

unsigned long Server::getMaxBodySize() const
{
    return this->maxBodySize;
}

std::vector<Location> Server::getLocationList() const
{
    return this->locationList;
}
// struct sockaddr_in* Server::getSocketAddr() const
// {
//     return const_cast<sockaddr_in*>(&serverAddr);
// }

//return needs to be an error

void Server::setPort(std::string cont, int key){
    if (getPort())
        return ;
    while(std::isspace(cont[key]))
        key++;
    size_t colon = cont.find(':');
    if (colon != std::string::npos){
        setHost(cont.substr(key, colon - key));
        this->port = std::stoi(cont.substr(colon + 1, cont.find('\n') - colon + 1));
    }
    else {
        this->port = std::stoi(cont.substr(key, cont.find('\n') - key + 1));
    }
    std::cout << "port is " << this->port << std::endl;
}

void Server::setHost(std::string cont){
    if (!getHost().empty())
        return ;
    this->host = cont;
    std::cout << "host is " << this->host << std::endl;
}
// void Server::setServerFd(int fd){

// }

void Server::setRoot(std::string cont, int key){
    if (!getRoot().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->root = cont.substr(key, cont.find('\n') - key);
    std::cout << "root is " << this->root << std::endl;
}

void Server::setIndex(std::string cont, int key){
    if (cont.find("autoindex") != std::string::npos)
        return ;
    if (!getIndex().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->index = cont.substr(key, cont.find('\n') - key);
    std::cout << "index is " << this->index << std::endl;
}


void Server::setMaxBodySize(std::string cont, int key){
    if (getMaxBodySize())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->maxBodySize = std::stoi(cont.substr(key, cont.find('\n') - key));
    std::cout << "maxbodysize is " << this->maxBodySize << std::endl;
}

//std::vector<std::string> getServerNames() const;

std::ostream& operator<<(std::ostream& out, const Server& server)
{
    out << "port: " << server.getPort();
    out << std::endl;
    out << "serverFd: " << server.getServerFd();
    out << std::endl;
    out << "host: " << server.getHost();
    out << std::endl;
    //out << server.serverNames;
    out << "root: " << server.getRoot();
    out << std::endl;

    return out;
}

// void Server::createSocket()
// {
//     this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
//     if (this->serverFd == -1)
//         throw std::runtime_error("Error: socket()");
// }

// void Server::setSocketAddr()
// {
//     std::memset(&serverAddr, 0, sizeof(serverAddr));
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(this->port);
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
// }

// void Server::setSocketOption()
// {

//     int optval = 1;
//     if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &optval,
//                         sizeof(optval)) == -1)
//         throw std::runtime_error("Error: setsockopt()");
// }

// void Server::bindSocket()
// {
//     if (bind(this->serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
//         throw std::runtime_error("Error: bind()");
// }

// void Server::listenSocket()
// {
//     if (listen(this->serverFd, 10) == -1)
//         throw std::runtime_error("Error: listen()");
// }

// int Server::acceptConnection();
