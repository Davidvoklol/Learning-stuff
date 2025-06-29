# pragma once

void OnListen(int socket_fd);
void OnConnect(int socket_fd);
void OnRecive(int socket_fd, char* message);
char* Send();
void OnSent(int socket_fd, char* message);
