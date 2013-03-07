#include "Server.hpp"

Server::Server() {
	BufferLength = 100;
	server_port = 3111;
	totalcnt = 0;
	on = 1;
	timeout.tv_sec = 15;
	timeout.tv_usec = 0;

	create_socket();
	allow_socket();
	bind_socket();
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
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Server-socket() error");
		/* Just exit */
		exit (-1);
	}
	else
		printf("Server-socket() is OK\n");
}

void Server::allow_socket() {
	/* The setsockopt() function is used to allow */
	/* the local address to be reused when the server */
	/* is restarted before the required wait time */
	/* expires. */
	/***********************************************/
	/* Allow socket descriptor to be reusable */
	if ((rc = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0) {
		perror("Server-setsockopt() error");
		close(sd);
		exit (-1);
	}
	else
		printf("Server-setsockopt() is OK\n");
}

void Server::bind_socket() {
	/* bind to an address */
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(server_port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	printf("%d\n", htonl(INADDR_ANY));
	// printf("Using %d, listening at %d\n", inet_ntoa(serveraddr.sin_addr), server_port);
	 
	/* After the socket descriptor is created, a bind() */
	/* function gets a unique name for the socket. */
	/* In this example, the user sets the */
	/* s_addr to zero, which allows the system to */
	/* connect to any client that used port 3005. */
	if ((rc = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		perror("Server-bind() error");
		/* Close the socket descriptor */
		close(sd);
		/* and just exit */
		exit(-1);
	}
	else
		printf("Server-bind() is OK\n");
}
