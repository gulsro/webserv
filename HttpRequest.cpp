#include "HttpRequest.hpp"
#include "utils.hpp"
#include <cstring>

HttpRequest::HttpRequest()
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Default constructor called" << DEFAULT << std::endl; 
	#endif

    // this->location = Location->getRoot() + Location->getPath();
    // this->contentLength = 0;
}

HttpRequest::HttpRequest(const HttpRequest &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Copy constructor called" << DEFAULT << std::endl; 
	#endif
	*this = other;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Copy assigment operator called" << DEFAULT << std::endl;
	#endif
	if (this != &other)
	{
		this->method = other.method;
		this->uri = other.uri;
		this->version = other.version;
		this->headers = other.headers;
		this->body = other.body;
	}
	return *this;
}

HttpRequest::~HttpRequest()
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Destructor called" << DEFAULT << std::endl; 
	#endif
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
	setRequestedport();
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
        // if (this->headers.find("Content-Type") != this->headers.end() && this->headers.at("Content-Type").value == "multipart/form-data")
        //     handlePostContents();
		#ifdef FUNC
			std::cout << YELLOW << "[FUNCTION] parsing body" << DEFAULT << std::endl;
			std::cout << PURPLE << "contentLength	: " << contentLength << DEFAULT << std::endl;
			std::cout << PURPLE << "body	: " << this->body << DEFAULT << std::endl;
		#endif	
	}
	return true;
}

// bool	HttpRequest::hasBoundary()
// {
// 	std::string	line;
// 	std::istringstream	iss(this->body);

// 	while (std::getline(iss, line, '\n'))
// 	{
// 		size_t	pos = line.find("boundary=");
// 		if (pos != std::string::npos)
// 		{
// 			std::string boundary = line.substr(pos + 9);
// 			postStruct.boundaryBegin = "--" + boundary;
// 			postStruct.boundaryEnd = "--" + boundary + "--";

// 			return true;
// 		}
// 	}
// 	return false;
// }

// void	HttpRequest::findFilename()
// {
// 	std::string line;
// 	std::istringstream iss(this->body);

// 	while (std::getline(iss, line, '\n'))
// 	{
// 		size_t pos = line.find("filename=");
// 		if (pos != std::string::npos)
// 			postStruct.filename = line.substr(pos + 9);
// 	}
// }


bool	isInvalidChar(const	char c)
{
	std::string	specialChar = "._~:/?#[]@!$&'()*+,;=%";

	for (size_t i = 0; i < specialChar.size(); i++)
	{
		if (c != specialChar[i])
			return (false);
	}
	return (true); 
}

// void	HttpRequest::checkUriValid();
// {
// 	if (this->uri.size() > 2048)
// 	{
// 		createResponse(STATUS_URI_TOO_LONG);
// 	}
// 	for (size_t i = 0; i < this->uri.size(); i++)
// 	{
// 		if (!isdigit(this->uri) && !isalpha(this->uri[i]) && !isInvalidChar(this->uri[i]))
// 		{
// 			createResponse(STATUS_BAD_REQUEST);
// 		}
// 	}
// }

bool	HttpRequest::parseRequestLine(const std::string &line)
{
	std::istringstream stream(line);
	std::string rawVersion;

	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] parseRequestLine" << DEFAULT << std::endl;
	#endif

	stream >> this->method >> this->uri >> rawVersion;
	std::istringstream	iss(rawVersion);
	std::getline(iss, this->version, '\r');
	#ifdef FUNC
		std::cout << PURPLE << "Method	:	" << this->method << DEFAULT<< std::endl;
		std::cout << PURPLE << "URI	:	" << this->uri << DEFAULT<< std::endl;
		std::cout << PURPLE << "Version	:	" << this->version << DEFAULT<< std::endl;
	#endif
	if ((this->method != "GET") && (this->method != "POST") && (this->method != "DELETE"))
	{
		// createResponseHelper(STATUS_NOT_IMPLEMENTED);
		 std::cerr << this->method << "Not implemented." << std::endl;
		return false;
	}
	return true;
}

bool	HttpRequest::parseHeader(const std::string &line)
{
	std::vector<std::string> keyValue = split(line, ':');
	if (keyValue.size() != 2)
	{
		if (keyValue[0] == "Host")
		{
			std::string hostValue = keyValue[1] + ':' + keyValue[2];
			keyValue[1] = hostValue;
		}
		else
		{
			std::cerr << "Invalid header format" << std::endl;
			return false;
		}
	}
	std::string	key = trim(keyValue[0]);
	std::string value = trim(keyValue[1]);
	headers[key] = HttpHeader{key, value};
	return true;
}

void	HttpRequest::setRequestedport()
{
	if (this->headers.count("Host"))
	{
		std::string	value = headers.at("Host").value;
		this->requestedPort = std::stoi(value.substr(value.size() - 4, value.size()));
	}
	else
		this->requestedPort = 0;
}