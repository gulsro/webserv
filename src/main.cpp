#include "Webserv.hpp"

int main(int ac, char **av)
{
    HttpResponse	Response;

    if (ac > 3){
        std::cout << "too many argument" << std::endl;
        return 1;
    }
    else{
        try{
            Config config;
            // Declare a unique_ptr without initializing
            //std::unique_ptr<ServerManager> serverManager;
            //ServerManager serverManager(config);
            
            // Later, assign a dynamically allocated object to it
            // The std::make_unique function creates a new instance of ServerPool on the heap
            // and returns a std::unique_ptr<ServerPool> that owns this instance
            //serverManager = std::make_unique<ServerManager>(&config);
            if (ac == 2)
                config.setConfigFile(av[1]);
            config.parseConfig();
            ServerManager serverManager(config);
            serverManager.startServerManager(serverManager); 
            serverManager.startPoll();
            
            // if (Request.readRequest(fd) == true)
            // {
            //     Request.setReqServer(config.getServerList());
            //     Request.checkLocations();
            //     Response.setRequest(&Request);
            //     Response.checkMethod();
            //     std::cout << Response.getContent() << std::endl;
            // }
           
        }
        catch (const std::exception& e){
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}