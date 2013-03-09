#include "Client.hpp"

void Client::CommonInit() {
	BufferLength = 100;
	server_port = 3111;

	totalcnt = 0;

	logfile_name = "client.log";

	nickname = "PaGrom";
}

Client::Client(string sip) : server(sip) {
	CommonInit();
}

Client::Client() : server("127.0.0.1") {
	CommonInit();
}

Client::~Client() {}

void Client::run() {
	write_to_log(logfile_name, "Start...\n");
	create_socket();
	get_host_address();
	connect_to_server();

	while (true) {
		write_to_server();
		wait_server_echo_back();
	}

	close_connect();
}

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
		char buff[100];
		snprintf(buff, sizeof(buff), "Client-socket() error: %s\n", strerror(errno));
		write_to_log(logfile_name, buff);
		printf("Error!! See %s\n", logfile_name.c_str());
		exit(-1);
	}
	else
		write_to_log(logfile_name, "Client-socket() OK\n");
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
			/* h_errno is usually defined */
			/* in netdb.h */
			char buff[100];
			snprintf(buff, sizeof(buff), "HOST NOT FOUND --> h_errno = %d\n",h_errno);
			write_to_log(logfile_name, buff);
			write_to_log(logfile_name, "Command usage: <programm name> <server IP>\n");
			printf("Error!! See %s\n", logfile_name.c_str());
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
		char buff[100];
		snprintf(buff, sizeof(buff), "Client-connect() error: %s\n", strerror(errno));
		write_to_log(logfile_name, buff);
		printf("Error!! See %s\n", logfile_name.c_str());
		close(sd);
		exit(-1);
	}
	else {
		char buff[100];
		snprintf(buff, sizeof(buff), "Connection with %s established...\n", server.c_str());
		write_to_log(logfile_name, buff);
	}
}

void Client::write_to_server() {
	/* Send string to the server using */
	/* the write() function. */
	/*********************************************/
	/* Write() some string to the server. */

	printf(" > ");
	cin >> data;
	std::cout << "\033[1A";
	
	rc = write(sd, data, sizeof(data));
	 
	if (rc < 0) {
		char buff[100];
		snprintf(buff, sizeof(buff), "Client-write() error: %s\n", strerror(errno));
		write_to_log(logfile_name, buff);
		printf("Error!! See %s\n", logfile_name.c_str());
		rc = getsockopt(sd, SOL_SOCKET, SO_ERROR, &temp, &length);
		if (rc == 0) {
			/* Print out the asynchronously received error. */
			errno = temp;
			memset(buff, 0, sizeof(buff));
			snprintf(buff, sizeof(buff), "SO_ERROR was: %s\n", strerror(errno));
			write_to_log(logfile_name, buff);
		}
		close(sd);
		exit(-1);
	}
	else {
		write_to_log(logfile_name, "Client-write() is OK\n");
		write_to_log(logfile_name, "String successfully sent!\n");
		write_to_log(logfile_name, "Waiting the server to echo back...\n");
	}
}

void Client::wait_server_echo_back() {

	while (totalcnt < BufferLength) {
		/* Wait for the server to echo the */
		/* string by using the read() function. */
		/***************************************/
		/* Read data from the server. */
		rc = read(sd, &buffer[totalcnt], BufferLength-totalcnt);

		if (rc < 0) {
			char buff[100];
			snprintf(buff, sizeof(buff), "Client-read() error: %s\n", strerror(errno));
			write_to_log(logfile_name, buff);
			printf("Error!! See %s\n", logfile_name.c_str());
			close(sd);
			exit(-1);
		}
		else if (rc == 0) {
			write_to_log(logfile_name, "Server program has issued a close()\n");
			printf("Error!! See %s\n", logfile_name.c_str());
			close(sd);
			exit(-1);
		}
		else
			totalcnt += rc;
	}

	write_to_log(logfile_name, "Client-read() is OK\n");
	printf("*%s %s: %s\n", get_time().c_str(), nickname.c_str(), data);
	memset(buffer, 0, sizeof(buffer));
}

void Client::close_connect() {
	/* When the data has been read, close() */
	/* the socket descriptor. */
	/****************************************/
	/* Close socket descriptor from client side. */
	close(sd);
	exit(0);
}

string Client::get_server() {
	printf("Server: %s\n", server.c_str());
	return server;
}
