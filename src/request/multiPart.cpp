#include "HttpRequest.hpp"

void	HttpRequest::makeKeyValuePair(int n, const std::string str)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] makeKeyValuePair" << DEFAULT << std::endl;
	#endif
	(void)n;
	std::vector<std::string> splittedStr = split(str, ';');
	// i = 1 : skipping Content-Disposition value. (e.g. form-data;)
	for (size_t i = 1; i < splittedStr.size(); ++i)
	{
		// Removing if '\r' is the end of a string
		size_t tmpPos = splittedStr[i].find('\r');
		if (tmpPos != std::string::npos)
		{
			std::string tmp = splittedStr[i].substr(0, tmpPos);
			splittedStr[i] = tmp;
		}
		std::vector<std::string> keyValue = splitForKeyValue(splittedStr[i], '=');
		std::string key = trim(keyValue[0], ' ');
		std::string value = trim(keyValue[1], '"');
		this->parts[n].pairs.push_back(std::make_pair(key, value));
		if (key == "filename")
		{
			this->parts[n].partFilename = value;
			this->body = key + "="+ "/html/" + value;
		}
	}
}

void	HttpRequest::handlePartInfo(const int n, const std::string partInfo)
{
	std::vector<std::string> strs = splitForKeyValue(partInfo, ':');

	for (size_t i = 0; i < strs.size(); ++i)
	{
		if (strs[i] == "Content-Disposition")
			makeKeyValuePair(n, strs[i + 1]);
		else if (strs[i] == "Content-Type")
			parts[n].partContentType = strs[i + 1];
	}
}

std::vector<std::string> HttpRequest::splitByBoundary()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] splitByBoundary" << DEFAULT << std::endl;
	#endif
	std::vector<std::string> parts;
	size_t begin = 0;
	size_t end = this->body.find(boundaryEnd);
	size_t pos = 0;

	while (begin < this->body.size() && begin != end)
	{
		pos = this->body.find(this->boundaryBegin, begin);
		if (pos != std::string::npos)
		{
			pos += this->boundaryBegin.size();
			size_t next = this->body.find(this->boundaryBegin, pos);
			if (next != std::string::npos)
			{
				parts.push_back(this->body.substr(pos, (next - pos) - 1));
				begin = next;
			}
			else
			{
				parts.push_back(this->body.substr(pos, (end - pos) - 1));
				begin = pos;
			}
		}
		else
			begin = pos;
	}
	return parts;
}

void	HttpRequest::handleMultiPartForm()
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] handleMultiPartForm" << DEFAULT << std::endl;
	#endif
	std::vector<std::string>	splittedBody;

	setBoundary();
	splittedBody = splitByBoundary(); 
	for (size_t i = 0; i < splittedBody.size(); ++i)
	{
		std::istringstream	iss(splittedBody[i]);
		// Extracting file data
		size_t	beginDataPos = splittedBody[i].find("\r\n\r\n") + 4;
		parts[i].data = splittedBody[i].substr(beginDataPos);
		// Extracting file info
		std::string partInfo = splittedBody[i].substr(0, beginDataPos);
		handlePartInfo(i, partInfo);
	}
	#ifdef FUNC
		for (size_t i = 0; i < this->parts.size(); ++i)
		{
			std::cout << BLUE << "[ " << i << " ]" << std::endl;
			std::cout << "Content type: " << parts[i].partContentType << std::endl;
			std::cout << "Filename: " << parts[i].partFilename << std::endl;
			for (size_t j = 0; j < this->parts[i].pairs.size(); ++j)
			{
				std::cout << "Key	: [" << this->parts[i].pairs[j].first << "]" << std::endl;
				std::cout << "Value	: [" << this->parts[i].pairs[j].second << "]" << std::endl;
			}
			std::cout << "data	: " << this->parts[i].data << std::endl;
			std::cout << "--------------------------------" << DEFAULT << std::endl;
		}
	#endif
}