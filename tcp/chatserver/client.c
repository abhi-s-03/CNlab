#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

void *server_chat(void *sockid) {
    char data[1024];
    int clientsock = *((int *)sockid);
    while (1) {
        memset(data, 0, 1024);
        int read_bytes = recv(clientsock, data, 1024, 0);
        data[read_bytes] = '\0';
        printf("%s\n", data);
    }
}

int main() {
    char buffer[1024];
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    printf("Connection established\n");

    pthread_t thread;
    pthread_create(&thread, NULL, server_chat, (void *)&sock);

    while (1) {
       scanf("%s",buffer);

        if (strcmp(buffer, "SEND") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            
            printf("Enter message: ");
            scanf(" %[^\n]",buffer);
            send(sock, buffer, strlen(buffer), 0);
        }
    }

    pthread_join(thread, NULL);
    close(sock);
    return 0;
}