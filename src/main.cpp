#include "utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Webserv.hpp"
#include <fcntl.h>
#include <unistd.h>

int main(int ac, char **av)
{
    HttpResponse	Response;
	HttpRequest		Request;

    if (ac > 3){
        std::cout << "too many argument" << std::endl;
        return 1;
    }
    else{
        try{
            Config config;

            // if (ac == 2)
			if (ac == 3)
                config.setConfigFile(av[1]);
            config.parseConfig();  
            std::string filename = av[2];
            std::fstream   file;

            int fd = open(filename.c_str(), O_RDONLY);
            if (fd)
            {
                if (Request.readRequest(fd) == true)
                {
                    Request.setReqServer(config.getServerList());
                    Response.setRequest(&Request);
                    Response.checkMethod();
                    std::cout << Response.getContent() << std::endl;
                }
            }
            else
                std::cerr << "Error opening file: " << filename << std::endl;

            file.close();
        }
        catch (const std::exception& e){
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}