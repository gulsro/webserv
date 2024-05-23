#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPResponse : Default constructor called" << DEFAULT << std::endl; 
	#endif
}

HTTPResponse::HTTPResponse(const HTTPResponse &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPResponse : Copy constructor called" << DEFAULT << std::endl; 
		std::cout << GREY << "I am a CONFLICT" << DEFAULT << std::endl; 
	#endif
}

HTTPResponse &HTTPResponse::operator=(const HTTPResponse &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPResponse : Copy assigment operator called" << DEFAULT << std::endl;
	#endif
}

HTTPResponse::~HTTPResponse()
{
	#ifdef DEBUG
		std::cout << GREY << "HTTPResponse : Destructor called" << DEFAULT << std::endl; 
	#endif
}