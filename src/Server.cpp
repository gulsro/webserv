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

void createSocket()
{
    if (this->serverFd = socket(AF_INET, SOCK_STREAM, 0) < 0)
        throw std::runtime_error("Error: socket()");
}

void Server::setSocketAddr()
{
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void setSocketOption()
{    optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval,
                        sizeof(optval)) == -1)
        throw std::runtime_error("Error: setsockopt()");
}

void bindSocket()
{

}

void listenSocket();
int acceptConnection();