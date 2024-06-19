#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"

// struct pollfd {
//   int     fd;       /* descriptor to check */
//   short   events;   /* events of interest on fd */
//   short   revents;  /* events that occurred on fd */
// };

// POLLIN -- Data other than high-priority data can be read
// POLLOUT -- Normal data can be written

// serverManager manages multiple Server instances, each representing a virtual host.
class Server;
class Config;

class ServerManager
{
    private:
        std::vector<std::unique_ptr<Server>> servers;
        //std::map<int, std::unique_ptr<Server>> mapServerFd;
        std::map<int, Server*> mapServerFd;
        // an array of fd's
        // first elem of the pollpds is serverfd, the rest will be client's
        std::vector<struct pollfd> pollfds;
    
    public:
       // ServerManager();
        ServerManager(const Config& config);
        ~ServerManager();

        const std::vector<std::unique_ptr<Server>>& getServers() const;
	    std::unique_ptr<Server>	getServer(std::string host) const; 
        
        void addServer(std::unique_ptr<Server> server);        
        void startServerManager(ServerManager &); // init vServers in a loop
        void startSockets();
        void addFdToPollFds(int fd, int events);
        void startPoll();

        void acceptClient(int serverFd, Server& server);
        void printServers() const;
        void printPollFds() const;

        bool isFdInMap(int fd, std::map<int, Server*>& mapServerFd);
};
    
bool isFdConnected(int fd, std::vector<int>& connectedClientFds);
std::ostream& operator<<(std::ostream& out, ServerManager& serverManager);

#endif