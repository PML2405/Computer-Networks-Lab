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
    int sockfd,newsockfd,status;
    struct sockaddr_in server,client;
    socklen_t clilen=sizeof(client);
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr(IP);

    status=bind(sockfd,(struct sockaddr *)&server,sizeof(server));
    if (status<0){
        perror("\nBind failed");
        exit(1);
    }
    char buff[256];

    listen(sockfd,5);
    printf("Server started and waiting for connections...\n");
    while(1){
        newsockfd=accept(sockfd,(struct sockaddr *)&client,&clilen);
        int n=recv(newsockfd,&buff,255,0);
        buff[n]='\0';
        printf("Received from client: %s\n",buff);
        send(newsockfd,buff,strlen(buff),0);
        close(newsockfd);
    }
    printf("Server closing...");
    close(sockfd);
}