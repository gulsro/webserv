#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	#ifdef DEBUG
		std::cout << GREY << "HttpResponse : Default constructor called" << DEFAULT << std::endl; 
	#endif
}

HttpResponse::HttpResponse(const HttpResponse &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HttpResponse : Copy constructor called" << DEFAULT << std::endl; 
	#endif
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HttpResponse : Copy assigment operator called" << DEFAULT << std::endl;
	#endif
}

HttpResponse::~HttpResponse()
{
	#ifdef DEBUG
		std::cout << GREY << "HttpResponse : Destructor called" << DEFAULT << std::endl; 
	#endif
}