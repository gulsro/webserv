#include "../include/Server.hpp"

Server::Server()
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

//Not necessary because createSock() assigns value for serverFd
// void Server::setServerFd(int fd)
// {
//     this->serverFd = fd;
// }

int Server::getPort() const
{
    return this->port;
}

std::string Server::getServerNames() const{
    return this->serverNames;
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

std::string Server::getIndex() const{
    return this->index;
}

int Server::getMaxBodySize() const{
    return this->maxBodySize;
}
// struct sockaddr_in* Server::getSocketAddr() const
// {
//     return const_cast<sockaddr_in*>(&serverAddr);
// }

//return needs to be an error

// need to remove followings:: localhost , 127.0.0.2
void Server::setPort(std::string cont, int key){
    if (getPort())
        return ;
    while(std::isspace(cont[key]))
        key++;
    std::cout << "Looking for port: key is " << key << std::endl;
    this->port = std::stoi(cont.substr(key, cont.find('\n') - key + 1));
    std::cout << "port is " << this->port << std::endl;
}

// void Server::setServerFd(int fd){

// }

void Server::setServerNames(std::string cont, int key){
    if (!getServerNames().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->serverNames = cont.substr(key, cont.find('\n') - key);
    std::cout << "server name is " << this->serverNames << std::endl;
}

void Server::setRoot(std::string cont, int key){
    if (!getRoot().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->root = cont.substr(key, cont.find('\n') - key);
    std::cout << "root is " << this->root << std::endl;
}

void Server::setIndex(std::string cont, int key){
    if (!getIndex().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->index = cont.substr(key, cont.find('\n') - key);
    std::cout << "index is " << this->index << std::endl;
}

void Server::setHost(std::string cont, int key){
    if (!getHost().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->host = cont.substr(key, cont.find('\n') - key);
    std::cout << "host is " << this->host << std::endl;
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
