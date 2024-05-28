#include "utils.hpp"
#include "HttpRequest.hpp"

void	HttpResponse::returnResponse(enum e_statusCode code)
{
	this->statusCode = code;

	if (this->statusCode == STATUS_MOVED
		|| this->statusCode == STATUS_NO_CONTENT
		|| this->statusCode == STATUS_CREATED)
	{
		std::ostringstream	ostream;
		ostream << "HTTP/1.1 " << this->statusCode << " " << this.getStatusMessage() << "\r\n";
		if (this->statusCode == STATUS_MOVED)
			ostream << "Locaion: " << this->resource + "/" < "\r\n";
		ostream << "Content-Length: 0\r\n";
 		ostream << "\r\n";
		this->content = ostream.str(); // a string a copy of ostream
	}
	if (this->statusCode >= 400)
		std::cerr << this->statusCode << " " << this->getStatusMessage() << std::endl;  
	this->completed = true;
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
		this->statusCode == 405; // Method Not Allowed
		Response->getStatusMessage(); 
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
	if (completed == true)
		return ;
	// Resource is a file
	if (this->resourceType == FILE)
	{
		checkMethod();
		returnFile(resource);
	}
	else // Resource is a directory
	{
		checkURI();
		if (completed == false)
		{
			returnFile();
		}
	}

}

void	HttpResponse::methodPost()
{
}

void	HttpResponse::deleteFile()
{
	int	result;

	result = remove(this->resource.c_str());
	if (result == 0)
		returnResponse(STATUS_NO_CONTENT);
	else
		returnResponse(STATUS_INTERNAL_ERR);
}

void	HttpResponse::deleteDir()
{
	std::string command = "rm -rf " + this->resource;
	int			result = std::system(command.c_str());
	if (result == 0)
	{ // Directory deleted successfully.
		returnResponse(STATUS_NO_CONTENT);
	}
	else
		returnResponse(STATUS_INTERNAL_ERR);
}

void	HttpResponse::methodDelete()
{
	if (completed == true)
		return ;
	if (this->resourceType == FILE)	
		deleteFile();
	else
	{
		checkURI();
		if (completed == false)
			deleteDir();
	}
}