#include "Window.hpp"

Window::Window(string name, int nlines, int ncols, int y0, int x0) {
	win = newwin(nlines, ncols, y0, x0);
	lines  = 0;
	title = name;
}

Window::~Window() {
	delwin(win);
}

void Window::refresh_win() {
	box(win, 0, 0);
	mvwprintw(win, 0, 1, " %s: ", title.c_str());
	wrefresh(win);
}

int main(int argc, char const *argv[])
{
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int ymax, xmax, height;
	getmaxyx(stdscr, ymax, xmax);

	Window *output = new Window("output", ymax - 5, xmax, 0, 0);
	Window *input = new Window("input", 5, xmax, ymax - 5, 0);

	output->refresh_win();
	input->refresh_win();

	endwin();

	return 0;
}