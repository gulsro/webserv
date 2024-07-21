#include "utils.hpp"
#include <regex>
#include <unistd.h>

std::string trim(const std::string &str, const char c)
{
	std::string result(str);

	auto const first{ result.find_first_not_of(c) };
	if (std::string::npos == first)
		return {};
	auto const	last{ result.find_last_not_of(c) };

	return result.substr(first, (last - first + 1));
}

std::vector<std::string> splitForKeyValue(const std::string &str, char delimiter)
{
	std::vector<std::string>	tokens;
	std::string					line;
	std::istringstream			iss(str);
	
	while (std::getline(iss, line))
	{
		size_t delimPos = line.find(delimiter);
		if (delimPos != std::string::npos)
		{
			std::string	key = line.substr(0, delimPos);
			tokens.push_back(key);
			std::string	value = line.substr(delimPos + 1);
			tokens.push_back(value);
		}
	}
	return tokens;
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

std::string	returnStatusMessage(int statusCode)
{
	#ifdef FUNC
	std::cout << YELLOW << "[FUNCTION] returnStatusMessage" << DEFAULT << std::endl;
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
		case 301:
			return ("Moved Permanently"); // redirection permanently
		case 302:
			return ("Found"); // redirection temporary 
		case 400:
			return ("Bad Request");
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
		case 411:
			return ("Length Required"); // missing "Content-Length" header
		case 413:
			return ("Content Too Large");
		case 414:
			return ("URI Too Long");
		case 418:
			return ("I'm a teapot");
		case 500:
			return ("Internal Server Error");
		case 504:
			return ("Gateway Timeout");
		case 505:
			return ("HTTP Version Not Supported");
		default :
			return ("Not Implemented");
	}
}

bool	isInvalidCharForURI(const char c)
{
	std::string	specialChar = "._~:/?#[]@!$&'()*+,;=%";

	for (size_t i = 0; i < specialChar.size(); i++)
	{
		if (c != specialChar[i])
			return (false);
	}
	return (true); 
}

bool	fileExistsInDir(const std::string dir, const std::string filename)
{
	std::string		path = dir + "/" + filename;
	std::ifstream	file(path);

	return file.is_open();
}

bool	fileExists(const std::string path)
{
	if (access(path.c_str(), F_OK) == 0)
		return true;
	return false;
}
