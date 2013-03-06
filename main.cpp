#include "Client.hpp"

int main(int argc, char const *argv[]) {
	/*If the server hostname is supplied*/
	if(argc > 1)
		Client* client = new Client(argv[1]);
	else
	/*Use the default server name or IP*/
		Client* client = new Client();
	
	return 0;
}
