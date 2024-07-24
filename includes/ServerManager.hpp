#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"

#define TIMEOUT 5000

// serverManager manages multiple Server instances, each representing a virtual host.
class Server;
class Config;
class Client;

class ServerManager
{
    private:
        std::vector<Server*>   servers;
        std::map<int, Server*> mapServerFd;
        std::map<int, Client*> mapClientFd;
        std::map<int, std::chrono::system_clock::time_point> clientLastActivity;

        // an array of fd's
        // first elem of the pollpds is serverfd, the rest will be client's
        std::vector<struct pollfd> pollfds;
		    bool	isWritingDone;
    
    public:
       // ServerManager();
        ServerManager(const Config& config);
        ~ServerManager();

        void                        checkTimeouts();
        const std::vector<Server*>& getServers() const;
	      Server*                   	getServer(int serverFd) const;
        std::vector<struct pollfd>& getPollfds();


        void  addServer(Server* server);        
        void  startServerManager(ServerManager &); // init vServers in a loop
        void  startSockets();
        int   setNonBlocking(int fd);
        void  addFdToPollFds(int fd, int events);
        void  rmFdFromPollfd(int fd);
        void  startPoll();
        int   handleIncoming(int fd);

        void  acceptClient(int serverFd, Server& server);
        void  printServers() const;
        void  printPollFds() const;

        bool  isFdInMap(int fd, std::map<int, Server*>& mapServerFd);
    		void	sendResponse(int clientFd);


        bool	readRequest(Client *currClient);
        bool	isPipeFd(int fd);
        int		getClientFdOfPipe(int pipeFd);
        bool  isFdInPollfds(int targetFd);
};
    
bool isFdConnected(int fd, std::vector<int>& connectedClientFds);
std::ostream& operator<<(std::ostream& out, ServerManager& serverManager);

template <typename T>
void deleteFromVector(std::vector<T>& vec, const T& itemToDelete)
{
  auto it = std::find(vec.begin(), vec.end(), itemToDelete);
  if (it != vec.end()) {
    vec.erase(it);
  }
}

#endif