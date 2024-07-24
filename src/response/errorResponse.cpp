#include "utils.hpp"
#include "HttpResponse.hpp"

std::string	createErrorResponse(int code, std::map<int, std::string> errorPages)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createErrorResponse" << DEFAULT << std::endl;
	#endif
	std::ostringstream	ostream;
	std::string			filename = "";
	std::string			pageName = "";

	ostream << "HTTP/1.1 " << code << " " << returnStatusMessage(code) << "\r\n";
	if (errorPages.empty())
		pageName = "501.html";
	for (auto& [errorCode, page] : errorPages)
	{
		if (errorCode == code)
			pageName = errorPages.at(errorCode);
	}
	if (pageName.empty())
		pageName = errorPages.at(501); // default page;
	filename = "./error_pages/" + pageName;
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
