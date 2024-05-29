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

class serverManager
{
    private:
        std::vector<Server> servers;
        std::vector<pollfd> pollfds;
    public:
        serverManager();
        ~serverManager();

        std::unique_ptr<serverManager> configServerManager(); // init vServers in a loop
        
        const std::vector<Server*>&	getServers() const;
	    Server*	getServer(std::string host) const; 
        

};
    
#endif