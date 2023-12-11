#pragma once

#include <iostream>
#include <vector>

class Msg {
private:
	std::string					original;
	std::string					src;
	std::string					cmd;
	std::vector<std::string>	params;

public:
	Msg(const std::string &msg);
	Msg(void);
	~Msg();

	void	parseCmd(void);
	void	parseParams(std::string &msg);
};