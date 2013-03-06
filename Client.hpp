#ifndef _Client_hpp
#define _Client_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <string>

using namespace std;

class Client {
private:
	/* BufferLength is 100 bytes */
	int BufferLength;
	/* Default host name of server system. Change it to your default */
	/* server hostname or IP.  If the user do not supply the hostname */
	/* as an argument, the_server_name_or_IP will be used as default*/
	string server;
	/* Server's port number */
	int server_port;

	int sd;
	int rc;
	int length;
	struct sockaddr_in serveraddr;
	// char buffer[BufferLength];
	char temp;
	int totalcnt;
	struct hostent *hostp;
	string data;

public:
	Client(string sip);
	Client();
	~Client();

	string get_server();
};

#endif
