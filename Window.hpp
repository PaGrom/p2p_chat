#ifndef _Window_hpp
#define _Window_hpp

#include <ncurses.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <locale.h>
#include <string>

using namespace std;

class Window {
private:
	WINDOW *win;
	string title;
	int lines;

public:
	Window(string name, int nlines, int ncols, int y0, int x0);
	~Window();

	void refresh_win();
};

#endif
