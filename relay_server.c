#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
int server_socket;
int my_port_number;
struct sockaddr_in relay_server_address;
struct sockaddr_in client_address;
char message[300];
FILE* database;
void create_file()
{
    database = fopen("relay_database.txt", "w");
    fclose(database);
}
int init_socket()
{
    int *socketfd = &server_socket;
    *socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*socketfd < 0)
    {
        return -1;
    }
    return 1;
}

int listen_on_port()
{
    if (init_socket(&server_socket) < 0)
    {
        printf("Socket not created.\n");
        exit(1);
    }
    bzero((char *)&relay_server_address, sizeof(relay_server_address));
    relay_server_address.sin_family = AF_INET;
    relay_server_address.sin_addr.s_addr = INADDR_ANY;
    relay_server_address.sin_port = htons(my_port_number);
    int v = bind(server_socket, (struct sockaddr *)&relay_server_address, sizeof(relay_server_address));
    if (v< 0)
    {
        printf("Error in Binding.\n");
        exit(1);
    }
    listen(server_socket, 5);
}
int accept_request(){
    int client_length;
    int process_socket = accept(server_socket,(struct sockaddr*)&client_address,&client_length);


    int peer_server_port;
    if(process_socket<0){
        printf("Error in accepting socket.\n");
        return -1;
    }
    int pid = fork();

    if(pid<0){
        printf("Error in Creating child Process.\n");
        return -1;
    }
    if(pid==0){
        int check = read(process_socket,message,299);
        peer_server_port = ntohs(client_address.sin_port) + 200;
        if(check<0){
            printf("Error in reading message from client or server.\n");
            return -1;
        }
        if(strcmp(message,"add_peer")==0){
            char peer_server_ip_address[100];
            printf("Got a request from peer server, adding details of peer to database.\n");
            database = fopen("relay_database.txt","a+");
            if(inet_ntop(AF_INET, &client_address.sin_addr.s_addr, peer_server_ip_address, sizeof(peer_server_ip_address))==NULL){
                printf("Address not found!");
            }
            else{
                printf("I got a message from %s peer which is at %d port.\n",peer_server_ip_address,peer_server_port);
            }
            fprintf(database,"%s %d\n",peer_server_ip_address,peer_server_port);
            sprintf(message,"%s %d\n",peer_server_ip_address,peer_server_port);
            printf("sending the following message - %s\n", message);
            check = write(process_socket, message, strlen(message));
            if (check < 0)
            {
                printf("Error in writing message to peer server.\n");
                return -1;
            }

            fclose(database);
            return 1;
        }
        

    }
}

int main(int argc, char *argv[])
{

    
    
    if (argc < 2)
    {
        printf("Enter port number\n");
        return 0;
    }
    create_file();
    my_port_number = atoi(argv[1]);
    listen_on_port();
    printf("Server started listening\n");
    int newsockfd;
    int pid;
    int clilen;
    FILE* database;
    while (1)
    {
        accept_request();

    }
}
