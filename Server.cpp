#include "Server.hpp"

Server::Server() {
	BufferLength = 100;
	totalcnt = 0;
	on = 1;

	logfile_name = "server.log";

	conf = "../server.conf";
	load_parameters();
}

Server::~Server() {}

void Server::get_window(Window *w) {
	win = w;
}

void Server::load_parameters() {

	ifstream file(conf.c_str());

	stringstream is_file;

	if (file) {
		is_file << file.rdbuf();
		file.close();
	}
	else {
		ostringstream buff;
		buff << "Config file error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		win->write(buff.str());
		/* Just exit */
		pthread_exit(0);
	}

	string line;
	while (getline(is_file, line)) {
		istringstream is_line(line);
		string key;
		if (getline(is_line, key, '=')) {
			string value;
			if (getline(is_line, value)) {
				if (key == "server_port")
					server_port = atoi(value.c_str());
				else if (key == "nickname")
					nickname = value;
			}
		}
	}
}

void Server::run() {
	write_to_log(logfile_name, "Start...\n");
	create_socket();
	allow_socket();
	bind_socket();
	get_ready();
	accept_socket();
	
	while (true)
		get_ready_to_read();
	
	close_connect();
}

void Server::create_socket() {
	/* The socket() function returns a socket descriptor */
	/* representing an endpoint. The statement also */
	/* identifies that the INET (Internet Protocol) */
	/* address family with the TCP transport (SOCK_STREAM) */
	/* will be used for this socket. */
	/************************************************/
	/* Get a socket descriptor */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		ostringstream buff;
		buff << "Server-socket() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		win->write(buff.str());
		/* Just exit */
		pthread_exit(0);
	}
	else
		write_to_log(logfile_name, "Server-socket() is OK\n");
}

void Server::allow_socket() {
	/* The setsockopt() function is used to allow */
	/* the local address to be reused when the server */
	/* is restarted before the required wait time */
	/* expires. */
	/***********************************************/
	/* Allow socket descriptor to be reusable */
	if ((rc = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0) {
		ostringstream buff;
		buff << "Server-setsockopt() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		win->write(buff.str());
		close_connect();
	}
	else
		write_to_log(logfile_name, "Server-setsockopt() is OK\n");
}

void Server::bind_socket() {
	/* bind to an address */
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(server_port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 
	/* After the socket descriptor is created, a bind() */
	/* function gets a unique name for the socket. */
	/* In this example, the user sets the */
	/* s_addr to zero, which allows the system to */
	/* connect to any client that used port 3005. */
	if ((rc = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		ostringstream buff;
		buff << "Server-bind() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		win->write(buff.str());
		close_connect();
	}
	else
		write_to_log(logfile_name, "Server-bind() is OK\n");
}

void Server::get_ready() {
	/* The listen() function allows the server to accept */
	/* incoming client connections. In this example, */
	/* the backlog is set to 10. This means that the */
	/* system can queue up to 10 connection requests before */
	/* the system starts rejecting incoming requests.*/
	/*************************************************/
	/* Up to 10 clients can be queued */
	if ((rc = listen(sd, 10)) < 0) {
		ostringstream buff;
		buff << "Server-listen() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		win->write(buff.str());
		close_connect();
	}
	else
		write_to_log(logfile_name, "Server-Ready for client connection...\n");
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
	ostringstream buff;
	unsigned int sin_size = sizeof(struct sockaddr_in);
	if ((sd2 = accept(sd, (struct sockaddr *)&their_addr, &sin_size)) < 0) {
		buff << "Server-accept() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		win->write(buff.str());
		close_connect();
	}
	else
		write_to_log(logfile_name, "Server-accept() is OK\n");
	 
	/*client IP*/
	write_to_log(logfile_name, "Server-new socket, sd2 is OK...\n");
	buff.str("");
	buff << "Got connection from " << inet_ntoa(their_addr.sin_addr) << endl;
	write_to_log(logfile_name, buff.str());
}

void Server::get_ready_to_read() {
	/* The select() function allows the process to */
	/* wait for an event to occur and to wake up */
	/* the process when the event occurs. In this */
	/* example, the system notifies the process */
	/* only when data is available to read. */
	/***********************************************/
	/* Wait for up to 15 seconds on */
	/* select() for data to be read. */
	write_to_log(logfile_name, "Server-Ready to read...\n");
	
	ostringstream buff;
	timeout.tv_sec = 3600;
	timeout.tv_usec = 0;
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
			write_to_log(logfile_name, "Server-Reading...\n");

			rc = read(sd2, &buffer[totalcnt], (BufferLength - totalcnt));

			if (rc > 0) {
				totalcnt += rc;
				write_to_log(logfile_name, "Server-read() is OK\n");
				/* Shows the data */
				buff.str("");
				buff << " " << get_time() << " " << nickname << ": " << buffer;
				win->write(buff.str());

				continue;
			}

			if (rc < 0) {
				buff.str("");
				buff << "Server-read() error: " << strerror(errno) << "\n";
				write_to_log(logfile_name, buff.str());
			}

			if (rc == 0)
				write_to_log(logfile_name, "Client program has issued a close()\n");

			buff.str("");
			buff << " Error!! See " << logfile_name << "\n";
			win->write(buff.str());
			close_connect();
		}

		return;
	}

	if (rc < 0) {
		buff.str("");
		buff << "Server-select() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
	}

	if (rc == 0)
		write_to_log(logfile_name, "Server-select() timed out.\n");
	
	buff.str("");
	buff << " Error!! See " << logfile_name << "\n";
	win->write(buff.str());
	close_connect();

}

void Server::close_connect() {
	/* When the data has been sent, close() */
	/* the socket descriptor that was returned */
	/* from the accept() verb and close() the */
	/* original socket descriptor. */
	/*****************************************/
	/* Close the connection to the client and */
	/* close the server listening socket. */
	/******************************************/
	close(sd2);
	close(sd);
	pthread_exit(0);
}
