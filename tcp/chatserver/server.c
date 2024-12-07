#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

typedef struct client {
    int id;
    int sock;
    struct sockaddr_in addr;
    socklen_t len;
} client;

client Client[1024];
pthread_t thread[1024];
int count = 0;

void *chat(void *Clientdetail) {
    struct client *clientdetail = (client *)Clientdetail;
    int index = clientdetail->id;
    int clientsock = clientdetail->sock;
    char data[1024];
    printf("Client %d connected.\n", index + 1);

    while (1) {
        int read_bytes = recv(clientsock, data, 1024, 0);
        if (read_bytes <= 0) {
            printf("Client %d disconnected.\n", index + 1);
            break;
        }
        data[read_bytes] = '\0';

        if (strcmp(data, "SEND") == 0) {
            read_bytes = recv(clientsock, data, 1024, 0);
            data[read_bytes] = '\0';
            for (int i = 0; i < count; i++) {
                if (i == index) continue;
                send(Client[i].sock, data, strlen(data), 0);
            }
        }
    }

    close(clientsock);
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Socket created\n");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_sock, 1024) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server listening on port: 45678\n");

    while (1) {
        Client[count].len = sizeof(Client[count].addr);
        Client[count].sock = accept(server_sock, (struct sockaddr*)&Client[count].addr, &Client[count].len);
        if (Client[count].sock < 0) {
            perror("Accept failed");
            continue;
        }

        Client[count].id = count;
        pthread_create(&thread[count], NULL, chat, (void *)&Client[count]);
        count++;
    }

    for (int i = 0; i < count; i++) {
        pthread_join(thread[i], NULL);
    }

    close(server_sock);
    return 0;
}