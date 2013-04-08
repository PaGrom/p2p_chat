#include <signal.h>

#include "Window.hpp"
#include "Client.hpp"
#include "Server.hpp"

typedef struct {
	Window* inwin;
	Window* outwin;
}  wins_t;

wins_t wins;

void sigint(int signo) {
    (void)signo;
}

void* run_client(void* par) {
	Client* client = (Client*)par;
	client->run();
}

void* run_server(void* par) {
	Server* server = (Server*)par;
	server->run();
}

void resize_wins(int sig_number) {
	int ymax, xmax, height;
	getmaxyx(stdscr, ymax, xmax);

	wins.outwin->resize_win(ymax - 5, xmax);
	wins.inwin->resize_win(5, xmax);

	wins.outwin->refresh_win();
	wins.inwin->refresh_win();

	return;
}

int main(int argc, char const *argv[]) {
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int ymax, xmax, height;
	getmaxyx(stdscr, ymax, xmax);

	Window *output = new Window("output", ymax - 5, xmax, 0, 0);
	Window *input = new Window("input", 5, xmax, ymax - 5, 0);

	// output->refresh_win();
	// input->refresh_win();

	memset(&wins, 0, sizeof(wins));

	wins.outwin = output;
	wins.inwin = input;

	// wins->set_outwin(output);
	// wins->set_inwin(input);

	signal(SIGWINCH, resize_wins); // not crash when terminal resized

	/*If the server hostname is supplied*/
	Client* client;
	if(argc > 1)
		client = new Client(argv[1]);
	else
	/*Use the default server name or IP*/
		client = new Client();

	client->get_input_window(input);
	client->get_output_window(output);

	Server* server = new Server();

	server->get_window(output);

	// Block the SIGINT signal. The threads will inherit the signal mask
    // This will avoid them catching SIGINT instead of this thread
	sigset_t sigset, oldset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	pthread_sigmask(SIG_BLOCK, &sigset, &oldset);
	
	pthread_t t_client, t_server, t_refresh;

	// Spawn the threads
	// pthread_create(&t_refresh, NULL, &refresh_wins, (void*)wins);
	pthread_create(&t_server, NULL, &run_server, (void*)server);
	sleep(1);
	pthread_create(&t_client, NULL, &run_client, (void*)client);

	// Install the signal handler for SIGINT
    struct sigaction s;
    s.sa_handler = sigint;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;
    sigaction(SIGINT, &s, NULL);

    // Restore the old signal mask only for this thread
    pthread_sigmask(SIG_SETMASK, &oldset, NULL);

    // Wait for SIGINT to arrive
    pause();

    // pthread_cancel(t_refresh);
	pthread_cancel(t_client);
	pthread_cancel(t_server);

	delete input;
	delete output;

	endwin();
	
	return 0;
}
