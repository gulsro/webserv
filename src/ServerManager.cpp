#include "Webserv.hpp"

ServerManager::ServerManager()
{
    std::cout << "ServerManager constructor is called" << std::endl;
}

ServerManager::~ServerManager()
{
    std::cout << "ServerManager destructor is called" << std::endl;
}

const std::vector<std::unique_ptr<Server>>& ServerManager::getServers() const
{
    return this->servers;
}

// std::unique_ptr<Server>	ServerManager::getServer(std::string host) const
// {
//     auto &servers = this->getServers();

//     for (auto &server: this->servers)
//     {
//         if (std::string::compare(host, server->getHost()) == 0)
//             //return *this;
//     }
// }

//When you call this->servers.push_back(server);
//you are trying to copy the std::unique_ptr.
//However, copying std::unique_ptr is not allowed because
//it would lead to multiple std::unique_ptr instances
//trying to manage the same resource, violating the unique ownership guarantee.
//Using this->servers.push_back(std::move(server));
//explicitly moves the std::unique_ptr into the vector.
//This transfer of ownership ensures that the vector now
//owns the resource, and the original std::unique_ptr becomes empty.
void ServerManager::addServer(std::unique_ptr<Server> server)
{
    //this->servers.push_back(server);
    this->servers.push_back(std::move(server));
}

void ServerManager::startServerManager(ServerManager& serverManager)
{
    auto &servers = serverManager.getServers();
    
    tempConfigServer(serverManager);
    startSockets();
    for (const auto &server: servers)
    {
        std::cout << "server: " << *server << std::endl;
        std::cout << "server: " << server->getServerFd() << std::endl;
    }
}

void ServerManager::startSockets()
{
    //make a loop here, start socket for each server in servers;
    //assign pollfd for each, then push it to pollfds array
    auto &servers = this->getServers();

    for (auto &server: servers)
    {
        server->createSocket();
        server->setSocketAddr();
        server->setSocketOption();
        server->bindSocket();
        server->listenSocket();
        assignPollFdForServer(server->getServerFd(), POLLIN);
    }
}

// struct pollfd {
//     int fd; /* descriptor to check */
//     short events; /* events of interest on fd */
//     short revents; /* events that occurred on fd */
// };

void ServerManager::assignPollFdForServer(int fd, int events)
{
    struct pollfd serverPollFd;

    serverPollFd.fd = fd;
    serverPollFd.events = events;
    pollfds.push_back(serverPollFd);
}

void ServerManager::startPoll()
{
    std::vector<struct pollfd> pollfds;

    //std::unique_ptr<Server>	getServer(std::string host) const; 
}

std::ostream& operator<<(std::ostream& out, const ServerManager& serverManager)
{
    (void)serverManager;
    return out;
}