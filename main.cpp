#include "utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	HttpResponse	Response;
	HttpRequest		Request;
    int fd;

    if (argc != 2)
        return 1;
    fd = open (argv[1], O_RDONLY);
	try{
		if (Request.readRequest(fd) == true)
		{
			Response.setRequest(&Request);
			Response.checkMethod();
			std::cout << Response.getContent() << std::endl;
		}
	} catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
    close(fd);
    return 0;
}