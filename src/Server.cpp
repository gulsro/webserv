#include "Webserv.hpp"

Server::Server(ServerManager& serverManager)
    :serverManager(serverManager)
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

Server::Server(int port,
				std::string host,
				std::vector<std::string> serverNames,
				std::string root,
                std::string index,
				unsigned long maxBodySize,
				ServerManager& serverManager)
        :port(port), host(host), serverNames(serverNames),
        root(root), index(index), maxBodySize(maxBodySize),
        serverManager(serverManager)
{
    std::cout << "Server constructor is called" << std::endl;
}


std::ostream& operator<<(std::ostream& out, const Server& server)
{
    out << server.getPort();
    out << server.getServerFd();
    out << server.getHost();
    //out << server.serverNames;
    out << server.getRoot();

    return out;
}