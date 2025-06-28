#include <errno.h>
#include <stdio.h>      // For input/output (printf, etc.)
#include <stdlib.h>     // For general utilities (exit, malloc)
#include <string.h>     // For string operations (memset, strlen)
#include <unistd.h>     // For close() and other POSIX functions
#include <sys/types.h>  // For data types used in sockets
#include <sys/socket.h> // For socket(), bind(), listen(), accept()
#include <netinet/in.h> // For sockaddr_in structure (internet addresses)
#include <arpa/inet.h>


int main() {
	int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket_fd < 0) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	const char server_ip[] = "127.0.0.1";

	// creating a sockaddr_in structure for the ip and port information for the SERVER 
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;          // IPv4
	inet_pton(AF_INET, server_ip, &server_address.sin_addr);
	server_address.sin_port = htons(8080);        // Port 8080 (network byte order)

	int server_connect_success = connect(client_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)); 

	if (server_connect_success < 0) {
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	while (1) {
		char *line = NULL;
		size_t len = 0;
		ssize_t nread;

		printf("Enter message: ");
		nread = getline(&line, &len, stdin);

		if (strstr(line, "quit")) {
			close(client_socket_fd);
			printf("Session closed\n");
			break;
		}

		if (nread != -1) {
			send(client_socket_fd, line, strlen(line), 0);

			char buffer[1024];
			ssize_t bytes_recived = recv(client_socket_fd, &buffer, sizeof(buffer) - 1, 0);

			if (bytes_recived == 0) {
				printf("Session closed.\n");
				break;
			}

			buffer[bytes_recived] = '\0';

			printf("Response: %s\n", buffer);
		}
		free(line);
	}
	close(client_socket_fd);
}
