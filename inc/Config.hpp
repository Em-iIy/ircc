#pragma once

#include <iostream>
#include <exception>
#include "types.hpp"

enum key_e {
	KEY_NONE,
	KEY_USER,
	KEY_NICK,
	KEY_HOST,
	KEY_SERVER,
	KEY_REAL,
};

class Config {
public:
	std::string	username;
	std::string	nickname;
	std::string	hostname;
	std::string servername;
	std::string realname;

	Config(const std::string &configFile);
};