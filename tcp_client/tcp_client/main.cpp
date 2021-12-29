// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <future>
#include <vector>

#define PORT 8080

int sock = 0, valread;
struct sockaddr_in serv_addr;
char hello[1024] = {0};
char buffer[1024] = {0};

class HandleMessages {
public:
    void getMessage() {
        while (1) {
            //printf("get initiated\n");
            memset(&buffer, 0, sizeof(buffer)); //clear the buffer
            read( sock , buffer, 1024);
            if (strcmp(buffer, "quit") == 0) {
                return;
            }
            printf("Server: %s\n",buffer );
        }
    }
    
    void sendMessage() {
        while (1) {
            //printf("send initited\n");
            printf("> ");
            scanf(" %[^\n]", hello);
            send(sock , hello , strlen(hello) , 0 );
            if (strcmp(hello, "quit") == 0) {
                return;
            }
        }
    }
};

int main(int argc, char const *argv[])
{
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
    
    std::thread t1(&HandleMessages::getMessage, HandleMessages());
    std::thread t2(&HandleMessages::sendMessage, HandleMessages());
    t1.join();
    t2.join();
    
    return 0;
}

