#include "Client.hpp"

Client::Client(string sip) {
	BufferLength = 100;
	server = sip;
	server_port = 3111;

	sd, rc, length = sizeof(int);
	totalcnt = 0;
	data = "This is a test string from client lol!!! ";

	create_socket();
	get_host_address();
	connect_to_server();
	write_to_server();
}

Client::Client() : Client("127.0.0.1") {}

Client::~Client() {}

void Client::create_socket() {
	/* The socket() function returns a socket */
	/* descriptor representing an endpoint. */
	/* The statement also identifies that the */
	/* INET (Internet Protocol) address family */
	/* with the TCP transport (SOCK_STREAM) */
	/* will be used for this socket. */
	/******************************************/
	/* get a socket descriptor */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Client-socket() error");
		exit(-1);
	}
	else
		printf("Client-socket() OK\n");
}

void Client::get_host_address() {
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(server_port);
	 
	if ((serveraddr.sin_addr.s_addr = inet_addr(server.c_str())) == (unsigned long)INADDR_NONE) {
		 
		/* When passing the host name of the server as a */
		/* parameter to this program, use the gethostbyname() */
		/* function to retrieve the address of the host server. */
		/***************************************************/
		/* get host address */
		hostp = gethostbyname(server.c_str());
		if (hostp == NULL) {
			printf("HOST NOT FOUND --> ");
			/* h_errno is usually defined */
			/* in netdb.h */
			printf("h_errno = %d\n",h_errno);
			printf("---This is a client program---\n");
			printf("Command usage: <programm name> <server name or IP>\n");
			close(sd);
			exit(-1);
		}
		memcpy(&serveraddr.sin_addr, hostp->h_addr, sizeof(serveraddr.sin_addr));
	}
}

void Client::connect_to_server() {
	/* After the socket descriptor is received, the */
	/* connect() function is used to establish a */
	/* connection to the server. */
	/***********************************************/
	/* connect() to server. */
	if ((rc = connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		perror("Client-connect() error");
		close(sd);
		exit(-1);
	}
	else
		printf("Connection established...\n");
}

void Client::write_to_server() {
	/* Send string to the server using */
	/* the write() function. */
	/*********************************************/
	/* Write() some string to the server. */
	printf("Sending some string to the f***ing %s...\n", server.c_str());
	rc = write(sd, data.c_str(), sizeof(data.c_str()));
	 
	if(rc < 0) {
		perror("Client-write() error");
		rc = getsockopt(sd, SOL_SOCKET, SO_ERROR, &temp, &length);
		if(rc == 0) {
			/* Print out the asynchronously received error. */
			errno = temp;
			perror("SO_ERROR was");
		}
		close(sd);
		exit(-1);
	}
	else {
		printf("Client-write() is OK\n");
		printf("String successfully sent lol!\n");
		printf("Waiting the %s to echo back...\n", server.c_str());
	}
}

string Client::get_server() {
	printf("Server: %s\n", server.c_str());
	return server;
}
