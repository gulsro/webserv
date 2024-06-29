#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
	: statusCode(0), body(""), index(""), resource(""), content(""), contentType(""), MIMEtype(""), completed(false)
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
	*this = other;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other)
{
	#ifdef DEBUG
		std::cout << GREY << "HttpResponse : Copy assigment operator called" << DEFAULT << std::endl;
	#endif
	if (this != &other)
	{
		this->statusCode = other.statusCode;
		this->body = other.body;
		this->headers = other.headers;

		this->Request = other.Request;
		this->index = other.index;
		this->resource = other.resource;
		this->resourceType = other.resourceType;
		this->content = other.content;
		this->contentType = other.contentType;
		this->MIMEtype = other.MIMEtype;
	}
	return *this;

}

HttpResponse::~HttpResponse()
{
	#ifdef DEBUG
		std::cout << GREY << "HttpResponse : Destructor called" << DEFAULT << std::endl; 
	#endif
}

void	HttpResponse::createResponse(enum e_statusCode code)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createResponse" << DEFAULT << std::endl;
	#endif
	this->statusCode = code;

	if (this->statusCode == STATUS_SUCCESS
		|| this->statusCode == STATUS_MOVED
		|| this->statusCode == STATUS_NO_CONTENT
		|| this->statusCode == STATUS_CREATED)
	{
		std::ostringstream	ostream;
		ostream << "HTTP/1.1 " << this->statusCode << " " << returnStatusMessage(this->statusCode) << "\r\n";
		if (this->statusCode == STATUS_MOVED)
		{
			ostream << "Location: " << this->resource + "/" << "\r\n";
		}
		ostream << "Content-Length: 0\r\n";
 		ostream << "\r\n";
		this->content = ostream.str(); // a string a copy of ostream
	}
	if (this->statusCode >= 400)
		throw ErrorCodeException(code);
	this->completed = true;
}

void	HttpResponse::createResponse_File(std::string filename)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createResponse_File" << DEFAULT << std::endl;
	#endif
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
		}
		else
			createResponse(STATUS_NO_CONTENT);
	}
	else
		createResponse(STATUS_INTERNAL_ERR);
}

void    HttpResponse::setResource()
{
	#ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] setResource" << DEFAULT << std::endl;
	#endif
	Server	*Server = this->Request->ReqServer;
	if (this->Request->ReqLocation != nullptr)
	{
		Location	*Location = this->Request->ReqLocation;
		// Check redirection
		if	(!(Location->getRedirect().empty()))
		{
			this->resource = Location->getRedirect();
			createResponse(STATUS_FOUND);
		}
		// Check index
		if (Location->getIndex().empty())
		{
			if (Location->getAutoindex() == true)
			{
				// Set default index
				this->resource = "." + Server->getRoot() + '/' + Server->getIndex();
			}
		}
		else
		{
			if (fileExistsInDir(Location->getRoot() + Location->getPath(), Location->getIndex()) == true)
				this->resource = "." + Location->getRoot() + Location->getPath() + Location->getIndex();
			else
				throw ErrorCodeException(STATUS_FORBIDDEN);
		}
		this->resource = "." + Location->getPath() + this->Request->getURI();
	}
	else // No selected Location
	{
		if (this->Request->getURI() == "/")
			this->resource = "." + Server->getRoot() + '/' + Server->getIndex();	
		else
			this->resource = "." + Server->getRoot() + this->Request->getURI();
	}
	std::cout << YELLOW << this->resource <<std::endl;
}

// void	HttpResponse::sendResponse(int fd)
// {
// 	//Checking client is still connected??

// 	//checking is clientFd is still connected
// 	//also check "if reading request is done" therefor we need a flag ?
// 	//if (fd ....)

// 	//What's that enum status code???
// 	std::string	content = createResponse();
// 	int retVal = write(fd, content.c_str(), content.size());
// 	if (retVal == -1)
// 	{
// 		std::cout << "Here disconnect the client" << std::endl;
// 	}
// 	//delete the request, it s done
// }
