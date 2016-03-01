#include "server.h"
int main(int argc, char * argv[])
{
    // init structures
    int listen_socket, client_len, port;
    struct sockaddr_in server, client_addr;
    // set variables based on command line
    switch (argc)
    {
        case 1:
            port = 4981;
            break;
        case 2:
            port = atoi(argv[1]);
            break;
    }
    // create a listening socket 
    // use select to determine if a socket has activity
    // create new client socket
    // handle receiving from a client
    // handle sending to all clients
    // free resources
    return 0;
}
