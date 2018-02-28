//https://www.geeksforgeeks.org/socket-programming-cc/ 

// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080


int isEmptyBuffer(char* buf, size_t count)
{
    for(int i=0;i<count;++i){
        if(buf[i])
           return 0;
    }
    return 1;
}

int main(void)
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
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
    
    char *mess;
    //while(1){
        //if (!isEmptyBuffer(buffer, 1024)){
            valread = read( new_socket , buffer, 1024);
            printf("%s\n",buffer );            
            //scanf("%s",hello);
            send(new_socket , hello , strlen(mess) , 0 );
            printf("%s sent to client\n", hello); //"Hello message sent\n"
            //bzero((char *) &buffer, sizeof(buffer));
       // }
    //}
    return 0;
}
