#include "utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	HttpResponse	Response;
	HttpRequest		Request;
    Location        Location;
    int fd;

    if (argc != 2)
        return 1;
    fd = open (argv[1], O_RDONLY);
	try{
		if (Request.readRequest(fd) == true)
		{
            Request.setLocation(&Location);
			Response.setRequest(&Request);
			Response.checkMethod();
			std::cout << Response.getContent() << std::endl;
		}
	} catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
    close(fd);
}
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
            config.parseConfig();
            
        }
        catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}