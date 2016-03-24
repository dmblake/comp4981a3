/*
 * Source file: server.cpp
 * Date: March 01, 2016
 * Revision: 
 *      v3 -- March 23, 2016 -- does not send to sender
 *      v2 -- March 05, 2016 -- broke up main into component functions
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
 * Revision: v2 -- March 23, 2016 -- added Client structure
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
    int listen_socket, port, max_descriptors, max_clients;
    int new_socket, sockfd;
    int nready;
    Client clients[MAX_CONN]; // array to hold clients
    int i; // loop variable
    int client_pos = 0;
    char *bp;
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
    // start the server
    listen_socket = start_server(port);
    if (listen_socket == 0)
    {
        // failed to start server
        exit(1);
    }
    listen(listen_socket, MAX_CONN);
    // initialize number of descriptors and clients
    max_descriptors = listen_socket;
    max_clients = -1;
    for (i = 0; i < MAX_CONN; i++)
    {
        clients[i].socket = -1; // -1 means available client
    }
    while (1)
    {
        FD_ZERO(&allset); // clear the set
        FD_SET(listen_socket, &allset); // add the listen socket to the set
        rset = allset;
        max_descriptors = listen_socket;
        // add each client to the set
        for (i = 0; i < max_clients; i++)
        {
            if (clients[i].socket > 0)
            {   
                if (fcntl(clients[i].socket, F_GETFL, 0) < 0) {
                    FD_SET(clients[i].socket, &rset);
                } else {
                    close(clients[i].socket);
                    clients[i].socket = -1;
                    update_usernames(clients);
                    print_clients(clients);
                }
            }

            if (clients[i].socket > 0)
            {
                max_descriptors = clients[i].socket;
            }
        }
        // use select to determine if a socket has activity
        nready = select(max_descriptors + 1, &rset, NULL, NULL, NULL);
        fprintf(stderr, "nready = %d\n", nready);
        // create new client socket
        if (FD_ISSET(listen_socket, &rset))
        {
            if ((new_socket = add_client(listen_socket, clients, &client_pos)) == -1)
            {
                fprintf(stderr, "Unable to add client.\n");
                break;
            }
            // get username from new socket
            get_username(clients, client_pos);
            update_usernames(clients);
            
            // update socket set
            FD_SET(new_socket, &allset);
            max_clients = (++client_pos > max_clients) ? client_pos : max_clients;
            // print current list of clients
            print_clients(clients);
            if (--nready <= 0)
            {
                continue;
            }
        }
        // handle receiving from a client
        for (i = 0; i <= max_clients; i++)
        {
            // check for empty socket
            if ((sockfd = clients[i].socket) < 0)
            {
                continue;
            }
            if (FD_ISSET(sockfd, &rset))
            {
                memset(bp, 0, BUFLEN);
                fprintf(stderr, "socket #%d flagged\n", sockfd);
                // read from sockets
                n = read_from_socket(sockfd, bp, BUFLEN);
                // if no message received, assume client closed socket
                if (n == 0)
                {
                    close(sockfd);
                    clients[i].socket = -1;
                    update_usernames(clients);
                    print_clients(clients);
                }
                else {
                    // handle sending to all clients
                    write_to_clients(bp, BUFLEN, clients, max_clients, clients[i].socket);
                }
                if (--nready <= 0)
                {
                    break;
                }
            }
        } 
    }
    // free resources
    free(bp);
    return 0;
}

/*
 * Function: add_client
 * Date: march 05, 2016
 * Revision: v2 -- added client structure
 * Designer: Dylan
 * Programmer: dylan
 * int add_client(int listen_socket, int * clients, int * client_pos)
 *                  listen_socket : socket on which an incoming
 *                                  connection was signalled
 *                  clients : array of client sockets to add the 
 *                            new connection to
 *                  client_pos : indicates position of the new
 *                               socket within the client array
 * returns: -1 upon failure, the descriptor of the new socket
 * Notes:
 *      Adds a client to the client array given a listening socket.            `
 *      If client_pos is null, no indicator of position will be returned.
 */
