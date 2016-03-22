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
        return -1;
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
    char *temp;
    int size;
    temp = (char*)malloc(strlen(username) + strlen(msg));
    // check for overflow
    strcpy(temp, username);
    strcat(temp, " : ");
    strcat(temp, msg);
    if ((size = strlen(temp)) > BUFLEN)
    {
        fprintf(stderr, "strings too large add_username_to_msg\n");
        return 0;
    }
    return temp;
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



