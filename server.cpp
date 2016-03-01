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
    int new_socket, sockfd;
    int nready;
    int client[FD_SETSIZE]; // array to hold clients
    int i; // loop variable
    struct sockaddr_in server, client_addr;
    char *bp, buf[BUFLEN];
    int n;
    fd_set allset, rset;
    bp = (char*)malloc(sizeof(char) * BUFLEN); // MAKE SURE YOU FREE THIS
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
    FD_ZERO(&allset); // clear the set
    FD_SET(listen_socket, &allset); // add the listen socket to the set
    while (1)
    {
        rset = allset;
        // use select to determine if a socket has activity
        nready = select(max_descriptors + 1, &rset, NULL, NULL, NULL);
        // create new client socket
        if (FD_ISSET(listen_socket, &rset))
        {
            client_len = sizeof(client_addr);
            if ((new_socket = accept(listen_socket, (struct sockaddr *) &client_addr, (socklen_t *)&client_len)) == -1)
            {
                perror("Accept error");
                exit(1);
            }
            for (i = 0; i < FD_SETSIZE; i++)
            {
                client[i] = new_socket;
                break;
            }
            if (i == FD_SETSIZE)
            {
                fprintf(stderr, "Too many clients.\n");
                exit(1);
            }
            FD_SET(new_socket, &allset);
            max_descriptors = (new_socket > max_descriptors) ? new_socket : max_descriptors;
            max_clients = (i > max_clients) ? i : max_clients;
            if (--nready <= 0)
            {
                continue;
            }
            // handle receiving from a client
            for (i = 0; i <= max_clients; i++)
            {
                // check for empty socket
                if ((sockfd = client[i]) < 0)
                {
                    continue;
                }
                if (FD_ISSET(sockfd, &rset))
                {
                    n = read_from_socket(sockfd, bp, BUFLEN);
                    // handle sending to all clients
                    write_to_clients(buf, BUFLEN, client, max_clients, i);
                    if (n == 0)
                    {
                        close(sockfd);
                        FD_CLR(sockfd, &allset);
                        client[i] = -1;
                    }
                    if (--nready <= 0)
                    {
                        break;
                    }
                }
            }
        }
    }
    // free resources
    free(bp);
    return 0;
}

/*
 * Function: write_to_clients
 * Date: March 01, 2016
 * Revision: v1
 * Designer: Dylan
 * Programmer: Dylan
 * Interface: void write_to_clients(char *buf, int bufsize, int *clients, int num_clients, int sender)
 *              buf : buffer of message to send
 *              bufsize : max length of the buffer,
 *              clients : pointer to array of client sockets
 *              num_clients : max number of clients to send to
 *              sender : the client that sent the original message
 * Returns: void
 * Notes:
 *  Modifies the message in buf to include information such as 
 *  username and timestamp, then sends that modified message
 *  to each client EXCEPT the client identifed as the sender.
 */
void write_to_clients(char *buf, int bufsize, int *clients, int num_clients, int sender)
{
}

/*
 * Function: read_from_socket
 * Date: March 01, 2016
 * Revision: v1
 * Designer: Dylan
 * Programmer: Dylan
 * Interface: int read_from_socket(int socket, char *buf, int bufsize)
 *              socket : socket to read from
 *              buf : buffer to read into
 *              bufsize : size of the buffer or max size to read
 * Returns: number of characters read
 * Notes:
 *  reads from the specified buffer up to bufsize characters.
 *  It is the caller's responsibility to ensure that bufsize does
 *  not overflow the buffer.
 */
int read_from_socket(int socket, char *buf, int bufsize)
{
    int n;
    int total_bytes_read = 0;
    int bytes_to_read = bufsize;
    while ((n = read(socket, buf, bytes_to_read)) > 0)
    {
        buf += n; // advance the buffer by the amount read
        total_bytes_read +=n;
        bytes_to_read -= n;
    }
    return total_bytes_read;
}
