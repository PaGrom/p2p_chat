all:
	g++ funcs.cpp Server.cpp Client.cpp main.cpp -o main -pthread