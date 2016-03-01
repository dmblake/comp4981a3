#pragma once

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>

#define DEFAULT_PORT 4981
#define BUFLEN 4096
#define MAX_CONN 10

int main(int argc, char* argv[]);
int read_from_socket(int socket, char *buf, int bufsize);
void write_to_clients(char *buf, int bufsize, int *clients, int num_clients, int sender);