int add_client (int listen_socket, Client * clients, int * client_pos)
{
    int client_len;
    struct sockaddr_in client_addr;
    int i;
    int new_socket;
    client_len = sizeof(client_addr);
    if ((new_socket = accept(listen_socket, (struct sockaddr *) &client_addr, (socklen_t *)&client_len)) == -1)
    {
        perror("Accept error");
        return -1;
    }
    for (i = 0; i < MAX_CONN; i++)
    {
        if (clients[i].socket == -1)
        {
            clients[i].socket = new_socket;
            break;
        }
    }
    if (client_pos != 0)
    {
        *client_pos = i;
        if (*client_pos == MAX_CONN)
        {
            fprintf(stderr, "Too many clients.\n");
            return -1;
        }
    }
    // add ip address to structure
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, clients[i].ip, 25);
    return new_socket;
}

/*
 * Function: start_server
 * Date: march 05, 2016
 * Revision: v1
 * Designer: Dylan
 * Programmer: Dylan
 * int start_server(char *host, int port)
 *                      host : hostname or address
 *                      port : port to use
 * Returns: a file descriptor for the listening socket
 *          on failure this will be 0
 * Notes:
 *  Starts a listening socket.
 */
int start_server(int port)
{
    int sockfd;
    int sockopt_arg;
    struct sockaddr_in server;
    // create a listening socket 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create listen socket");
        return 0;
    }
    // set resuse addr
    sockopt_arg = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt_arg, sizeof(int)) == -1)
    {
        perror("Failed to setsockopt");
        return 0;
    }
    // bind listen socket
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Failed to bind");
        return 0;
    }
    return sockfd;
}

/*
 * Function: write_to_clients
 * Date: March 01, 2016
 * Revision: 
 *          v3 -- added client structure
 *          v2 -- March 23, does not send to original sender
 * Designer: Dylan & Dhivya
 * Programmer: Dylan & Dhivya
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
void write_to_clients(char *buf, int bufsize, Client *clients, int num_clients, int sender)
{
    int i;
    fprintf(stderr, "writing %s from client #%d\n", buf, sender);
    for (i = 0; i < MAX_CONN; i++)
    {
        if (clients[i].socket == -1 || clients[i].socket == sender)
        {
        	fprintf(stderr, "sender is %d\n", sender);
            continue;
        }
        fprintf(stderr, "i= %d\n", i);
        fprintf(stderr, "clients[i]%d\n", clients[i].socket);
        if (send(clients[i].socket, buf, bufsize, 0) == -1)
        {
            perror("Send failed");
            exit(1);
        }
        fprintf(stderr, "after send\n");
    }
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
    fprintf(stderr, "Read %s\n", buf);
    return total_bytes_read;
}

void get_username(Client * clients, int position)
{
    int bread;
    char uname[BUFLEN];
    bread = recv(clients[position].socket, uname, BUFLEN, 0);
    if (bread == 0)
        perror("recv fail in get_username");
    if (uname[0] == 4)
    {
        strcpy(clients[position].username, uname+1);
    } 
}

void update_usernames(Client * clients)
{
    char names[BUFLEN] = {0};
    int i;
    // set control character
    names[0] = 5;
    for (i = 0; i < MAX_CONN; i++)
    {
        if (clients[i].socket != -1)
        {
            // append usernames after control character
            strcat(names+1, clients[i].username);
            strcat(names+1, " ");
        }
    }
    write_to_clients(names, BUFLEN, clients, MAX_CONN, -1);
}

void print_clients(Client * clients)
{
    int i;
    for (i = 0; i < MAX_CONN; i++)
    {
        if (clients[i].socket != -1)
        {
            fprintf(stderr, "USERNAME: %s\n"
                    "IP ADDRESS:%s\n"
                    "SOCKET #%d\n",
                    clients[i].username,
                    clients[i].ip,
                    clients[i].socket);
        }
    }
}
