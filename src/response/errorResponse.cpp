#include "utils.hpp"
#include "HttpResponse.hpp"

std::string	createErrorResponse(int code)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createErrorResponse" << DEFAULT << std::endl;
	#endif
		std::ostringstream	ostream;

		ostream << "HTTP/1.1 " << code << " " << returnStatusMessage(code) << "\r\n";
		if (code >= 400 && code <= 501)
		{
			std::string	filename = "./error_pages/" + std::to_string(code) + ".html";
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
		}
		else
		{
			ostream << "Content-Length: 0\r\n";
 			ostream << "\r\n";
		}
		return ostream.str();
}
