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

// void	HttpRequest::findFilename(auto it, std::vector<std::string> strings)
// {
// 	size_t	pos;

// 	pos = (*it).find(" filename=");
// 	parts.bodyFilename = (*it).substr(pos + 11, ((*it).size() - 1));
// 	++it; // move to next line
// 	if (it != data.end())
// 	{
// 		pos = (*it).find("Content-Type: ");
// 		if (pos != std::string::npos)
// 			parts.bodyContentType = (*it).substr(pos + 14, (*it).size());
// 	}
// }

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
				parts.push_back(this->body.substr(pos, next - pos));
				begin = next;
			}
			else
			{
				parts.push_back(this->body.substr(pos, end - pos));
				begin = pos;
			}
		}
		else
			begin = pos;
	}
	return parts;
}

void	HttpRequest::handleMultiPartForm()
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] handleMultiPartForm" << DEFAULT << std::endl;
	#endif
	// std::vector<std::pair<std::string, std::string>> body;
	std::vector<std::string>	splittedBody;
	// std::vector<t_part>			multiData;
	// std::string	key;
	// std::string value;

	setBoundary();
	splittedBody = splitByBoundary();
	for (size_t i = 0; i < splittedBody.size(); ++i)
	{
		std::cout << splittedBody[i] << std::endl;
	}
	// for (auto it = splittedBody.begin(); it != splittedBody.end(); it++)
	// {
	// 	if (*it == this->boundaryBegin)
	// 	{
	// 		it++;
	// 		size_t pos = (*it).find(" filename=");
	// 		if (pos != std::string::npos)
	// 			findFilename(it, splittedBody);

	// 	}
	// }
	

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

void	HttpRequest::setLocation()
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] setLocation" << DEFAULT << std::endl;
	#endif
	// std::vector<Location*> locations;
	std::vector<std::string>	locations;
	size_t					len = 0;
	std::string				path;

	// locations = Server->getServer()->getLocation();
	locations = {"/root", "/html", "/bin"};
	for (size_t i = 0; i < locations.size(); i++)
	{
		// path = location[i]->getPath();
		path = ReqLocation->getPath();
		if (path.size() > this->uri.size())
			continue ;
		size_t	tmp = 0;
		for (size_t j = 0; j < uri.size() && path[j] == uri[j]; j++)
			tmp++;
		if (tmp > len)
		{
			len = tmp;
			this->location = location[i];
		}
	}
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