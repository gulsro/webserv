#include "utils.hpp"

std::string trim(const std::string &str)
{
	std::string result(str);

	auto const first{ result.find_first_not_of(' ') };
	if (std::string::npos == first)
		return {};
	auto const	last{ result.find_last_not_of(' ') };

	return result.substr(first, (last - first + 1));
}