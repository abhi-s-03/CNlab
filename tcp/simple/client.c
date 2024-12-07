#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

void main(){
    char *ip="127.0.0.1";
    int port=45662;
    int sock,connect_status;
    struct sockaddr_in addr;
    char buffer[1024];

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("Socket creation failed\n");
        exit(1);
    }
    printf("Socket creation successfull\n");

    memset(&addr,'\0',sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=port;
    addr.sin_addr.s_addr=inet_addr(ip);

    connect_status=connect(sock,(struct sockaddr *)&addr,sizeof(addr));
    if(connect_status<0){
        perror("Connection failed\n");
        exit(1);
    }
    printf("Connected to the server\n");

    bzero(buffer,1024);
    strcpy(buffer,"Hi,I'm Client");
    send(sock,buffer,sizeof(buffer),0);

    bzero(buffer,1024);
    recv(sock,buffer,sizeof(buffer),0);
    printf("Server: %s\n",buffer);

    close(sock);
    printf("Client Disconnected\n");
}