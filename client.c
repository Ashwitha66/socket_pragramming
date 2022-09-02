#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[])
{

    FILE *peers = fopen("peer.txt", "r");
    char peerName[50];
    int port;
    while (fscanf(peers, "%s %d", peerName, &port) != EOF)
    {
        int client_socket, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        struct in_addr ipv4addr;
        char buffer[256];
        char *filename = 'file1.txt';
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (argc < 3)
        {
            printf("You need to give 2 commands.\n");
            return 0;
        }
        char *server_ip_address =   peerName;
        int server_port_number = port;
        if (client_socket < 0)
        {
            printf("Socket not created.\n");
            return 0;
        }
        inet_pton(AF_INET, server_ip_address, &ipv4addr);
        server = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
        if (server == NULL)
        {
            fprintf(stderr, "ERROR, no such host\n");
            exit(0);
        }

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(server_port_number);
        /* Now connect to the server */
        if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("Connection unsuccesful.\n");
            return 0;
        }

        /* Now ask for a message from the user, this message
         * will be read by server
         */

        printf("Connection to the Peer SUCCESSFUL.\nSending File transfer Request message with the file name....\n");
        char req[50];
        
        // sprintf(req, "client");
        /* Send message to the server */
        n = write(client_socket, filename, strlen(filename));
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }

        /* Now read server response */
        bzero(buffer, 256);
        n = read(client_socket, buffer, 255);
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }
        printf("received the reply :-%s\n", buffer);
        if (strcmp(buffer, "File NOT FOUND") == 0)
        {
            // close the connection gracefully since file not found
            printf("Closing the connection gracefully since file NOT FOUND on this node...\n");
            n = shutdown(client_socket, 0);
            if (n < 0)
            {
                perror("ERROR closing the connection");
                exit(1);
            }
        } 
        else
        {
            for(int i = 0;i<strlen(buffer);i++){
                printf("%c", buffer[i]);
            }
            return 0;
        }
       
    }

    return 0;
}
