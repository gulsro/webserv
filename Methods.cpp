#include "utils.hpp"
#include "HttpRequest.hpp"

void	HttpResponse::returnResponse(enum e_statusCode code)
{
	this->statusCode = code;

	if (this->statusCode == STATUS_MOVED)
	{
		std::ostringstream	stream;
		stream << "HTTP/1.1 " << this->statusCode << " " << this.getStatusMessage() << "\r\n";
		
	}
}

// Iterates allowed methods container and execute method. 
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
	else
	{
		Response->getStatusMessage(405); // Method Not Allowed
	}
}

void	HttpResponse::checkURI()
{
	std::string	uri = Request->getURI();
	std::string	method = Request->getMethod();
	
	if (uri[uri.size() - 1] != '/')
	{
		if (method == "DELETE")
			returnResponse(STATUS_CONFLICT);
		else
			returnResponse(STATUS_MOVED);
	}
}

void	HttpResponse::methodGet()
{
	// Resource is a file
	if (this->resourceType == FILE)
	{
		checkMethod();
		returnFile(resource);
	}
	else // Resource is a directory
	{
		checkURI();

	}

}

void	HttpResponse::methodPost()
{

}

void	HttpResponse::methodDelete()
{

}