#ifndef NETWORK_H
#define NETWORK_H

#pragma once

#define DEFAULT_PORT 4981
#define BUFLEN 1028
#define MAX_CONN 10

typedef struct Client {
    int socket;
    char ip[16];
    char username[20]; 
} Client;


#endif // NETWORK_H
