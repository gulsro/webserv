#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

// Manages multiple Server instances, each representing a virtual host.

class serverManager
{
    private:
        std::vector<Server> servers;
        std::vector<pollfd> pollfds;
}