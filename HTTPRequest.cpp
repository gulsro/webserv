#include "HTTPRequest.hpp"
#include <cstring>

HTTPRequest::HTTPRequest()
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPRequest : Default constructor called" << DEFAULT << std::endl; 
	#endif
}

HTTPRequest::HTTPRequest(const HTTPRequest &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPRequest : Copy constructor called" << DEFAULT << std::endl; 
	#endif
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPRequest : Copy assigment operator called" << DEFAULT << std::endl;
	#endif
}

HTTPRequest::~HTTPRequest()
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPRequest : Destructor called" << DEFAULT << std::endl; 
	#endif
}


bool	HTTPRequest::parseHttpRequest(const std::string &rawRequest)
{
	std::string			line;
	std::istringstream	stream(rawRequest);

	if (!std::getline(stream, line))
		return false;
	if (!parseRequestLine(line))
		return false;

	while (std::getline(stream, line) && line != "\r\n")
	{
		if (line.empty())
			continue;
		if (!parseHeader(line))
			return false;
	}
	if (stream)
	{
		std::ostringstream bodyStream;
		bodyStream << stream.rdbuf();
		this->body = bodyStream.str();
	}

	return true;
}

bool	HTTPRequest::parseRequestLine(const std::string &line)
{
	std::istringstream stream(line);

	stream >> this->method >> this->uri >> this->version;
	if ((this->method != "GET") && (this->method != "POST") && (this->method != "DELETE"))
	{
		//code 501
		std::cerr << this->method << "Not implemented." << std::endl;
		return false;
	}
	do
	{
		size_t pos = line.find("Content-Length");
		if (this->method == "POST" && (pos != std::string::npos))
		{
			// store numbers after Content-length

		}
	} while (strcmp(line, "\r\n"));
	
	return true;
}

bool	parseHeader(const std::string &line)
{
	size_t	colon	= line.find(':');
	if (colon == std::string::npos)
		return false;
	std::string key = line.substr(0, colon);
	std::string	value = line.substr(colon + 1);
}
