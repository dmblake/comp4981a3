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
 * Revision: v1
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
    int port;
    int sock;
    switch (argc)
    {
        case 2:
            port = DEFAULT_PORT;
            host = argv[1];
            break;
        case 3:
            port = atoi(argv[2]);
            host = argv[1];
            break;
        default:
            fprintf(stderr, "Usage: %s [server_ip] [port]\n", argv[0]);
            exit(1);
    }
    // connect to server
    if ((sock = connect_to_server(host, port)) == -1)
    {
        fprintf(stderr, "Failed to connect\n");
        exit(1);
    }
    if (send(sock, argv[0], BUFLEN, 0) == -1)
    {
        perror("Send failed");
        //exit(1);
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
