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