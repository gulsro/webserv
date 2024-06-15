#include "HttpRequest.hpp"

void	HttpRequest::handlePartInfo(const int n, const std::vector<std::string> strs)
{
	for (size_t i = 0; i < strs.size(); i++)
	{
		if (strs[i] == "Content-Disposition")
			makeKeyValuePair(n, strs[i + 1]);
		else if (strs[i] == "Content-Type")
			parts[n].partContentType = strs[i + 1];
	}
}

void	HttpRequest::handleMultiPartForm()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] handleMultiPartForm" << DEFAULT << std::endl;
	#endif
	std::vector<std::string>	splittedBody;

	setBoundary();
	splittedBody = splitByBoundary();
	for (size_t i = 0; i < splittedBody.size(); i++)
	{
		std::istringstream	iss(splittedBody[i]);
		std::string			line;
		// size_t	pos = splittedBody[i].find("\r\n\r\n") + 4;
		// Extracting file data
		size_t	beginDataPos = splittedBody[i].find("\n\n") + 2;
		parts[i].data = splittedBody[i].substr(beginDataPos);
		// Extracting file info
		std::string partInfo = splittedBody[i].substr(0, beginDataPos);
        std::vector<std::string> strs = splitForKeyValue(partInfo, ':');
		handlePartInfo(i, strs);
	}
	#ifdef FUNC
		for (size_t i = 0; i < this->parts.size(); i++)
		{
			std::cout << "[ " << i << " ]" << std::endl;
			std::cout << "Content type: " << parts[i].partContentType << std::endl;
			std::cout << "Filename: " << parts[i].partFilename << std::endl;
			for (size_t j = 0; j < this->parts[i].pairs.size(); j++)
			{
				std::cout << "Key	: " << this->parts[i].pairs[j].first << std::endl;
				std::cout << "Value	: " << this->parts[i].pairs[j].second << std::endl;
			}
			std::cout << "data	: " << this->parts[i].data << std::endl;
			std::cout << "--------------------------------" << std::endl;
		}
	#endif
}