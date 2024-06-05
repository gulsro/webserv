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
