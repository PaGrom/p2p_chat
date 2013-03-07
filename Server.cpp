#include "Server.hpp"

Server::Server() {
	BufferLength = 100;
	totalcnt = 0;
	on = 1;
	timeout.tv_sec = 15;
	timeout.tv_usec = 0;
}