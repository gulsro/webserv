#include <iostream>
#include <string>


void	transaction(int fd)
{
	int			isStatic;
	std::string fileName;
	int			log;
	size_t		n;

	isStatic = parseUri();
	if (isStatic)
	{
		//static content
	}
	else
	{
		//dynamic content
	}
	
}