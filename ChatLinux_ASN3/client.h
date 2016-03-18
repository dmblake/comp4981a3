#ifndef CLIENT_H
#define CLIENT_H

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
char * add_username_to_msg(char * username, char * msg);
int send_msg(int sockfd, char * msg, char * username, int buflen, int flags);
void* recvMsg(void* param);


#endif // CLIENT_H
