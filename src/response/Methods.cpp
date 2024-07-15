#include "utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <sys/stat.h> // stat()
#include <filesystem> // filesystem

const std::string	HttpRequest::getHeaderValue(const std::string &key) const
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] getHeaderValue" << DEFAULT << std::endl;
	#endif
	auto	it = headers.find(key);
	
	if (it != headers.end())
		return it->second.value;
	else
		return "";
}

void	HttpResponse::setMIMEtype(const std::string &filename)
{
	#ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] setMIMEtype" << DEFAULT << std::endl;
	#endif

	std::unordered_map<std::string, std::string> mimeTypes = 
	{
		{".html", "text/html"},
		{".htm", "text/html"},
		{".css", "text/css"},
		{".js", "text/javascript"},
		{".json", "application/json"},
		{".png", "image/png"},
		{".jpg", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".gif", "image/gif"}
	};
	// if  Content-Type exists in the request Header
	std::string contentType = Request->getHeaderValue("Content-Type");
	if (!contentType.empty())
	{
		size_t	semicolonPos = contentType.find(';');
		if (semicolonPos != std::string::npos)
		this->MIMEtype = contentType.substr(0, semicolonPos);
	}
	else
	{
		size_t pos = filename.find_last_of('.');
		std::string fileExtension = "";

		if (pos == std::string::npos)
			this->MIMEtype = "application/octet-stream"; // Default for unknown type
		else
			fileExtension = filename.substr(pos);
		auto	it = mimeTypes.find(fileExtension);
		if (it != mimeTypes.end())
			this->MIMEtype = it->second;
		else
			this->MIMEtype = "application/octet-stream"; // Default for unknown type
	}
}

bool	HttpResponse::checkResourcePermission(const std::string path)
{
	#ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] checkResourcePermission" << DEFAULT << std::endl;
	#endif

	const char	*file = path.c_str();
	std::string	method = this->Request->getMethod();
	bool		readable = false, writable = false;  
	
	if (access(file, R_OK) == 0)
		readable = true;
	if (access(file, W_OK) == 0)
		writable = true;

	if (method == "GET")
	{
		if (readable == true)
			return true;
	}
	else if (method == "POST" )
	{
		if (readable == true && writable == true)
			return true;
	}
	else if (method == "DELETE")
	{
		if (writable == true)
			return true;
	}
	return false;
}

void	HttpResponse::checkResourceType()
{
	#ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] checkResourceType" << DEFAULT << std::endl;
	#endif
	std::string	path;
	struct stat buf;
	
	if (this->completed == true)
		return	;
	path = this->resource;
	std::cout << "RESOURCE:::" << path << std::endl;
	if (stat(path.c_str(), &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
		{
			std::cout << "It's directory" << std::endl;
			this->setResourceType(RESOURCE_DIR);
		}
		else if (S_ISREG(buf.st_mode))
			this->setResourceType(RESOURCE_FILE);
	}
	else
	{
		this->content = createErrorResponse(STATUS_NOT_FOUND);
		this->completed = true;
	}
}

void	HttpResponse::printDefaultPage()
{
	Server *server = this->Request->ReqServer;
	std::string	defaultPage = "." + server->getRoot() + "/" + server->getIndex();
	createResponse_File(defaultPage);
}

void	HttpResponse::checkURI()
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] checkURI" << DEFAULT << std::endl;
	#endif
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

void	HttpResponse::printDirectoryListing(const std::string &path)
{
	if (std::filesystem::exists(path) == true)
	{
		std::string contnet = "";
		std::ostringstream oss;

		oss << "<!DOCTYPE html>\n<html lang=\"en\">\n";
		oss << "<head>\n<meta charset=\"UTF-8\">\n";
		oss << "<title>Directory Listing</title>\n";
		oss << "</head>\n";
		oss << "<body>\n";
		oss << "<h2>" << path << "</h2>\n";
		oss << "<ul>\n";
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_regular_file())
				oss << "<li><a href=\"" << entry.path().filename().string() << "\">" << entry.path().filename().string() << "</a></li>";
			else
				oss << "<li><a href=\"" << entry.path().filename().string() << "/\">" << entry.path().filename().string() << "/</a></li>";

			// oss << "<li>" << entry.path() << "</li>";
		}
		oss << "</ul>\n";
		oss << "</body></html>";
		content = oss.str();
		createResponse(STATUS_SUCCESS, content);
	}
	else
	{
		createErrorResponse(STATUS_NOT_FOUND);
		this->completed = true;
	}
}

void	HttpResponse::methodGet()
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] methodGet" << DEFAULT << std::endl;
	#endif

	if (this->completed == true)
		return ;
	// Resource is a file
	if (this->resourceType == RESOURCE_FILE)
	{
		createResponse_File(getResource());
	}
	else // Resource is a directory
	{
		checkURI();
		if (completed == false)
		{
			if  (this->Request->ReqLocation && this->Request->ReqLocation->getAutoindex() == true)
			{
				printDirectoryListing(this->resource);
			}
			else
				printDefaultPage();
		}
	}
}

void    HttpResponse::postFile()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] postFile" << DEFAULT << std::endl;
	#endif
    std::string filename;
    for (size_t i = 0; i < Request->parts.size(); i++)
    {
        filename = this->resource + "/" + Request->parts[i].partFilename;
        std::ofstream   file(filename.c_str());
        if (file.is_open())
        {
            file << Request->parts[i].data;
            file.close();
           createResponse(STATUS_CREATED);
        }
    }
}

void	HttpResponse::methodPost()
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] methodPost" << DEFAULT << std::endl;
	#endif
	if (this->completed == true)
		return ;
	std::string	location = this->resource;
    bool		dirExists = std::filesystem::exists(location);

	if (dirExists == false)
        createResponse(STATUS_NOT_FOUND);
	postFile();
}

void	HttpResponse::deleteFile()
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] deleteFile" << DEFAULT << std::endl;
	#endif

	int	result;

	result = remove(this->resource.c_str());
	if (result == 0)
		createResponse(STATUS_SUCCESS);
	else
		createResponse(STATUS_INTERNAL_ERR);
}

void	HttpResponse::deleteDir()
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] deleteDir" << DEFAULT << std::endl;
	#endif

	std::string command = "rm -rf " + this->resource;
	int			result = std::system(command.c_str());
	if (result == 0)
	{ // Directory deleted successfully.
		createResponse(STATUS_SUCCESS);
	}
	else
		createResponse(STATUS_INTERNAL_ERR);
}

void	HttpResponse::methodDelete()
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] methodDelete" << DEFAULT << std::endl;
	#endif
	if (this->completed == true)
		return ;
	if (this->resourceType == RESOURCE_FILE)	
		deleteFile();
	else // Resource is a directory
	{
		checkURI();
		if (this->completed == false)
			deleteDir();
	}
}

// Iterates allowed methods container and execute method. 
void	HttpResponse::checkMethod()
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] checkMethod" << DEFAULT << std::endl;
	#endif
	std::string	method = Request->getMethod();

	setResource();
	checkResourceType();
	if (this->resourceType == RESOURCE_FILE && fileExists(this->resource) == false && this->Request->cgi == nullptr)
		throw ErrorCodeException(STATUS_NOT_FOUND);
	if (checkResourcePermission(this->resource) == false && this->Request->cgi == nullptr)
		throw ErrorCodeException(STATUS_FORBIDDEN);
	if (method == "GET" || (method == "POST" && Request->contentLength == 0 ))
		methodGet();
	else if (method == "POST")
		methodPost();
	else
		methodDelete();
}