/*Sample Program: Write an iterative TCP client server program where client sends a message to server
and server echoes back the message to client. Client should display the original message and echoed
message. Note: As socket is also a file descriptor, we can use read and write system calls to receive
and send data.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 10200
#define IP "127.0.0.1"

int main(){
    int sockfd;
    struct sockaddr_in server;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr(IP);
    
    char buff[256];
    printf("Enter the text to send to server\n");
    fgets(buff,255,stdin);

    connect(sockfd,(struct sockaddr *)&server,(socklen_t)sizeof(server));
    send(sockfd,buff,strlen(buff),0);
    printf("Message sent to server: %s\n",buff);
    int n=recv(sockfd,&buff,255,0);
    buff[n]='\0';
    printf("Received from server: %s\n",buff);
    
    close(sockfd);
}