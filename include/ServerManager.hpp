#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

// struct pollfd {
//   int     fd;       /* descriptor to check */
//   short   events;   /* events of interest on fd */
//   short   revents;  /* events that occurred on fd */
// };

// POLLIN -- Data other than high-priority data can be read
// POLLOUT -- Normal data can be written

// serverManager manages multiple Server instances, each representing a virtual host.

class ServerManager
{
    private:
        std::vector<Server> servers;
        //std::vector<pollfd> pollfds;
    public:
        ServerManager();
        ~ServerManager();

        const std::vector<Server*>&	getServers() const;
	    std::unique_ptr<Server>	getServer(std::string host) const; 
        
        void addServer(std::unique_ptr<Server> server);        
        void startServerManager(ServerManager&); // init vServers in a loop
};
    
std::ostream& operator<<(std::ostream& out, const ServerManager& serverManager);

#endif