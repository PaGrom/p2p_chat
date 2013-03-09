#include "funcs.hpp"

string get_time() {
	time_t timer;
	char buffer[25];
	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);

	strftime(buffer, 25, "%Y:%m:%d %H:%M:%S", tm_info);

	return buffer;
}

void write_to_log(string logfile_name, string line) {
	FILE* logfile = fopen(logfile_name.c_str(), "at");
	fprintf(logfile, "%s %s", get_time().c_str(), line.c_str());
	fclose(logfile);
}