 #include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

void main(){
    char *ip="127.0.0.1";
    int port=45662;
    struct sockaddr_in server_addr,client_addr;
    int sock,bind_val;
    char buffer[1024];

    sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0){
        perror("Socket creation failed\n");
        exit(1);
    }
    printf("Socket creation successfull\n");

    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(ip);

    bind_val=bind(sock,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(bind_val<0){
        perror("Bind failed\n");
        exit(1);
    }
    printf("Bind successfull to the port: %d\n",port);

    bzero(buffer,1024);

    socklen_t size=sizeof(client_addr);
    recvfrom(sock,buffer,1024,0,(struct sockaddr *)&client_addr,&size);
    printf("From client:%s\n",buffer);

    strcpy(buffer,"Data Received");
    printf("Server:%s\n",buffer);
    sendto(sock,buffer,1024,0,(struct sockaddr *)&client_addr,size);
}