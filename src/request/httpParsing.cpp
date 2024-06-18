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
			// std::cerr << "Disconnection with " << fd << std::endl;
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

bool	HttpRequest::parseHttpRequest(const std::string &rawRequest)
{
	std::istringstream	stream(rawRequest);
	std::vector<std::string>	lines = split(rawRequest, '\n');

	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] parseHttpRequest" << DEFAULT << std::endl;
	#endif
	if (lines.empty())
		std::cerr << "Invalid HTTP request format." << std::endl;
	if (!parseRequestLine(lines[0]))
	{
		std::cerr << "Invalid HTTP request line." << std::endl;
		return false;
	}
	//parse headers
	for (size_t i = 1; i < lines.size(); ++i)
	{
		if (lines[i].empty())
			break ; // end of headers
		if (!parseHeader(lines[i]))
			return false;
	}
	setRequestedPort();
	setContentType();
	#ifdef FUNC
		auto it = headers.begin();
		std::cout << PURPLE << "________HEADERS________" << std::endl;
		while (it != headers.end())
		{
			std::cout << PURPLE << "[ " << it->first << " ]" << std::endl;
			std::cout << PURPLE << it->second.key << " : " << it->second.value << DEFAULT << std::endl;
			++it; 
		}
		std::cout << GREEN << "requestedPort : " << this->requestedPort << std::endl;
		std::cout << GREEN << "contentType : " << this->contentType << std::endl;
	#endif
	// parse body
	if (this->headers.count("Content-Length"))
	{
		this->setContentLength(std::stoi(headers.at("Content-Length").value));
		// size_t	bodyStart = rawRequest.find("\r\n\r\n") + 4;
		size_t	bodyStart = rawRequest.find("\n\n") + 2;
		if (bodyStart + this->contentLength > rawRequest.size())
		{
			std::cerr << "Incomplete HTTP request body." << std::endl;
			return false;
		}
		this->body = rawRequest.substr(bodyStart, this->contentLength);
		#ifdef FUNC
			std::cout << YELLOW << "[FUNCTION] parsing body" << DEFAULT << std::endl;
			std::cout << PURPLE << "contentLength	: " << contentLength << DEFAULT << std::endl;
			std::cout << PURPLE << "body	: " << this->body << DEFAULT << std::endl;
		#endif	
	}
	checkContentType();
	return true;
}