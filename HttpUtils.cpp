#include "utils.hpp"
#include <regex>

std::string trim(const std::string &str)
{
	std::string result(str);

	auto const first{ result.find_first_not_of(' ') };
	if (std::string::npos == first)
		return {};
	auto const	last{ result.find_last_not_of(' ') };

	return result.substr(first, (last - first + 1));
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string>	tokens;
	std::string					token;
	std::istringstream			stream(str);

	while (std::getline(stream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

std::string	getStatusMessage(int statusCode)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] getStatusMessage" << DEFAULT << std::endl;
	#endif

	switch (statusCode)
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
		case 418:
			return ("I'm a teapot");
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
