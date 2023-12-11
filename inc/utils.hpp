#pragma once

#include "types.hpp"
#include <poll.h>
#include <string>

std::vector<std::string>	split(const std::string &str, const std::string &d);
void						rmCRLF(std::string &str);
void						msgLimitSize(std::string &str);
pollfd						initPFD(sockfd_t fd);
std::string					date(void);
std::string					time(void);
