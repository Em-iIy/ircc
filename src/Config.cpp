#include "Config.hpp"
#include "utils.hpp"
#include "colour.h"
#include <fstream>

static key_e	checkKey(const std::string &key)
{
	if (key == "username")
		return (KEY_USER);
	if (key == "nickname")
		return (KEY_NICK);
	if (key == "hostname")
		return (KEY_HOST);
	if (key == "servername")
		return (KEY_SERVER);
	if (key == "realname")
		return (KEY_REAL);
	return (KEY_NONE);
}

Config::Config(const std::string &configFile)
{
	std::ifstream	file;
	std::string		line;
	std::string		key;
	std::string		val;
	int				i = 0;

	file.open(configFile);
	if (file.is_open() == false)
	{
		std::string	tempName;
		char *irccPath = getenv("IRCC_PATH");
		if (!irccPath)
		{
			std::cerr << RED "Error" RESET ": File " << configFile << ": Could not be opened" << std::endl;
			throw std::runtime_error(RED "Error" RESET ": IRCC_PATH not set");
		}
		std::vector<std::string> paths = split(irccPath, ":");
		for (std::vector<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
		{
			tempName = *it + "/" + configFile;
			std::cout << "Trying file: " << tempName << std::endl;
			file.open(tempName);
			if (file.is_open())
				break ;
		}
		if (file.is_open() == false)
			throw std::runtime_error(RED "Error" RESET ": File " + configFile + ": Could not be opened\n");
	}
	while (std::getline(file, line))
	{
		if (line == "" || line[0] == '#')
			continue ;
		std::size_t pos = line.find('=');
		if (pos == std::string::npos)
		{
			std::cerr << RED "Error" RESET ": Config: " << configFile << ":" << i << " Missing value" << std::endl;
			throw std::exception();
		}
		key = line.substr(0, pos);
		val = line.substr(pos + 1);

		switch (checkKey(key))
		{
		case KEY_USER:
			this->username = val;
			break ;
		case KEY_NICK:
			this->nickname = val;
			break ;
		case KEY_HOST:
			this->hostname = val;
			break ;
		case KEY_SERVER:
			this->servername = val;
			break ;
		case KEY_REAL:
			this->realname = val;
			break ;
		case KEY_NONE:
			std::cerr << RED "Error" RESET ": Config: " << configFile << ":" << i << " `" << key << "` Unknown identifier" << std::endl;
			throw std::exception();
			break ;
		}
		i++;
	}
	if (this->hostname == "")
		this->hostname = "*";
	if (this->servername == "")
		this->servername = "*";
	
	if (
		this->username == "" ||
		this->nickname == "" ||
		this->realname == ""
	)
	{
		std::cerr << RED "Error" RESET ": Config: " << configFile << " Missing fields" << std::endl;
		std::cerr << "Required fields:" << std::endl;
		std::cerr << " - username" << std::endl;
		std::cerr << " - nickname" << std::endl;
		std::cerr << " - realname" << std::endl;
		throw std::exception();
	}
}
