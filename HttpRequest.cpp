#include "HttpRequest.hpp"
#include "utils.hpp"
#include <cstring>

HttpRequest::HttpRequest()
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Default constructor called" << DEFAULT << std::endl; 
	#endif
}

HttpRequest::HttpRequest(const HttpRequest &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Copy constructor called" << DEFAULT << std::endl; 
	#endif
}

HttpRequest &HttpRequest::operator=(const HttpRequest &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HttpRequest : Copy assigment operator called" << DEFAULT << std::endl;
	#endif
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
	// parse body
	if (this->headers.count("Content-Length"))
	{
		int	contentLength = std::stoi(headers.at("Content-Length").value);
		size_t	bodyStart = rawRequest.find("\r\n\r\n") + 4;
		if (bodyStart + contentLength > rawRequest.size())
		{
			std::cerr << "Incomplete HTTP request body." << std::endl;
			return false;
		}
		this->body = rawRequest.substr(bodyStart, contentLength);
	}
	return true;
}

bool	HttpRequest::parseRequestLine(const std::string &line)
{
	std::istringstream stream(line);

	stream >> this->method >> this->uri >> this->version;
	if ((this->method != "GET") && (this->method != "POST") && (this->method != "DELETE"))
	{
		//code 501
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
		std::cerr << "Invalid header format" << std::endl;
		return false;
	}
	std::string	key = trim(keyValue[0]);
	std::string value = trim(keyValue[1]);
	headers[key] = HttpHeader{key, value};
	
	return true;
}
