#pragma once

#include "network.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include <netdb.h>

int main(int argc, char * argv[]);
int connect_to_server(char *host, int port);
