#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

void main(){
    char *ip="127.0.0.1";
    int port=45662;
    struct sockaddr_in server_addr,client_addr;
    int server_sock,client_sock,bind_val;
    char buffer[1024];

    server_sock=socket(AF_INET,SOCK_STREAM,0);
    if(server_sock<0){
        perror("Socket creation failed\n");
        exit(1);
    }
    printf("Socket creation successfull\n");

    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=port;
    server_addr.sin_addr.s_addr=inet_addr(ip);

    bind_val=bind(server_sock,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(bind_val<0){
        perror("Bind failed\n");
        exit(1);
    }
    printf("Bind successfull to the port: %d\n",port);

    listen(server_sock,10);
    printf("Server listening...\n");

    memset(&client_addr,'\0',sizeof(client_addr));
    printf("Initial IP: %s\n",inet_ntoa(client_addr.sin_addr));

    while(1){
        socklen_t size=sizeof(client_addr);
        client_sock=accept(server_sock,(struct sockaddr *)&client_addr,&size);
        if(client_sock<0){
            perror("Connection error\n");
            exit(1);
        }
        printf("Client connected\n");
        printf("Current IP: %s\n",inet_ntoa(client_addr.sin_addr));

        bzero(buffer,1024);
        recv(client_sock,buffer,sizeof(buffer),0);
        printf("Client: %s\n",buffer);

        bzero(buffer,1024);
        strcpy(buffer,"Hi, I'm Server");
        send(client_sock,buffer,sizeof(buffer),0);

        close(client_sock);
        printf("Client Disconnected\n");
    }

    close(server_sock);
    printf("Server Disconnected\n");
}