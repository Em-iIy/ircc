#include "Msg.hpp"
#include "utils.hpp"
#include "colour.h"

void	Msg::parseCmd(void)
{
	size_t	space = 0;
	size_t	pos = 0;
	std::string	param;
	if (this->original[0] == ':')
	{
		pos = 1;
		space = this->original.find(' ', pos);
		this->src = this->original.substr(pos, space - pos);
		pos = space + 1;
		space = this->original.find(' ', pos);
		this->cmd = this->original.substr(pos, space - pos);
	}
	else
	{
		space = this->original.find(' ');
		this->cmd = this->original.substr(0, space);
	}
	param = this->original.substr(space + 1);
	this->parseParams(param);
}

void	Msg::parseParams(std::string &msg)
{
	// Remove \r\n from msg string
	rmCRLF(msg);
	if (msg == "")
		return ;
	std::string	temp;

	size_t colon = msg.find(" :");
	size_t	space = msg.find(' ');
	size_t	pos = 0;

	while (space != std::string::npos && this->params.size() < 14)
	{
		// Separate out the trailing argument after the ':'
		if (msg[pos] == ':')
		{
			temp = msg.substr(pos + 1);
			if (temp != "")
				this->params.push_back(temp);
			return ;
		}
		// Separate out the argument
		if (space != pos) // otherwise it's an empty string
		{
			temp = msg.substr(pos, space - pos);
			this->params.push_back(temp);
		}
		// Move arg to 1 past the space and find next space
		pos = space + 1;
		space = msg.find(' ', pos);
	}
	// Separate out the trailing argument after the ':'
	if (this->params.size() == 14)
	{
		colon = msg.find(" :", pos);
		if (colon != std::string::npos)
			temp = msg.substr(colon + 2);
		if (temp != "")
			this->params.push_back(temp);
		return ;
	}
	if (msg[pos] == ':')
	{
		temp = msg.substr(pos + 1);
		if (temp != "")
			this->params.push_back(temp);
		return ;
	}
	// Get the last parameter
	if (pos != msg.length()) // otherwise it's an empty string
	{
		temp = msg.substr(pos);
		this->params.push_back(temp);
	}
}

Msg::Msg(const std::string &msg): original(msg)
{

	this->parseCmd();
	if (src != "")
		std::cout << "<" << BLUE << this->src << RESET << ">";
	if (cmd != "")
		std::cout << "<" << YELLOW << this->cmd << RESET << ">";
	for (std::vector<std::string>::iterator it = this->params.begin(); it != this->params.end(); ++it)
	{
		std::cout << "<" << *it << ">";
	}
	std::cout << std::endl;
}

Msg::Msg(void): original("")
{
}

Msg::~Msg(void)
{
}