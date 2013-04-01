#include "Client.hpp"

void Client::CommonInit() {
	BufferLength = 100;

	totalcnt = 0;

	logfile_name = "client.log";

	conf = "../client.conf";
	load_parameters();
}

Client::Client(string sip) : server(sip) {
	CommonInit();
}

Client::Client() : server("127.0.0.1") {
	CommonInit();
}

Client::~Client() {}

void Client::get_input_window(Window *w) {
	input_win = w;
}

void Client::get_output_window(Window *w) {
	output_win = w;
}

void Client::load_parameters() {

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
		output_win->write(buff.str());
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

void Client::run() {
	write_to_log(logfile_name, "Start...\n");
	
	while (true) {
		data = input_win->read();
		ostringstream buff;
		buff << " " << get_time() << " " << nickname << ": " << data;
		output_win->write(buff.str());

		if (data.find("/") == 0) {
			parse_command();
			continue;
		}
		
		if (sd)
			write_to_server();
		else {
			buff.str("");
			buff << " Warning! Not connected!\n";
			output_win->write(buff.str());
		}
	}

	close_connect();
}

void Client::parse_command() {

	ostringstream buff;
	
	if (data.find("/connect") == 0) {
		vector<string> vec = split(data);
		if (vec.size() > 2)
			server = vec.at(1);
		if (sd)
			close(sd);
		create_socket();
		get_host_address();
		connect_to_server();
		return;
	}

	if (data.find("/nickname") == 0) {
		vector<string> vec = split(data);
		if (vec.size() > 2) {
			nickname = vec.at(1);
			buff << " Nickname was changed to " << nickname;
			output_win->write(buff.str());
		}
		else {
			buff << " Your nickname is " << nickname;
			output_win->write(buff.str());
		}
		return;
	}
	
	if (data.find("/exit") == 0) {
		close_connect();
		pthread_exit(0);
	}

	if (data.find("/help") == 0) {
		
		buff << "\t" << "/connect <ip>" << "\t" << " - connect to server";
		output_win->write(buff.str());
		buff.str("");
		buff << "\t" << "/nickname <new nickname>" << "\t" << " - change nickname";
		output_win->write(buff.str());
		buff.str("");
		buff << "\t" << "/exit" << "\t" << " - close program";
		output_win->write(buff.str());
		buff.str("");
		buff << "\t" << "/help" << "\t" << " - show this message";
		output_win->write(buff.str());
		return;
	}

	buff << " Wrong command!\n";
	output_win->write(buff.str());
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
		ostringstream buff;
		buff << "Client-socket() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		output_win->write(buff.str());
		pthread_exit(0);
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
			ostringstream buff;
			buff << "HOST NOT FOUND --> h_errno = " << h_errno << "\n";
			write_to_log(logfile_name, buff.str());
			write_to_log(logfile_name, "Command usage: <programm name> <server IP>\n");
			buff.str("");
			buff << " Error!! See " << logfile_name << "\n";
			output_win->write(buff.str());
			close_connect();
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
	ostringstream buff;
	if ((rc = connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		buff << "Client-connect() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		output_win->write(buff.str());
		close_connect();
	}
	else {
		buff << " Connection with " << server << " established...\n";
		write_to_log(logfile_name, buff.str());
		output_win->write(buff.str());
	}
}

void Client::write_to_server() {
	/* Send string to the server using */
	/* the write() function. */
	/*********************************************/
	/* Write() some string to the server. */
	write_to_log(logfile_name, "Client-Writing...\n");
	sleep(1); //wait server

	char* dt = (char*)data.c_str();

	rc = write(sd, dt, sizeof(dt));
	
	ostringstream buff;

	if (rc < 0) {	
		buff << "Client-write() error: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		buff.str("");
		buff << " Error!! See " << logfile_name << "\n";
		output_win->write(buff.str());
		rc = getsockopt(sd, SOL_SOCKET, SO_ERROR, &temp, &length);
		close_connect();
	}

	else if (rc == 0) {
		/* Print out the asynchronously received error. */
		errno = temp;
		buff << "SO_ERROR was: " << strerror(errno) << "\n";
		write_to_log(logfile_name, buff.str());
		close_connect();
	}

	else {
		write_to_log(logfile_name, "Client-write() is OK\n");
		write_to_log(logfile_name, "String successfully sent!\n");
		write_to_log(logfile_name, "Waiting the server to echo back...\n");
	}
}

void Client::close_connect() {
	/* When the data has been read, close() */
	/* the socket descriptor. */
	/****************************************/
	/* Close socket descriptor from client side. */
	write_to_log(logfile_name, "Client-Closing...\n");
	close(sd);
	pthread_exit(0);
}

string Client::get_server() {
	ostringstream buff;
	buff << " Server: " << server << "\n";
	return server;
}
