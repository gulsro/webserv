#include "Webserv.hpp"

Server::Server()
{
    std::cout << "Server constructor is called" << std::endl;
}

Server::Server(int port,
				std::string host,
				std::vector<std::string> serverNames,
				std::string root,
                std::string index,
				unsigned long maxBodySize
)
        :port(port), host(host), serverNames(serverNames),
        root(root), index(index), maxBodySize(maxBodySize)
{
    std::cout << "Server constructor is called" << std::endl;
}

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

struct sockaddr_in* Server::getSocketAddr() const
{
    return const_cast<sockaddr_in*>(&serverAddr);
}

//std::vector<std::string> getServerNames() const;

// std::vector<int> Server::getConnectedClientFds() const
// {
//     return connectedClientFds;
// }

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

void Server::createSocket()
{
    this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverFd == -1)
        throw std::runtime_error("Error: socket()");
}

void Server::setSocketAddr()
{
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void Server::setSocketOption()
{
    //This allows me to reuse a socket address
    //even if it's still in the TIME_WAIT state from a previous connection.
    //this is helpful when you quickly
    //restart a server and want to bind to the same port without waiting
    int optval = 1;
    if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &optval,
                        sizeof(optval)) == -1)
    {
        close(this->serverFd);
        throw std::runtime_error("Error: setsockopt()");
    }
}

void Server::bindSocket()
{
    if (bind(this->serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        close(this->serverFd);
        throw std::runtime_error("Error: bind()");
    }
}

void Server::listenSocket()
{
    if (listen(this->serverFd, 10) == -1)
    {
        close(this->serverFd);
        throw std::runtime_error("Error: listen()");
    }
}

void Server::printConnectedClientFds() const
{
    for (auto &clientFd: this->connectedClientFds)
    {
        std::cout << "Connected clientFd: " << clientFd << std::endl;
        std::cout << "KAKAM" << std::endl;
    }
}

