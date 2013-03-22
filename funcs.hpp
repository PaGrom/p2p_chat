#ifndef _funcs_hpp
#define _funcs_hpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

string get_time();
void write_to_log(string logfile_name, string line);
vector<string> split(string s);

#endif
