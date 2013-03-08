all:
	g++ Client.cpp main.cpp -o client
	g++ Server.cpp -o server