#include "HttpRequest.hpp"
#include "Location.hpp"
#include "utils.hpp"
#include <cstring>

HttpRequest::HttpRequest()
	: rawRequest(""), method(""), uri(""), contentLength(0), contentType(""), requestedPort(0), boundaryBegin(""), boundaryEnd(""), isChunked(false), cgi(nullptr)
{
	#ifdef STRUCTOR
		std::cout << GREY << "HttpRequest : Default constructor called" << DEFAULT << std::endl; 
	#endif
}

HttpRequest::HttpRequest(const HttpRequest &other)
{
	#ifdef STRUCTOR
		std::cout << GREY << "HttpRequest : Copy constructor called" << DEFAULT << std::endl; 
	#endif
	*this = other;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &other)
{
	#ifdef STRUCTOR
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
	#ifdef STRUCTOR
		std::cout << GREY << "HttpRequest : Destructor called" << DEFAULT << std::endl; 
	#endif
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
	if (this->body.size() > this->ReqLocation->getMaxBodySize())
		throw ErrorCodeException(STATUS_TOO_LARGE);
}

void    HttpRequest::setReqServer(std::vector<Server*> serverList)
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] setReqServer" << DEFAULT << std::endl;
	#endif
	for (size_t i = 0; i < serverList.size(); ++i)
	{
		int	port = serverList[i]->getPort();
		if (this->requestedPort == port)
			this->ReqServer = serverList[i];
		else
			continue;
	}
	this->ReqServer = serverList[0]; // Default server
}

void	HttpRequest::setReqLocation(std::vector<Location*> locationList)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] setReqLocation" << DEFAULT << std::endl;
	#endif

	for (size_t i = 0; i < locationList.size(); ++i)
	{
		std::string	path = locationList[i]->getPath();
		if (this->uri == path)
		{
			this->ReqLocation = locationList[i];
			break;
		}
		// if uri contains cgi program extension file name. For us, Python.
		size_t pos = this->uri.find(".py");
		if (pos != std::string::npos && path == "/*.py")
		{
			std::cout << MAG << "CGI extention is detected" << RES << std::endl;
			char	c = this->uri[pos + 3];
			// check file extension name is only ".py"
			if (isdigit(c) == false && isalpha(c) == false && c != '-' && c != '_')
			{
				this->ReqLocation = locationList[i];
				std::cout << MAG << "CGI is instantiated" << RES << std::endl;
				cgi = new Cgi(*this, *(locationList[i]), *ReqServer);
				break;
			}
		}
	}
	this->ReqLocation = nullptr;
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

void	HttpRequest::checkUriValidation()
{
	if (this->uri.length() > 2048)
		throw ErrorCodeException(STATUS_URI_TOO_LONG);
	for (size_t i = 0; i < this->uri.length(); ++i)
	{
		char c = this->uri[i];
		if (std::isdigit(c) == false && std::isalpha(c) == false && isInvalidCharForURI(c) == true)
			throw ErrorCodeException(STATUS_BAD_REQUEST);
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
	checkUriValidation();
	// check query string in URI
	if (this->uri.find('?') != std::string::npos)
	{
		this->setQueryPairs();
	}
	if ((this->method != "GET") && (this->method != "POST") && (this->method != "DELETE"))
	{
		throw ErrorCodeException(STATUS_NOT_ALLOWED);
	}
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
		// std::cout << YELLOW << "size : " << keyValue.size() << DEFAULT << std::endl;
		std::cerr << "Invalid header format" << std::endl;
		return false;
	}
	std::string	key = trim(keyValue[0], ' ');
	std::string value = trim(keyValue[1], ' ');
	headers[key] = HttpHeader{key, value};
	return true;
}

void	HttpRequest::setRequestedPort()
{
	if (this->headers.count("Host"))
	{
		std::string	value = headers.at("Host").value;
		this->requestedPort = std::stoi(value.substr(value.size() - 4, value.size()));
	}
	else
		this->requestedPort = 0;
}

void	HttpRequest::setQueryString(std::string str)
{
	this->queryString = str;
}

void	HttpRequest::setQueryPairs()
{
	size_t		begin;
	std::string	uri = this->getURI();
	std::vector<std::string>	queryPair;

	begin = uri.find('?');
	std::string queryStr = uri.substr(begin + 1, uri.size() - (begin + 1));
	setQueryString(queryStr);
	queryPair = split(queryStr, '&');
	for (size_t i = 0; i < queryPair.size(); ++i)
	{
		std::vector<std::string> keyValue = splitForKeyValue(queryPair[i], '=');
		std::string	key = keyValue[0];
		std::string value = keyValue[1];
		this->queryPairs.push_back(std::make_pair(key, value));
	}
}
