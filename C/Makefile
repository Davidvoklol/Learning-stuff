tcp-server: 
	gcc -o ./build/server ./src/TCP/server.c
tcp-client: ./src/TCP/client.c
	gcc -o ./build/client ./src/TCP/client.c

tcp: ./src/TCP/server.c ./src/TCP/client.c
	gcc -o ./build/server ./src/TCP/server.c
	gcc -o ./build/client ./src/TCP/client.c

clear: ./build/
	rm -f ./build/*
	touch ./build/.gitignore
