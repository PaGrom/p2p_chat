#include "Client.hpp"

Client::Client(string sip) {
	BufferLength = 100;
	server = sip;
	server_port = 3111;

	sd, rc, length = sizeof(int);
	totalcnt = 0;
	data = "This is a test string from client lol!!! ";
}

Client::Client() : Client("127.0.0.1") {

}

Client::~Client() {}

string Client::get_server() {
	printf("Server: %s\n", server.c_str());
	return server;
}
