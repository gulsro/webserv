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

std::string	HttpResponse::getStatusMessage()
{
	switch (this->statusCode)
	{
		case 001:
			return ("File Open Failed");
		case 200:
			return ("OK");
		case 201:
			return ("Created");
		case 204:
			return ("No Content");
		case 205:
			return ("Reset Content");
		case 301:
			return ("Moved Permanently");
		case 400:
			return ("Bad Request");
		case 401:
			return ("Unauthorized");
		case 403:
			return ("Forbidden");
		case 404:
			return ("Not Found");
		case 405:
			return ("Method Not Allowed");
		case 408:
			return ("Reguest Timeout");
		case 409:
			return ("Conflict");
		case 410:
			return ("Gone"); // Requsted uri has been intentionally removed or unlikely to return.
		case 411:
			return ("Length Required"); // missing "Content-Length" header
		case 413:
			return ("Content Too Large");
		case 414:
			return ("URI TOO LONG");
		case 500:
			return ("Internal Server Error");
		case 503:
			return ("Sevice Unavailable");
		case 504:
			return ("Gateway Timeout");
		case 505:
			return ("HTTP Version Not Supported");
		default :
			return ("Not Implemented");
	}
}

void	sendRespose(int fd)
{
	std::string content = createResponse();
	int val = send(fd, content.c_str(), content.size(), 0);
	if (val == -1)
	{
		// Sending error occured
		//clean up
	}
	deleteRequest();
}

void	HttpResponse::checkMethod()
{
	std::string	method = Requset->getMethod();

	if (/*comparing location block method and requested method*/)
	{
		if (method == "GET")
			methodGet();
		else if (method == "POST")
			methodPost();
		else
			methodDelete();
	}
}

std::string HttpRequest::createResponse()
{

}

void	HttpResponse::methodGet()
{

}

void	HttpResponse::methodPost()
{

}

void	HttpResponse::methodDelete()
{

}