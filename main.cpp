#include "Client.hpp"

int main(int argc, char const *argv[]) {
	/*If the server hostname is supplied*/
	Client* client;
	if(argc > 1)
		client = new Client(argv[1]);
	else
	/*Use the default server name or IP*/
		client = new Client();
	
	client->run();

	return 0;
}
