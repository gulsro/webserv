#include "utils.hpp"
#include "HttpResponse.hpp"

std::string	createErrorResponse(int code)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createErrorResponse" << DEFAULT << std::endl;
	#endif
	std::ostringstream	ostream;
	std::string			filename;

	ostream << "HTTP/1.1 " << code << " " << returnStatusMessage(code) << "\r\n";
	if (code >= 400 && code <= 500)
		filename = "./error_pages/" + std::to_string(code) + ".html";
	else
		filename = "./error_pages/501.html";
	std::ifstream file(filename);
	if (file.is_open())
	{
		std::ostringstream fileContent;
		fileContent << file.rdbuf();
		file.seekg(0, std::ios::end);
		ostream << "Content-Length: " << fileContent.str().length() << "\r\n";
		ostream << "\r\n";
		ostream << fileContent.str();
		file.close();
	}
	return ostream.str();
}
