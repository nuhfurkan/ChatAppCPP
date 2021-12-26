// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <thread>
#include <future>

#define PORT 8080

using namespace std;
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
char hello[1024] = {0};

class HandleMessages {
public:
    void getMessage() {
        while (1) {
            //printf("get initiated\n");
            memset(&buffer, 0, sizeof(buffer)); //clear the buffer
            recv(new_socket, buffer, 1024, 0);
            if (strcmp(buffer, "quit") == 0) {
                return;
            }
            printf("Client: %s \n", buffer);
        }
    }
    
    void sendMessage() {
        while (1) {
            //printf("send initiated\n");
            printf("> ");
            scanf(" %[^\n]", hello);
            send(new_socket , hello , strlen(hello) , 0 );
            if (strcmp(hello, "quit") == 0) {
                return;
            }
        }
    }
};

int main(int argc, char const *argv[])
{
    printf("started\n");
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080
    if (::bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    printf("Waiting...\n");
    
    std::thread t1(&HandleMessages::getMessage, HandleMessages());
    std::thread t2(&HandleMessages::sendMessage, HandleMessages());
    t1.join();
    t2.join();
    
    return 0;
}

