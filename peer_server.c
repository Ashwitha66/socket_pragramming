#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
void debug()
{
    printf("This is debug Line\n");
}
int init_socket(int *socketfd)
{
    *socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*socketfd < 0)
    {
        return -1;
    }
    return 1;
}
int my_port_number;
int tell_relay()
{
    // server_client_socket is server when it is like client
    int server_client_socket;
    int relay_port;
    struct hostent *relay_server;
    struct sockaddr_in relay_server_address;
    struct in_addr proper_ip_address;
    char ip_address[100];
    if (init_socket(&server_client_socket) < 0)
    {
        printf("Failed to initialize socket\n");
        return 0;
    }
    printf("Enter relay server IP Address:\n");
    scanf("%s", ip_address);

    printf("Enter relay server Port Number: \n");
    scanf("%d", &relay_port);
    debug();
    bzero((char *)&relay_server_address, sizeof(relay_server_address));
    relay_server_address.sin_family = AF_INET;

    inet_pton(AF_INET, ip_address, &proper_ip_address);

    relay_server = gethostbyaddr(&proper_ip_address, sizeof proper_ip_address, AF_INET);

    if (relay_server == NULL)
    {
        printf("Relay server doesn't exist\n.");
        return 0;
    }

    bcopy((char *)relay_server->h_addr, (char *)&relay_server_address.sin_addr.s_addr, relay_server->h_length);

    relay_server_address.sin_port = htons(relay_port);

    if (connect(server_client_socket, (struct sockaddr *)&relay_server_address, sizeof(relay_server_address)) < 0)
    {
        printf("There was an error in connecting relay server\n");
        return 0;
    }

    printf("Connected to Relay Server from peer server\n");
    char buffer[300];
    char *message = "add_peer";
    int check = write(server_client_socket, message, strlen(message));
    if (check < 0)
    {
        printf("There was an error in writing to socket\n");
        return 0;
    }
    check = read(server_client_socket, buffer, 299);
    printf("%s\n", buffer);
    my_port_number = atoi(buffer);
    printf("%d\n", my_port_number);
    shutdown(server_client_socket, 0);
}

int main(int argc, char *argv[])
{

    tell_relay();

      int server_socket;
    int process_socket;
     int clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, client_address;
     int n;

     server_socket = socket(AF_INET, SOCK_STREAM, 0);
     if (server_socket < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
      /* +1 because getting segmentation fault otherwise*/
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(my_port_number);
     if (bind(server_socket, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(server_socket,5);
     printf("Server running on peer node, listening.....\n");
     clilen = sizeof(client_address);
    //  newsockfd = accept(sockfd, 
    //              (struct sockaddr *) &cli_addr, 
    //              &clilen);
    //  if (newsockfd < 0) 
    //       error("ERROR on accept");
    //  close(sockfd);



    // struct sockaddr_in server_address, client_address;
    // server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // if (server_socket < 0)
    // {
    //     printf("Socket cant be initialized.\n");
    //     return 0;
    // }
    // bzero((char *)&server_address, sizeof(server_address));
    // server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = INADDR_ANY;
    // server_address.sin_port = htons(my_port_number);
    // printf("%d",my_port_number);

    // if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    // {
    //     perror("ERROR on binding");
    //     exit(1);
    // }
    // listen(server_socket, 5);
    // process_socket = accept(server_socket,(struct sockaddr *)&client_address, &clilen);
    // if(process_socket<0){
    //     printf("Process Socket not created.\n");
    //     return 0;
    // }
    // int v = read(process_socket, buffer, 255);
    // printf("%s", buffer);
    // if(v<0){
    //     printf("Couldnt read message.\n");
    //     return 0;
    // }
    
    // char* firstfile = 'file1.txt';
    // char* secondfile = 'file2.txt';
    // char* files[2] = {firstfile,secondfile};
    
    
    // for(int i = 0;i<2;i++){

    //     if(strcmp(files[i],buffer)==0){
    //         printf("Got file.\n");
    //         FILE *f = fopen(files[i], "rb");
    //         fseek(f, 0, SEEK_END);
    //         long fsize = ftell(f);
    //         fseek(f, 0, SEEK_SET); // send the pointer to beginning of the file

    //         char *string = malloc(fsize + 1);
    //         fread(string, fsize, 1, f);
    //         fclose(f);

    //         int v = write(process_socket,string,strlen(string));

    //         return 0;

    //     }
       
    // }
    // char* buf = "File NOT FOUND";
    // write(process_socket,buf,strlen(buf));
    return 0;
}