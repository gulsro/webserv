#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <sys/types.h> // recv()
#include <sys/socket.h> // recv()
#include <cstring> // strerror()
#include <unistd.h>

#define BUFFER_SIZE 4096 // common page size


bool	HttpRequest::isReadingRequestFinished(std::string rawRequest)
{
	size_t	headerEnd = rawRequest.find("\r\n\r\n");

	if (headerEnd == std::string::npos)
		return false;
	if (this->contentLength) // post method
	{
		if (rawRequest.size() < (headerEnd + 4) + this->contentLength)
			return false;
		else
			return true;
	}
	else
		return true;
}

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
		// size_t	byteRead = recv(fd, buffer, BUFFER_SIZE, 0);
		long long byteRead = read(fd, buffer, BUFFER_SIZE);
        if (byteRead == 0)
		{
			close(fd);
			std::cout << "Disconnection with " << fd << std::endl;
		}
		else if (byteRead == -1)
		{
			close(fd);
			//delete client object
			throw ErrorCodeException(STATUS_BAD_REQUEST);
		}
		else
		{
			std::string tmp = buffer;
			size_t pos;
			if (tmp.find("Content-Length: ") != std::string::npos)
			{
				pos = tmp.find("Content-Length: ");
				this->contentLength = stoll(tmp.substr(pos + 16));
			}
			// if (tmp.find("Transfer-Encoding: chunked") != std::string::npos)
			// {
			// 	pos = tmp.find("application/x-www-form-urlendcoded");
			// 	this->contentLength = stoll(tmp.substr(pos + 37), 0, 16);
			// }
			// Append received data to the stream
			stream.write(buffer, byteRead);
		}
		// Check for complete request
		std::string rawRequest = stream.str();
		if (isReadingRequestFinished(rawRequest) == false)
			return false;
		else
			return parseHttpRequest(rawRequest);
}

std::vector<std::string>	splitHeaderByLine(const std::string &rawRequest)
{
	std::vector<std::string>	tokens;
	std::string					line;
	std::istringstream			iss(rawRequest);

	while (std::getline(iss, line))
	{
		size_t	pos	= line.find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			std::string str = line.substr(0, pos);
			tokens.push_back(str);
		}
	}
	return	tokens;
}

bool	HttpRequest::parseHttpRequest(const std::string &rawRequest)
{
	std::istringstream	stream(rawRequest);
	std::vector<std::string>	lines = splitHeaderByLine(rawRequest);

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
		size_t	bodyStart = rawRequest.find("\r\n\r\n") + 4;
		// size_t	bodyStart = rawRequest.find("\n\n") + 2;
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

void	HttpRequest::checkAllowedMethods()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] checkAllowedMethods" << DEFAULT << std::endl;
	#endif
	std::map<std::string, int> methods = this->ReqLocation->getMethods();
	std::string	requestedMethod = this->getMethod();

	if ((methods["POST"] == 0 && methods["DELETE"] == 0)
		|| (methods["POST"] == 0 && requestedMethod == "POST")
		|| (methods["DELETE"] == 0 && requestedMethod == "DELETE"))
	{
		throw ErrorCodeException(STATUS_NOT_ALLOWED);
	}
}

void	HttpRequest::checkLocations()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] checkLocation" << DEFAULT << std::endl;
	#endif
	setReqLocation(this->ReqServer->getLocationList());
	// GET is always allowed depending on our own config file
	if (this->getMethod() != "GET" && this->ReqLocation != nullptr)
		checkAllowedMethods();
}