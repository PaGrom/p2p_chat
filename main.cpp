#include "Client.hpp"
#include "Server.hpp"

void* run_client(void* par) {
	Client* client = (Client*)par;
	client->run();
}

void* run_server(void* par) {
	Server* server = (Server*)par;
	server->run();
}

int main(int argc, char const *argv[]) {
	/*If the server hostname is supplied*/
	Client* client;
	if(argc > 1)
		client = new Client(argv[1]);
	else
	/*Use the default server name or IP*/
		client = new Client();

	Server* server = new Server();
	
	pthread_t t_client, t_server;
	void* t_client_status;
	void* t_server_status;

	pthread_create(&t_server, NULL, &run_server, (void*)server);
	sleep(1);
	pthread_create(&t_client, NULL, &run_client, (void*)client);

	pthread_join(t_client, &t_client_status);
	pthread_join(t_server, &t_server_status);

	return 0;
}
