#include "Client.hpp"

Client::Client(string sip) {
	BufferLength = 100;
	server = sip;
	server_port = 3111;

	sd, rc, length = sizeof(int);
	totalcnt = 0;
	data = "This is a test string from client lol!!! ";

	create_socket();
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

string Client::get_server() {
	printf("Server: %s\n", server.c_str());
	return server;
}
