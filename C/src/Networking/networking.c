#pragma once

#include <stdio.h>
#include <arpa/inet.h>

void display_socket_info(int fd);

void display_socket_info(int fd) {
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	getpeername(fd, (struct sockaddr *)&addr, &len);;

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
	int port = ntohs(addr.sin_port);

	printf("[%s:%i]", ip, port);
}
