#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

void main(){
    char *ip="127.0.0.1";
    int port=45662;
    struct sockaddr_in addr;
    int sock;
    char buffer[1024];

    sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0){
        perror("Socket creation failed\n");
        exit(1);
    }
    printf("Socket creation successfull\n");

    memset(&addr,'\0',sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);

    bzero(buffer,1024);
    strcpy(buffer,"Request");
    printf("Data to server :%s\n", buffer);
    sendto(sock,buffer,1024,0,(struct sockaddr *)&addr,sizeof(addr));

    bzero(buffer,1024);
    socklen_t size=sizeof(addr);
    recvfrom(sock,buffer,1024,0,(struct sockaddr *)&addr,&size);
    printf("Data from server :%s\n", buffer);
}