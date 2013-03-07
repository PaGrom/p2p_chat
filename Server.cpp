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
	accept_socket();
	get_ready();
	write_to_client_back();
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

void Server::accept_socket() {
	/* The server will accept a connection request */
	/* with this accept() function, provided the */
	/* connection request does the following: */
	/* - Is part of the same address family */
	/* - Uses streams sockets (TCP) */
	/* - Attempts to connect to the specified port */
	/***********************************************/
	/* accept() the incoming connection request. */
	unsigned int sin_size = sizeof(struct sockaddr_in);
	if ((sd2 = accept(sd, (struct sockaddr *)&their_addr, &sin_size)) < 0) {
		perror("Server-accept() error");
		close(sd);
		exit (-1);
	}
	else
		printf("Server-accept() is OK\n");
	 
	/*client IP*/
	printf("Server-new socket, sd2 is OK...\n");
	// printf("Got connection from the f***ing client: %d\n", inet_ntoa(their_addr.sin_addr));
}

void Server::get_ready() {
	/* The select() function allows the process to */
	/* wait for an event to occur and to wake up */
	/* the process when the event occurs. In this */
	/* example, the system notifies the process */
	/* only when data is available to read. */
	/***********************************************/
	/* Wait for up to 15 seconds on */
	/* select() for data to be read. */
	FD_ZERO(&read_fd);
	FD_SET(sd2, &read_fd);
	rc = select(sd2+1, &read_fd, NULL, NULL, &timeout);
	if ((rc == 1) && (FD_ISSET(sd2, &read_fd))) {
		/* Read data from the client. */
		totalcnt = 0;
		 
		while(totalcnt < BufferLength) {
			/* When select() indicates that there is data */
			/* available, use the read() function to read */
			/* 100 bytes of the string that the */
			/* client sent. */
			/***********************************************/
			/* read() from client */
			rc = read(sd2, &buffer[totalcnt], (BufferLength - totalcnt));
			if (rc < 0) {
				perror("Server-read() error");
				close(sd);
				close(sd2);
				exit (-1);
			}
			else if (rc == 0) {
				printf("Client program has issued a close()\n");
				close(sd);
				close(sd2);
				exit(-1);
			}
			else {
				totalcnt += rc;
				printf("Server-read() is OK\n");
			}
			 
		}
	}
	else if (rc < 0) {
		perror("Server-select() error");
		close(sd);
		close(sd2);
		exit(-1);
	}
	/* rc == 0 */
	else {
		printf("Server-select() timed out.\n");
		close(sd);
		close(sd2);
		exit(-1);
	}
	 
	/* Shows the data */
	printf("Received data from the f***ing client: %s\n", buffer);
}

void Server::write_to_client_back() {
	/* Echo some bytes of string, back */
	/* to the client by using the write() */
	/* function. */
	/************************************/
	/* write() some bytes of string, */
	/* back to the client. */
	printf("Server-Echoing back to client...\n");
	rc = write(sd2, buffer, totalcnt);
	if(rc != totalcnt) {
		perror("Server-write() error");
		/* Get the error number. */
		rc = getsockopt(sd2, SOL_SOCKET, SO_ERROR, &temp, &length);
		if(rc == 0) {
			/* Print out the asynchronously */
			/* received error. */
			errno = temp;
			perror("SO_ERROR was: ");
		}
		else
			printf("Server-write() is OK\n");
		 
		close(sd);
		close(sd2);
		exit(-1);
	}
}
