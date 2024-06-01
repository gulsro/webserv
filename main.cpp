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
    if (Request.readRequest(fd) == true)
	{
		Response.setRequest(&Request);
		Response.checkMethod();
		std::cout << Response.getContent() << std::endl;
	}
    close(fd);
    return 0;
}