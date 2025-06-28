#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../networking.c"

int main() {
	int listener_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (listener_socket_fd < 0) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in listener_address;
	listener_address.sin_family = AF_INET;
	listener_address.sin_addr.s_addr = INADDR_ANY;
	listener_address.sin_port = htons(8080);

	int listener_bind_success = bind(listener_socket_fd, (struct sockaddr *)&listener_address, sizeof(listener_address));

	if (listener_bind_success < 0) {
		perror("Error binding socket");
		close(listener_socket_fd);
		exit(EXIT_FAILURE);
	}

	fd_set fds_to_read, fds_to_write, read_fds, write_fds;
	FD_ZERO(&fds_to_read);
	FD_ZERO(&fds_to_write);

	FD_SET(listener_socket_fd, &fds_to_read);
	listen(listener_socket_fd, 5);
	int maxfd = listener_socket_fd;

	while(1) {
		read_fds = fds_to_read;
		write_fds = fds_to_write;

		int activity = select(maxfd+1, &read_fds, &write_fds, NULL, NULL);
		if (activity < 0) {
			perror("Select error");
			break;
		}

		for (int i = 0; i <= maxfd; i++) {
			if (FD_ISSET(i, &read_fds)) {
				if(i == listener_socket_fd) {
					struct sockaddr_in client_address;
					socklen_t len = sizeof(client_address);
					int new_fd = accept(listener_socket_fd, (struct sockaddr *)&client_address, &len);

					display_socket_info(new_fd);
					printf(" --> connected\n");
					FD_SET(new_fd, &fds_to_read);
					if (new_fd > maxfd) maxfd = new_fd;
				} else {
					char buffer[1024];
					int bytes = recv(i, &buffer, sizeof(buffer) - 1, 0);
					if (bytes > 0) {
						buffer[bytes] = '\0';

						display_socket_info(i);
						printf(" %s\n", buffer);
						FD_SET(i, &fds_to_write);
					} else if (bytes == 0) {
						display_socket_info(i);
						printf(" --> disconnected\n");
						close(i);
					}
					FD_CLR(i, &fds_to_read);
				}			
			}
			if (FD_ISSET(i, &write_fds) && i != listener_socket_fd) {
				char response[] = "Got your message...";
				send(i, &response, sizeof(response), 0);
				FD_CLR(i, &fds_to_write);
				FD_SET(i, &fds_to_read);
			}
		}
	}
}
