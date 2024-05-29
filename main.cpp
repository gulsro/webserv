#include "utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    HttpRequest Request;
    int fd;

    if (argc != 2)
        return 1;
    fd = open (argv[1], O_RDONLY);
    if (Request.readRequest(fd) == true)
        std::cout << "Done" << std::endl;
    close(fd);
    return 0;
}