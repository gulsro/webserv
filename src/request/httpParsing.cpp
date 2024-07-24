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

	if (this->contentLength) // post method
	{
		if (rawRequest.size() < (headerEnd + 4) + this->contentLength)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	if (headerEnd == std::string::npos)
	{
		return false;
	}
	else if (this->isChunked == true)
	{
		size_t	chunkedBodyEnd = rawRequest.find("\r\n0\r\n\r\n");
		if (chunkedBodyEnd != std::string::npos)
			return true;
		else
			return false;
	}
	else
		return true;
}

void	HttpRequest::checkRequestSize()
{
	size_t pos;
	std::string tmp = this->getRawRequest();

	if (tmp.find("Content-Length: ") != std::string::npos)
	{
		pos = tmp.find("Content-Length: ");
		this->setContentLength(stoll(tmp.substr(pos + 16)));
	}
	if (tmp.find("Transfer-Encoding: chunked") != std::string::npos)
		this->setIsChunked(true);
}

/**
 * Non-blocking sockets: When working with non-blocking sockets,
 * MSG_PEEK can be used to check if any data is available before attempting a recv call.
 * This helps avoid blocking operations if no data is present.
*/
bool	ServerManager::readRequest(Client *Client)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] readRequest" << DEFAULT << std::endl;
	#endif

	//detect cgi (for the second )
	// check if cgi is running 
	char	buffer[BUFFER_SIZE];
	int		fd = Client->getClientFd();
	ssize_t	byteRead = recv(fd, buffer, BUFFER_SIZE, 0);

	if (byteRead == 0)
	{
		rmFdFromPollfd(fd);
		close(fd);
		throw std::runtime_error("recv()");
	}
	else if (byteRead == -1)
	{
		close(fd);
		throw ErrorCodeException(STATUS_BAD_REQUEST, Client->getRequest()->getReqServer().getErrorPage());
	}
	else
	{
		std::string tmp(buffer, static_cast<size_t>(byteRead));
		if (!Client->getRequest()) // Creat a new HttpRequest class in the Client object
		{
			Client->setRequest(new HttpRequest());
			Client->getRequest()->setRawRequest(tmp);
		}
		else
		{
			// Appending read buffer
			std::string rawRequest = Client->getRequest()->getRawRequest();
			Client->getRequest()->setRawRequest(rawRequest + tmp);
		}
		Client->getRequest()->checkRequestSize();
	}
	// Check for complete request
	std::string rawRequestStr = Client->getRequest()->getRawRequest();
	if (Client->getRequest()->isReadingRequestFinished(rawRequestStr) == false)
	{
		if (byteRead < BUFFER_SIZE)
			return true;
		return false;
	}
	else
	{
		Client->setReadyToFlag(WRITE);
		return Client->getRequest()->parseHttpRequest(rawRequestStr);
	}
}

std::vector<std::string>	splitHeaderByLine(const std::string &rawRequest)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] splitHeaderByLine" << DEFAULT << std::endl;
	#endif

	std::vector<std::string>	tokens;
	std::string					line;
	std::istringstream			iss(rawRequest);

	while (std::getline(iss, line))
	{
		size_t	pos	= line.find('\r');
		size_t	bodyBegin = line.find("\r\n\r\n");

		if (bodyBegin != std::string::npos)
			continue ;
		if (pos != std::string::npos && pos != bodyBegin)
		{
			std::string str = line.substr(0, pos);
			tokens.push_back(str);
		}
	}
	return	tokens;
}

bool	HttpRequest::parseHttpRequest(const std::string &rawRequest)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] parseHttpRequest" << DEFAULT << std::endl;
	#endif
	std::istringstream	stream(rawRequest);
	std::vector<std::string>	lines = splitHeaderByLine(rawRequest);
	// delete client fd?
	if (lines.empty())
		throw std::runtime_error("Invalid HTTP request format.");
	if (!parseRequestLine(lines[0]))
		throw std::runtime_error("Invalid HTTP request line.");
	//parse headers
	for (size_t i = 1; i < lines.size(); ++i)
	{
		if (lines[i].empty())
			break ; // end of headers
		if (!parseHeader(lines[i]))
			return false;
	}
	if (this->method == "POST" && this->contentLength == 0)
		throw ErrorCodeException(STATUS_LENGTH_REQUIRED, this->ReqServer->getErrorPage());
	setRequestedPort();
	setContentType();
	// parse body
	if (this->contentLength > 0 || isChunked == true)
	{
		size_t	bodyStart = rawRequest.find("\r\n\r\n") + 4;
		if (isChunked == true)
			handleChunkedBody(bodyStart, rawRequest);
		if (bodyStart + this->contentLength > rawRequest.size())
			throw std::runtime_error("Incomplete HTTP request body.");
		this->body = rawRequest.substr(bodyStart, this->contentLength);
	}
	checkContentType();
	// checkRequestValid();
	#ifdef FUNC
		printParsedRequest(this);
	#endif	
	return true;
}

void	HttpRequest::checkAllowedMethods(std::string requestedMethod)
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] checkAllowedMethods" << DEFAULT << std::endl;
	#endif
	std::map<std::string, int> methods;
	
	if (this->ReqLocation == nullptr)
	    methods = this->ReqServer->getMethods();
	else
		methods = this->ReqLocation->getMethods();

	if ((methods["POST"] == 0 && methods["DELETE"] == 0)
		|| (methods["POST"] == 0 && requestedMethod == "POST")
		|| (methods["DELETE"] == 0 && requestedMethod == "DELETE"))
	{
		throw ErrorCodeException(STATUS_NOT_ALLOWED, this->ReqServer->getErrorPage());
	}
}

void	HttpRequest::checkLocations()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] checkLocation" << DEFAULT << std::endl;
	#endif
	selectReqLocation(this->ReqServer->getLocationList());
	if (this->getMethod() != "GET" && this->ReqLocation != nullptr)
		checkAllowedMethods(this->getMethod());
}

void	HttpRequest::handleChunkedBody(const size_t bodyStart, const std::string rawRequest)
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] handleChunkedBody" << DEFAULT << std::endl;
	#endif
	std::string		chunkedData = rawRequest.substr(bodyStart);
	size_t 			begin = 0;
	unsigned int	chunkSize = 1;
	std::string 	pureData;
	size_t			pos;

	while (1)
	{
		pos = rawRequest.find("\r\n"); // Finding Hex number chunk size.
		std::stringstream 	ss(chunkedData.substr(begin, pos));
		ss >> std::hex >> chunkSize;
		if (chunkSize == 0)
			break ;
		pureData = chunkedData.substr(pos + 2, chunkSize); // Extract pure data
		this->body.append(pureData);
		begin = (pos + 2) + (pureData.length() + 2); // (hex number + CRLF) + (Data length + CRLF)
		std::string tmp = chunkedData.substr(begin);
		chunkedData = tmp;
		begin = 0; // reset begin
	}
	this->contentLength = pureData.length();
}