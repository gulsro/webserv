#include "Webserv.hpp"

void	printParsedRequest(HttpRequest *Request)
{
	std::cout << PURPLE << "Method	:	" << Request->getMethod() << DEFAULT<< std::endl;
	std::cout << PURPLE << "URI	:	" << Request->getURI() << DEFAULT<< std::endl;
	std::cout << PURPLE << "Version	:	" << Request->getVersion() << DEFAULT<< std::endl;
	if (!Request->getQueryPairs().empty())
	{
		std::cout << "________QUERY________" << std::endl;
		std::cout << PURPLE << "QUERY_STRING : " << Request->getQueryString() << std::endl << std::endl;
		for (const auto& [key, value] : Request->getQueryPairs())
		{
			std::cout << PURPLE << "key [ " << key << " ]"  << DEFAULT;
			std::cout << PURPLE << " & value [ " << value << " ]" << DEFAULT << std::endl;
			std::cout << std::endl;
		}
	}
	auto it = Request->getHeaders().begin();
	std::cout << GREEN "________HEADERS________" << std::endl;
	while (it != Request->getHeaders().end())
	{
		std::cout << GREY << "[ " << it->first << " ]" << std::endl;
		std::cout << PURPLE << it->second.key << " : " << it->second.value << DEFAULT << std::endl;
		++it; 
	}
	std::cout << GREEN << "requestedPort : " << Request->getRequestedPort() << std::endl;
	std::cout << GREEN << "contentType : " << Request->getContentType() << std::endl;

	if (!Request->getBody().empty())
	{
		std::cout << YELLOW << "[FUNCTION] parsing body" << DEFAULT << std::endl;
		std::cout << PURPLE << "contentLength	: " << Request->getContentLength() << DEFAULT << std::endl;
		std::cout << PURPLE << "body	: " << Request->getBody() << DEFAULT << std::endl;
	}

}