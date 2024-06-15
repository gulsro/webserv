#include "HttpRequest.hpp"
#include "Location.hpp"
#include "utils.hpp"
#include <cstring>

#define MAX_BODY 5000

HttpRequest::HttpRequest()
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Default constructor called" << DEFAULT << std::endl; 
	#endif
	this->version = "";
	this->contentLength = 0;
	this->contentType = "";
	this->requestedPort = 0;
	this->location = "";

    // this->location = ReqLocation->getRoot() + ReqLocation->getPath();
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

std::vector<std::string> HttpRequest::splitByBoundary()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] splitByBoundary" << DEFAULT << std::endl;
	#endif
	std::vector<std::string> parts;
	size_t begin = 0;
	size_t end = this->body.find(boundaryEnd);

	while (begin < this->body.size() && begin != end)
	{
		size_t pos = this->body.find(this->boundaryBegin, begin);
		if (pos != std::string::npos)
		{
			pos += this->boundaryBegin.size();
			size_t next = this->body.find(this->boundaryBegin, pos);
			if (next != std::string::npos)
			{
				parts.push_back(this->body.substr(pos, (next - pos) - 1));
				begin = next;
			}
			else
			{
				parts.push_back(this->body.substr(pos, (end - pos) - 1));
				begin = pos;
			}
		}
		else
			begin = pos;
	}
	return parts;
}

void	HttpRequest::makeKeyValuePair(int n, const std::string str)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] makeKeyValuePair" << DEFAULT << std::endl;
	#endif
	(void)n;
	std::vector<std::string> splittedStr = split(str, ';');
	// i = 1 : skipping Content-Disposition value. (e.g. form-data;)
	for (size_t i = 1; i < splittedStr.size(); i++)
	{
		std::vector<std::string> keyValue = splitForKeyValue(splittedStr[i], '=');
		std::string key = trim(keyValue[0], ' ');
		std::string value = trim(keyValue[1], '"');
		this->parts[n].pairs.push_back(std::make_pair(key, value));
		if (key == "filename")
			this->parts[n].partFilename = value;
	}
}

void	HttpRequest::checkContentType()
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] checkContentType" << DEFAULT << std::endl;
	#endif
	if (this->contentType == "multipart/form-data")
		handleMultiPartForm();
	// else if (this->contentType == "application/x-www-form-urlencoded")
	// 	handleEncoding();
}

void	HttpRequest::checkRequestValid()
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] checkRequestValid" << DEFAULT << std::endl;
	#endif
	if (this->method == "POST" && this->headers.at("Content-Type").value.empty())
		throw ErrorCodeException(STATUS_NOT_IMPLEMENTED);
	if (this->body.size() > MAX_BODY)
		throw ErrorCodeException(STATUS_TOO_LARGE);
}

void	HttpRequest::setLocation(Location *ReqLocation)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] setLocation" << DEFAULT << std::endl;
	#endif
	// std::vector<Location*> locations;
	std::string	path = ReqLocation->getPath();
	std::string	root = ReqLocation->getRoot();
	std::string	uri = this->getURI();

	// locations = Server->getServer()->getLocation();
	// path = location[i]->getPath();

	if (uri == path)
	{
		this->location = root + path;
	}
}

void	HttpRequest::setBoundary()
{
	std::string	str = headers.at("Content-Type").value;
	size_t pos = str.find("boundary=");
	
	if (pos != std::string::npos)
	{
		std::string boundary = str.substr(pos + 9);
		this->boundaryBegin = "--" + boundary + '\n';
		this->boundaryEnd = "--" + boundary + "--";
	}
}

void	HttpRequest::checkUriValid()
{
	if (this->uri.size() > 2048)
	{
		throw ErrorCodeException(STATUS_URI_TOO_LONG);
	}
	for (size_t i = 0; i < this->uri.size(); i++)
	{
		if (!isdigit(this->uri[i]) && !isalpha(this->uri[i]) && !isInvalidChar(this->uri[i]))
		{
			throw ErrorCodeException(STATUS_BAD_REQUEST);
		}
	}
}

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
		throw ErrorCodeException(STATUS_NOT_ALLOWED);
		// return false;
	}
	// checkUriValid();
	return true;
}

void HttpRequest::setContentType()
{
	if (this->headers.count("Content-Type"))
	{
		std::string value = headers.at("Content-Type").value;

		size_t pos = value.find(';');
		if (pos != std::string::npos)
		{
			this->contentType = value.substr(0, pos);
		}
		else
			this->contentType = value;
	}
	else
		this->contentType = "";
}

bool	HttpRequest::parseHeader(const std::string &line)
{
	std::vector<std::string> keyValue = splitForKeyValue(line, ':');
	if (keyValue.size() != 2)
	{
		std::cerr << "Invalid header format" << std::endl;
		return false;
	}
	std::string	key = trim(keyValue[0], ' ');
	std::string value = trim(keyValue[1], ' ');
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