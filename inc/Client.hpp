#pragma once

#include <iostream>
#include <exception>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "types.hpp"
#include "Config.hpp"
#include "Msg.hpp"

class Client {

private:
	pollfd						pfds[2];
	Config						conf;
	std::string					_host;
	std::string					_pass;
	int							_port;
	std::vector<std::string>	toSend;

public:
	Client(const std::string &hostname, const std::string &port, const std::string &password, const std::string &config);
	~Client();

	void	pollIn(pollfd &pfd);
	void	pollOut(pollfd &pfd);
	void	checkPoll(void);

	void	Connect(void);
	void	registerUser(void);

	void	Start(void);

	void	sendMsg(const std::string &msg);
	void	recvMsg(const std::string &msg);
};