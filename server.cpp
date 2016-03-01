/*
 * Source file: server.cpp
 * Date: March 01, 2016
 * Revision: v1
 * Designer: Dylan & Dhivya
 * Programmer: Dylan & Dhivya
 * Functions:
 *  int main(int argc, char * argv[])
 * Notes:
 *  A single process server that accepts
 *  client connections and echoes messages sent
 *  to it.
 */ 
#include "server.h"

/*
 * Function: main
 * Date: March 01, 2016
 * Revision: v1
 * Designer: Dylan & Dhivya
 * Programmer: Dylan & Dhivya
 * Interface: int main (int argc, char* argv[])
 *              argc : number of command line arguments
 *              argv : pointer to command line arguments
 * Returns: 0 upon success, 1 on failure
 * Notes:
 *  main entry point for the program.
 */
int main(int argc, char * argv[])
{
    // init structures
    int listen_socket, client_len, port, sockopt_arg, max_descriptors, max_clients;
    int client[FD_SETSIZE]; // array to hold clients
    int i; // loop variable
    struct sockaddr_in server, client_addr;
    // set variables based on command line
    switch (argc)
    {
        case 1:
            port = DEFAULT_PORT;
            break;
        case 2:
            port = atoi(argv[1]);
            break;
        default:
            fprintf(stderr, "Usage: %s [port]\n", argv[0]);
            exit(1);
    }
    // create a listening socket 
    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create listen_socket");
        exit(1);
    }
    // set resuse addr
    sockopt_arg = 1;
    if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt_arg, sizeof(int)) == -1)
    {
        perror("Failed to setsockopt");
        exit(1);
    }
    // bind listen socket
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listen_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Failed to bind");
        exit(1);
    }
    listen(listen_socket, MAX_CONN);
    // initialize number of descriptors and clients
    max_descriptors = listen_socket;
    max_clients = -1;
    for (i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1; // -1 means available client
    }
    // use select to determine if a socket has activity
    // create new client socket
    // handle receiving from a client
    // handle sending to all clients
    // free resources
    return 0;
}
