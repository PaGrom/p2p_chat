#ifndef _Server_hpp
#define _Server_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

using namespace std;

class Server {
private:
	int BufferLength;
	int server_port;
	int sd;
	int sd2;
	int rc;
	unsigned int length;
	int totalcnt;
	int on;
	char temp;
	char buffer[100];
	struct sockaddr_in serveraddr;
	struct sockaddr_in their_addr;
	 
	fd_set read_fd;
	struct timeval timeout;

public:
	Server();
	~Server();

	void run();

	void create_socket();
	void allow_socket();
	void bind_socket();
	void get_ready();
	void accept_socket();
	void get_ready_to_read();
	void write_to_client_back();
	void close_connect();
};

#endif
