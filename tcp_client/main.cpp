// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char hello[1024] = {0};
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    printf("Waiting...\n");
    send(sock, "connected", strlen("connected"), 0);
    while (1) {
        memset(&buffer, 0, sizeof(buffer));//clear the buffer
        read( sock , buffer, 1024);
        if (!strcmp(buffer, "exit")) {
            printf("connection lost!");
            break;
        } else {
            printf("Server: %s\n",buffer );
            printf("> ");
            scanf(" %[^\n]", hello);
            send(sock , hello , strlen(hello) , 0 );
            if (!strcmp(hello, "exit")) {
                printf("Quiting...");
                break;
            }
        }
    }
    
    return 0;
}

