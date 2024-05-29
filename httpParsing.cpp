#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <sys/types.h> // recv()
#include <sys/socket.h> // recv()
#include <cstring> // strerror()
#include <unistd.h>

#define BUFFER_SIZE 4096 // common page size


/**
 * Non-blocking sockets: When working with non-blocking sockets,
 * MSG_PEEK can be used to check if any data is available before attempting a recv call.
 * This helps avoid blocking operations if no data is present.
*/
bool	HttpRequest::readRequest(int fd)
{
	char				buffer[BUFFER_SIZE];
	std::stringstream 	stream;

    #ifdef FUNC
    std::cout << YELLOW << "[FUNCTION] readRequest" << DEFAULT << std::endl;
	#endif
    
    // Read data in chunks
	while (true)
	{
		// int	byteRead = recv(fd, buffer, BUFFER_SIZE, 0);
		int byteRead = read(fd, buffer, BUFFER_SIZE);
        if (byteRead == 0)
		{
			//remove poll fd;
			std::cerr << "Disconnection with " << fd << std::endl;
			break;
		}
		else if (byteRead == -1)
		{
			std::cerr << "Error reading from socket: " << strerror(errno) << std::endl;
			return false;
		}
		// Append received data to the stream
		stream.write(buffer, byteRead);
		// Check for complete request
		std::string rawRequest = stream.str();
		if (rawRequest.find("\r\n\r\n") != std::string::npos)
			break;

	}
	std::string rawRequest = stream.str();
	stream.str("");

	return parseHttpRequest(rawRequest);
}