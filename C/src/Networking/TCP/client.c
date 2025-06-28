#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../networking.c"

int main() {
	int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket_fd < 0) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	const char server_ip[] = "127.0.0.1";
	const u_int16_t server_port = 8080;

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, server_ip, &server_address.sin_addr);
	server_address.sin_port = htons(server_port);

	int server_connect_success = connect(client_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)); 

	if (server_connect_success < 0) {
		perror("Connection failed");
		exit(EXIT_FAILURE);
	} else {
		display_socket_info(client_socket_fd);
		printf(" --> connected\n");
	}

	while (1) {
		char *line = NULL;
		size_t len = 0;
		ssize_t nread;

		printf("Enter message: ");
		nread = getline(&line, &len, stdin);

		if (nread > 0) {
			char *position = strstr(line, "\n");
			if (position) *position = '\0';
		}

		if (strstr(line, "quit")) {
			close(client_socket_fd);
			printf("Session closed\n");
			break;
		}

		if (nread != -1) {
			send(client_socket_fd, line, strlen(line), 0);

			char buffer[1024];
			ssize_t bytes = recv(client_socket_fd, &buffer, sizeof(buffer) - 1, 0);

			if (bytes == 0) {
				printf("Session closed.\n");
				break;
			}

			buffer[bytes] = '\0';

			display_socket_info(client_socket_fd);
			printf(" %s\n", buffer);
		}
		free(line);
	}
	close(client_socket_fd);
}
