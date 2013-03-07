#include "Server.hpp"

Server::Server() {
	BufferLength = 100;
	totalcnt = 0;
	on = 1;
	timeout.tv_sec = 15;
	timeout.tv_usec = 0;
}

Server::~Server() {}

void Server::create_socket() {
	/* The socket() function returns a socket descriptor */
	/* representing an endpoint. The statement also */
	/* identifies that the INET (Internet Protocol) */
	/* address family with the TCP transport (SOCK_STREAM) */
	/* will be used for this socket. */
	/************************************************/
	/* Get a socket descriptor */
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Server-socket() error");
		/* Just exit */
		exit (-1);
		}
	else
		printf("Server-socket() is OK\n");
}
