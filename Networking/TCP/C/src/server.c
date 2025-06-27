#include <errno.h>
#include <stdio.h>      // For input/output (printf, etc.)
#include <stdlib.h>     // For general utilities (exit, malloc)
#include <string.h>     // For string operations (memset, strlen)
#include <unistd.h>     // For close() and other POSIX functions
#include <sys/types.h>  // For data types used in sockets
#include <sys/socket.h> // For socket(), bind(), listen(), accept()
#include <netinet/in.h> // For sockaddr_in structure (internet addresses)

int main() {
	// creating a new socket
	// - AF_INET -> IPv4
	// - SOCK_STREAM -> data transfer type (TCP)
	// - 0 -> default protocol (TCP for SOCK_STREAM)
	int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket_fd < 0) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	// creating a sockaddr_in structure for the ip and port information for the SERVER
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;          // IPv4
	server_address.sin_addr.s_addr = INADDR_ANY;  // Listen on any IP
	server_address.sin_port = htons(8080);        // Port 8080 (network byte order)

	// binding the socket to the IP address and port
	int server_bind_success = bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));

	if (server_bind_success < 0) {
		perror("Error binding socket");
		close(server_socket_fd);
		exit(EXIT_FAILURE);
	}

	// making the socket listen for incoming connections
	listen(server_socket_fd, 5);


	// creating a sockaddr_in structure for the ip and port information for the CLIENT
	struct sockaddr_in client_address;
	socklen_t addr_len = sizeof(client_address);

	int client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_address, &addr_len);

	while(1) {
		char buffer[1024];
		ssize_t bytes_recived = recv(client_socket_fd, &buffer, sizeof(buffer) - 1, 0);

		if (bytes_recived == 0) {
			printf("Session closed.\n");
			break;
		}

		buffer[bytes_recived] = '\0';

		char response[1024] = "You sent: ";
		strcat(response, buffer);

		printf("Client sent: %s\n", buffer);
		send(client_socket_fd, response, strlen(response), 0);
	}

	close(client_socket_fd);
	close(server_socket_fd);
}
