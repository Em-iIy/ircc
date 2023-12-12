#include "Client.hpp"
#include "colour.h"

int	main(int argc, char **argv)
{
	if (argc != 5)
	{
		std::cerr << RED "Error" RESET ": Not enough parameters" << std::endl;
		std::cerr << "Usage: ircc <hostname> <port> <password> <config>" << std::endl;
		return (1);
	}
	try
	{
		Client client(argv[1], argv[2], argv[3], argv[4]);
	}
	catch(const std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	catch(...)
	{
		return (1);
	}
}