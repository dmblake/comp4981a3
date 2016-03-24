#pragma once

#include "../network.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>


int main(int argc, char* argv[]);
int read_from_socket(int socket, char *buf, int bufsize);
void write_to_clients(char *buf, int bufsize, Client *clients, int num_clients, int sender);
int start_server(int port);
int add_client(int listen_socket, Client * clients, int * client_pos);
void get_username(Client * clients, int position);
void update_usernames(Client * clients);
void print_clients(Client * clients);
