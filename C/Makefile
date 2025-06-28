tcp-server: 
	gcc -o ./build/server ./src/Networking/TCP/server.c
tcp-client: ./src/Networking/TCP/client.c
	gcc -o ./build/client ./src/Networking/TCP/client.c

tcp: ./src/Networking/TCP/server.c ./src/Networking/TCP/client.c
	gcc -o ./build/server ./src/Networking/TCP/server.c
	gcc -o ./build/client ./src/Networking/TCP/client.c

clear: ./build/
	rm -f ./build/*
	touch ./build/.gitignore
