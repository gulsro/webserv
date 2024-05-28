#include "../include/Webserv.hpp"

int main(int ac, char **av)
{
    if (ac > 2){
        std::cout << "too many argument" << std::endl;
        return 1;
    }
    else{
        try{
            Config config;
            if (ac == 2)
                config.setConfigFile(av[1]);
            config.checkConfig();
            
        }
        catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            return 1;
        }
    }
    
    
}