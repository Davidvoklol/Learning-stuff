all: ./src/client.c ./src/server.c
	gcc -o ./build/server ./src/server.c
	gcc -o ./build/client ./src/client.c

clear: ./build/client ./build/server
	rm ./build/client
	rm ./build/server
