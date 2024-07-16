#include "HttpRequest.hpp"
#include "Location.hpp"
#include "utils.hpp"
#include <cstring>

HttpRequest::HttpRequest()
	: rawRequest(""), method(""), uri(""), contentLength(0), contentType(""), requestedPort(0), boundaryBegin(""), boundaryEnd(""), isChunked(false), isCgi(false)
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
	// #ifdef STRUCTOR
		std::cout << GREY << "HttpRequest : Destructor called" << DEFAULT << std::endl; 
	// #endif
}

void	HttpRequest::checkContentType()
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] checkContentType" << DEFAULT << std::endl;
	#endif
	if (this->contentType.find("multipart/form-data") != std::string::npos)
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
	unsigned long maxBodySize;
	if (this->ReqLocation != nullptr)
		maxBodySize = this->ReqLocation->getMaxBodySize();
	else
		maxBodySize = this->ReqServer->getMaxBodySize();
	if (this->body.size() > maxBodySize)
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
		if (this->requestedPort == port){
			this->ReqServer = serverList[i];
			return ;
		}
	}
	this->ReqServer = serverList[0]; // Default server
}

std::string returnFileExtension(const std::string path)
{
	std::string fileExtension;
	size_t pos = path.find('.');

	if (pos != std::string::npos)
	{
		fileExtension = path.substr(pos);
	}
	return fileExtension;
}

void	HttpRequest::selectReqLocation(std::vector<Location*> locationList)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] setReqLocation" << DEFAULT << std::endl;
	#endif

	for (size_t i = 0; i < locationList.size(); ++i)
	{
		// if uri contains cgi program extension file name. For us, Python.
		size_t pos = this->uri.find(".py");
		if (pos != std::string::npos && locationList[i]->getPath() == "/*.py")
		{
			std::cout << MAG << "CGI extention is detected" << RES << std::endl;
			std::string fileExtension = returnFileExtension(locationList[i]->getPath());
			// check file extension name is only ".py"
			if (fileExtension == ".py")
			{
				setReqLocation(locationList[i]);
				std::cout << MAG << "CGI is instantiated" << RES << std::endl;
				this->isCgi = true;
				setReqLocation(locationList[i]);
				return ;
			}
		}
		std::string	path =  this->ReqServer->getRoot() + locationList[i]->getPath() + "/";
			std::cout << "______path____" << std::endl;
			std::cout << path << std::endl;
			std::cout << "______uri____" << std::endl;
			std::cout << this->uri << std::endl;
		if (this->uri.find(path) != std::string::npos)
		{
			setReqLocation(locationList[i]);
			return ;
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
		this->boundaryBegin = "--" + boundary + "\r\n";
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

int hexdigiToInt(char hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    else if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    else if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    else
        return -1;
}
std::string decodeUri(const std::string& encoded)
{	
	std::string decoded;
	decoded.reserve(encoded.size());

    for (size_t i = 0; i < encoded.size(); ++i)
	{
        char c = encoded[i];
        if (c == '%') {
            if (i + 2 >= encoded.size())
			{
                // Invalid encoding
                break;
            }
            char hex1 = encoded[i + 1];
            char hex2 = encoded[i + 2];
            if (!isxdigit(hex1) || !isxdigit(hex2))
			{
                // Invalid encoding
                break;
            }
            int value = (hexdigiToInt(hex1) << 4) + hexdigiToInt(hex2);
            decoded += static_cast<char>(value);
            i += 2;
        } else
            decoded += c;
    }

    return decoded;
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
		this->setQueryPairs();
	if (this->uri.find('%') != std::string::npos)
		this->uri = decodeUri(this->uri);
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

		// size_t pos = value.find(';');
		// if (pos != std::string::npos)
		// {
		// 	this->contentType = value.substr(0, pos);
		// }
		// else
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
		throw std::runtime_error("Invalid header format.");
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
