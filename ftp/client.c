#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

void main(){
    int sock;
    struct sockaddr_in addr;
    socklen_t size;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket error\n");
        exit(1);
    }

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.100");

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    char buffer[1024];
    int choice;
    char filename[100];
    FILE *fptr;
    while(1){
        printf("\n1. Upload a file(PUT)\n2. Download a file(GET)\n3. Exit(BYE)\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                bzero(buffer, 1024);
                printf("Enter filename: ");
                scanf("%s", filename);
                fptr = fopen(filename, "r");
                if (!fptr)
                    perror("File doesn't exist!\n");
                else
                {
                    strcpy(buffer, "PUT");
                    send(sock, buffer, 1024, 0);
                    bzero(buffer, 1024);
                    strcpy(buffer, filename);
                    send(sock, buffer, 1024, 0);

                    bzero(buffer, 1024);
                    while (!feof(fptr))
                    {
                        bzero(buffer, 1024);
                        fscanf(fptr, "%[^\n] ", buffer);
                        send(sock, buffer, 1024, 0);
                    }
                    bzero(buffer, 1024);
                    strcpy(buffer, "END$");
                    send(sock, buffer, 1024, 0);
                    fclose(fptr);
                    printf("File %s sent\n", filename);
                }
                break;

            case 2:
                bzero(buffer, 1024);
                strcpy(buffer, "GET");
                send(sock, buffer, 1024, 0);
                bzero(buffer, 1024);
                recv(sock, buffer, 1024, 0);
                printf("Process id: %s\n", buffer);

                printf("Enter filename: ");
                scanf("%s", filename);
                bzero(buffer, 1024);
                strcpy(buffer, filename);
                send(sock, buffer, 1024, 0);

                bzero(buffer, 1024);
                recv(sock, buffer, 1024, 0);

                if (strcmp(buffer, "404") == 0)
                    printf("File does not exist in remote server\n");
                else if (strcmp(buffer, "200") == 0)
                {
                    fptr = fopen(filename, "w");
                    while (1)
                    {
                        recv(sock, buffer, 1024, 0);
                        if (strcmp(buffer, "END$") == 0)
                            break;
                        fprintf(fptr, "%s", buffer);
                        bzero(buffer, 1024);
                    }
                    fclose(fptr);
                    printf("File %s received\n", filename);
                }
                break;
            case 3:
                bzero(buffer, 1024);
                strcpy(buffer, "BYE");
                send(sock, buffer, 1024, 0);
                close(sock);
                exit(0);
        }
    }
}