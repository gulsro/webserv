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


    for (const auto &server: servers)
    {
        std::cout << "server: " << *server << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const ServerManager& serverManager)
{
    (void)serverManager;
    return out;
}