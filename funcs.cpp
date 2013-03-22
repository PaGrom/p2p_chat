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
	// Open logfile for writing at the end
	ofstream logfile(logfile_name.c_str(), ios::app);
	logfile << get_time() << " " << line;
	logfile.close();
}

vector<string> split(string s) {
	vector<string> vec;
	istringstream iss(s);
	
	while (iss) {
		string sub;
        iss >> sub;
        vec.push_back(sub);
	}
	
	return vec;
}
