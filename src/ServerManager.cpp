#include "Webserv.hpp"

ServerManager::ServerManager()
{
    std::cout << "ServerManager constructor is called" << std::endl;
}

ServerManager::~ServerManager()
{
    std::cout << "ServerManager destructor is called" << std::endl;
}

void ServerManager::addServer(std::unique_ptr<Server> server)
{
    this->servers.push_back(*server);
}

void ServerManager::startServerManager(ServerManager& serverManager)
{
    std::unique_ptr<std::vector<Server>>servers = std::make_unique<std::vector<Server>>();
    tempConfigServer(*servers, serverManager);

    for (auto &server: *servers)
    {
        std::cout << "server: " << server << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const ServerManager& serverManager)
{
    (void)serverManager;
    return out;
}