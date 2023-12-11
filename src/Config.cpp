#include "Config.hpp"
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
	std::ifstream	file(configFile);
	std::string		line;
	std::string		key;
	std::string		val;
	int				i = 0;
	if (file.is_open() == false)
	{
		std::cerr << "Error: File " << configFile << ": Could not be opened" << std::endl;
		throw std::exception();
	}
	while (std::getline(file, line))
	{
		if (line == "" || line[0] == '#')
			continue ;
		std::size_t pos = line.find('=');
		if (pos == std::string::npos)
		{
			std::cerr << "Error: Config: " << configFile << ":" << i << " Missing value" << std::endl;
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
			std::cerr << "Error: Config: " << configFile << ":" << i << " `" << key << "` Unknown identifier" << std::endl;
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
		std::cerr << "Error: Config: " << configFile << " Missing fields" << std::endl;
		std::cerr << "Required fields:" << std::endl;
		std::cerr << " - username" << std::endl;
		std::cerr << " - nickname" << std::endl;
		std::cerr << " - realname" << std::endl;
		throw std::exception();
	}
}
