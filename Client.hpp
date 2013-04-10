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
#include <signal.h>

#include <string>
#include <iostream>
#include <sstream>

#include <vector>
#include <iterator>


#include "funcs.hpp"
#include "Window.hpp"

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
	unsigned int length;
	struct sockaddr_in serveraddr;
	// char buffer[100];
	char *buffer;
	char temp;
	int totalcnt;
	struct hostent *hostp;
	string data;

	string logfile_name;

	string nickname;

	string conf;

	Window *input_win;

	Window *output_win;

	void CommonInit();

public:
	Client(string sip);
	Client();
	~Client();

	void get_input_window(Window *w);
	void get_output_window(Window *w);

	void load_parameters();

	void run();

	void parse_command();

	void create_socket();
	void get_host_address();
	void connect_to_server();
	void write_to_server();
	void close_connect();
	void quit();

	string get_server();
};

#endif
