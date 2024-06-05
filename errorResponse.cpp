#include "utils.hpp"
#include "HttpResponse.hpp"

std::string	createErrorResponse(int code)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createErrorResponse" << DEFAULT << std::endl;
	#endif
		std::ostringstream	ostream;

		ostream << "HTTP/1.1 " << code << " " << getStatusMessage(code) << "\r\n";
		ostream << "Content-Length: 0\r\n";
 		ostream << "\r\n";

		return ostream.str();
}

// const char*	HttpResponse::ErrorCodeException::what() const throw()
// {
// 	return createErrorResponse(errorCode); 
// }
