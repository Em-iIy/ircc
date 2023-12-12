#include "Client.hpp"
#include "utils.hpp"
#include "colour.h"



void	Client::pollIn(pollfd &pfd)
{
	int			bRead;
	char		buffer[512];
	std::string	msg;

	if (!(pfd.revents & POLLIN))
		return ;
	if (pfd.fd == STDIN_FILENO)
	{
		bRead = read(pfd.fd, buffer, sizeof(buffer) - 1);
		buffer[bRead] = '\0';
		msg = buffer;
		rmCRLF(msg);
		msg += "\r\n";
		this->sendMsg(msg);
	}
	else
	{
		bRead = recv(pfd.fd, buffer, sizeof(buffer) - 1, 0);
		if (bRead <= 0)
		{
			std::cout << "Disconnected from server." << std::endl;
			if (bRead < 0)
			{
				perror("recv");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		buffer[bRead] = '\0';
		// std::cout << buffer << std::endl;
		this->recvMsg(buffer);
	}
}

void	Client::pollOut(pollfd &pfd)
{
	if (pfd.fd == STDIN_FILENO)
		return ;
	if (!(pfd.revents & POLLOUT))
		return ;
	for (std::vector<std::string>::iterator it = this->toSend.begin(); it != this->toSend.end(); ++it)
	{
		if (send(pfd.fd, it->c_str(), it->length(), MSG_NOSIGNAL) < 0)
		{
			perror("send");
			throw std::exception();
		}
	}
	this->toSend.clear();
}

void	Client::checkPoll(void)
{
	for (int i = 0; i < sizeof(this->pfds); ++i)
	{
		pollfd	&pfd = pfds[i];
		this->pollIn(pfd);
		this->pollOut(pfd);
	}
}

void	Client::Connect(void)
{
	sockaddr_in	server;
	socklen_t	addrlen = sizeof(server);
	sockfd_t	fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd < 0)
	{
		perror("socket");
		throw std::exception();
	}
	this->pfds[1] = initPFD(fd);
	hostent *host = gethostbyname(this->_host.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(this->_port);
	server.sin_addr.s_addr = *((unsigned long *)host->h_addr_list[0]);
	if (connect(fd, (sockaddr *)&server, addrlen) < 0)
	{
		std::cerr << this->_host << ":" << this->_port;
		perror(" ");
		throw std::exception();
	}
}

void	Client::registerUser(void)
{
	this->sendMsg("PASS " + this->_pass + "\r\n");
	this->sendMsg("NICK " + this->conf.nickname + "\r\n");
	this->sendMsg("USER " + this->conf.username + " " + this->conf.hostname + " " + this->conf.servername + " :" + this->conf.realname + "\r\n");

}

void	Client::Start(void)
{
	int rc = 0;

	while (true)
	{
		rc = poll(this->pfds, sizeof(this->pfds), 100000);
		if (rc < 0)
		{
			perror("poll");
			throw std::exception();
		}
		if (rc == 0)
			break ;
		this->checkPoll();
	}
}

void	Client::sendMsg(const std::string &msg)
{
	this->toSend.push_back(msg);
}

void	Client::recvMsg(const std::string &inMsg)
{
	std::vector<std::string>	cmds = split(inMsg, "\r\n");
	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
	{
		if (*it != "")
			Msg msg = Msg(*it);
	}
}



Client::~Client()
{
	close(this->pfds[1].fd);
}

Client::Client(const std::string &hostname, const std::string &port, const std::string &password, const	std::string &config): conf(config), _host(hostname), _pass(password)
{
	try
	{
		// Check if port is only numeric characters
		if (port.find_first_not_of("0123456789") != std::string::npos)
			throw std::invalid_argument("Must be a number!");
		this->_port = std::stoi(port);
		// Check if port is within valid range
		if (this->_port < 0 || this->_port > 65536)
			throw std::out_of_range("Must be between 0-65536");
	}
	catch(const std::exception& e)
	{
		std::cerr << RED "Error" RESET ": port: " << e.what() << '\n';
		throw (std::exception());
	}
	this->pfds[0] = initPFD(STDIN_FILENO);
	this->Connect();
	this->registerUser();
	this->Start();
}