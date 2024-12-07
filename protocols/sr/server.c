#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<arpa/inet.h>

void main(){
    char *ip = "127.0.0.100";
    int port = 5568,x=0,n,sockfd,b;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;
    srand(time(0));
    int recd[10];

    for (int i = 0; i < 10; i++)
        recd[i] = -1;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0) {
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(n<0){
        perror("Bind error");
        exit(1);
    }

    while (1)
    {
        bzero(buffer, 1024);
        addr_size = sizeof(client_addr);
        recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_size);
        sleep(1);

        int packet = atoi(buffer);
        if (recd[packet - 1] == 1 || rand() % 5 == 0)
            continue;
        else if (rand() % 4 == 0)
        {
            printf("Server: Received corrupted packet %s. Sending negative acknowledgement\n", buffer);
            sprintf(buffer, "NACK %d", packet);
            sendto(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        }
        else
        {
            printf("Server: Received packet %s. Sending acknowledgement\n", buffer);
            sprintf(buffer, "ACK %d", packet);
            sendto(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
            recd[packet - 1] = 1;
        }
    }
}