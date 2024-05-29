#include "utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <fstream> // ifstream

void	HttpResponse::createResponse(enum e_statusCode code)
{
	this->statusCode = code;

	if (this->statusCode == STATUS_MOVED
		|| this->statusCode == STATUS_NO_CONTENT
		|| this->statusCode == STATUS_CREATED)
	{
		std::ostringstream	ostream;
		ostream << "HTTP/1.1 " << this->statusCode << " " << this->getStatusMessage() << "\r\n";
		if (this->statusCode == STATUS_MOVED)
			ostream << "Locaion: " << this->resource + "/" << "\r\n";
		ostream << "Content-Length: 0\r\n";
 		ostream << "\r\n";
		this->content = ostream.str(); // a string a copy of ostream
	}
	if (this->statusCode >= 400)
		std::cerr << this->statusCode << " " << this->getStatusMessage() << std::endl;  
	this->completed = true;
}

void	HttpResponse::createResponse_File(std::string filename)
{
	std::ifstream	file(filename.c_str());

	if (file.is_open())
	{
		file.seekg(0, std::ios::end); // Move to the end of the file.
		if (file.tellg() > 0) // file is not empty.
		{
			file.seekg(0, std::ios::beg);
			std::ostringstream	ostream;
			std::ostringstream	fileContent;
			fileContent << file.rdbuf();

			ostream << "HTTP/1.1 200 OK\r\n";
			ostream << "Content-Length: " << fileContent.str().length() << "\r\n";
			ostream << "Content-Type: " << getMIMEtype() << "\r\n";
			ostream << "\r\n";
			// print body part
			ostream << fileContent.str();
			file.close();
			this->content = ostream.str();
			createResponse(STATUS_SUCCESS);
		}
		else
			createResponse(STATUS_NO_CONTENT);
	}
	else
		createResponse(STATUS_INTERNAL_ERR);
}

const std::string	HttpRequest::getHeaderValue(const std::string &key) const
{
	auto	it = headers.find(key);
	
	if (it != headers.end())
		return it->second.value;
	else
		return "";
}

std::string	HttpResponse::getMIMEtype() const
{
	std::string contentType = Request->getHeaderValue("Content-Type");

	if (contentType.empty())
		return "";

	size_t	semicolonPos = contentType.find(';');
	if (semicolonPos != std::string::npos)
		return contentType.substr(0, semicolonPos);
	else
		return contentType;
}

void	HttpResponse::checkURI()
{
	std::string	uri = Request->getURI();
	std::string	method = Request->getMethod();
	
	if (uri[uri.size() - 1] != '/')
	{
		if (method == "DELETE")
			createResponse(STATUS_CONFLICT);
		else
			createResponse(STATUS_MOVED);
	}
}

void	HttpResponse::methodGet()
{
	if (completed == true)
		return ;
	// Resource is a file
	if (this->resourceType == RESOURCE_FILE)
	{
		checkMethod();
		createResponse_File(getResource());
	}
	else // Resource is a directory
	{
		checkURI();
		if (completed == false)
		{
			// if (checkIndexFileExistence(Request->getIndex()) == true)
				createResponse_File(getResource());
			// else
			// 	checkAutoindex();
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
		createResponse(STATUS_NO_CONTENT);
	else
		createResponse(STATUS_INTERNAL_ERR);
}

void	HttpResponse::deleteDir()
{
	std::string command = "rm -rf " + this->resource;
	int			result = std::system(command.c_str());
	if (result == 0)
	{ // Directory deleted successfully.
		createResponse(STATUS_NO_CONTENT);
	}
	else
		createResponse(STATUS_INTERNAL_ERR);
}

void	HttpResponse::methodDelete()
{
	if (completed == true)
		return ;
	if (this->resourceType == RESOURCE_FILE)	
		deleteFile();
	else
	{
		checkURI();
		if (completed == false)
			deleteDir();
	}
}

// Iterates allowed methods container and execute method. 
void	HttpResponse::checkMethod()
{
	
	std::string	method = Request->getMethod();

	// if (/*comparing location block method and requested method*/)
	// {
		if (method == "GET")
			methodGet();
		else if (method == "POST")
			methodPost();
		else
			methodDelete();
	// }
	// else
	// {
	// 	this->statusCode == STATUS_NOT_ALLOWED; // Method Not Allowed
	// 	getStatusMessage();
	// }
}