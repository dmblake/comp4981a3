/*
 * Source file: client.cpp
 * Date: March 4, 2016
 * Revision: v2 -- March 05, 2016 -- broke up main into component functions
 * Designer: Dylan & Dhivya
 * Programmer: Dylan & Dhivya
 * Functions:
 *  
 * Notes:
 *  TCP client connects to a server and sends and receives
 *  messages.
 */

#include "client.h"

/*
 * FUnction: main
 * Date: March 4, 2016
 * Revision: 
 *          v3 -- March 7, 2016 -- refactored command line arguments
 *          v2 -- March 7, 2016 -- broke down into sub functions
 * Designer: Dylan & Dhivya
 * Programmer: Dylan & Dhivya
 * Interface: int main(int argc, char * argv[])
 *              argc : argument count
 *              argv : arguments
 * Returns: 0 upon success, 1 on failure
 * Notes:
 *  Main entry point for the client.
 */
int main(int argc, char * argv[])
{
    char *bp, buf[BUFLEN], *host;
    char username[256] = "ANONYMOUS";
    int port = DEFAULT_PORT;
    int sock;
    int arg;
    switch (argc)
    {
        default:
        case 1:
            fprintf(stderr, "Usage: %s : -p [PORT] -s [SERVER] -u [USERNAME]\n", argv[0]);
            exit(1);
    }
    while ((arg = getopt(argc, argv, "u:s:p:")) != -1)
    {
        switch (arg)
        {
            case 'u':
            case 'U':
                strcpy(username, optarg);
                break;
            case 's':
            case 'S':
                host = optarg;
                break;
            case 'p':
            case 'P':
                port = atoi(optarg);
                break;
        }
    }
    // connect to server
    if ((sock = connect_to_server(host, port)) == -1)
    {
        fprintf(stderr, "Failed to connect\n");
        exit(1);
    }
    if (send_msg(sock, argv[0], username, BUFLEN, 0) == 0)
    {
        exit(1);
    }
    bp = buf;
    int bytes_to_read = BUFLEN; 
    // client makes repeated calls to recv until no more data is expected to arrive.
    int n = 0;
    while ((n = recv (sock, bp, bytes_to_read, 0)) < BUFLEN)
    {
        bp += n;
        bytes_to_read -= n;
    }
    printf ("%s\n", bp);
    fflush(stdout);
    close (sock);
}

/*
 * Function: connect_to_server
 * Date: March 05, 2016
 * Revision: v1
 * Designer: Dylan
 * Programmer: Dylan
 * Interface: int connect_to_server(char * host, int port)
 *                      host : pointer to a string of an internet address
 *                      port : port to use
 * Returns: a socket descriptor upon success, -1 on failure
 * Notes:
 *  Connects to the server specified by host using port
 */
int connect_to_server(char * host, int port)
{ 
    int sock, sockopt_arg;
    struct hostent * hp;
    struct sockaddr_in server;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create socket");
        return -1;
    }
    sockopt_arg = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt_arg, sizeof(int)) == -1)
    {
        perror("failed to setsockopt");
        return -1;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    // check address
    if ((hp = gethostbyname(host)) == NULL)
    {
        perror("Invalid host");
        exit(1);
    }
    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Connect failed");
        return -1;
    } 
    return sock;
}


/*
 * Function: add_username_to_msg
 * Date: March 07, 2016
 * Revision: v1
 * Designer: Dylan
 * Programmer: Dylan
 * Interface: char* add_username_to_msg(char * username, char * msg)
 *                  username : the username
 *                  msg : message to append
 * Returns: a pointer to the modified message. 0 on failure.
 * Notes:
 *  Appends the username to the message being sent.
 *  return value must be freed if call is successful.
 */
char * add_username_to_msg(char * username, char * msg)
{
    char * ret, *temp;
    int size;
    // check for overflow
    temp = strcat(username, " : ");
    temp = strcat(temp, msg);
    if ((size = strlen(temp)) > BUFLEN)
    {
        fprintf(stderr, "strings too large add_username_to_msg\n");
        return 0;
    }
    ret = (char*)malloc(size);
    ret = strncpy(ret, temp, size);
    return ret;
}

/*
 * Function: int send_msg
 * Date: March 07, 2016
 * Revision: v1
 * Programmer: Dylan
 * Designer: Dylan & Dhivya
 * Interface: int send_msg(int sockfd, char * msg, char * username, int buflen, int flags)
 *              sockfd : socket descriptor
 *              msg : message
 *              username: user name to prepend to msg
 *              buflen : length of the max message buffer size
 *              flags : flags for sending
 * Notes:
 *  Sends a message along with the user name.
 */
int send_msg(int sockfd, char * msg, char * username, int buflen, int flags)
{
    int num_sent;
    char * modified_message;
    modified_message = add_username_to_msg(username, msg);
    if (modified_message == 0)
    {
        free(modified_message);
        return 0;
    }
    if ((num_sent = send(sockfd, modified_message, buflen, flags)) == 0)
    {
        perror("Send error");
        free(modified_message);
        return 0;
    }
    free(modified_message);
    return num_sent;
}
