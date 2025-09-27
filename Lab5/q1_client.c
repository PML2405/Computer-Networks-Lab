// 1. Write an iterative TCP client-server program where the client accepts a sentence from the user and
// sends it to the server. The server will check for duplicate words in the string. Server will find number of
// occurrences of duplicate words present and remove the duplicate words by retaining single occurrence
// of the word and send the resultant sentence to the client. The client displays the received data on the
// client screen. The process repeats until the user enter the string “Stop”. Then both the processes
// terminate.

//this is client code
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORTNO 10200
#define IP "127.0.0.1" // For local testing 
int main(){
    int sockfd,flag,n;
    char buff[256];
    struct sockaddr_in server;

    server.sin_family=AF_INET;
    server.sin_port=htons(PORTNO);
    server.sin_addr.s_addr=inet_addr(IP);
    
    

    while(1){
        sockfd=socket(AF_INET,SOCK_STREAM,0);
        flag=connect(sockfd,(struct sockaddr*)&server,sizeof(server));
        if(flag<0){
            perror("Connection failed");
            exit(1);
        }
        printf("Enter a sentence (or 'Stop' to terminate): ");
        fgets(buff,256,stdin);
        buff[strlen(buff)-1]='\0';
        n=write(sockfd,buff,strlen(buff));
        if (n<0){
            perror("Write failed");
            exit(1);
        }
        if (strcmp(buff,"Stop")==0){
            printf("Termination signal sent. Exiting...\n");
            break;
        }
        n=read(sockfd,buff,255);
        buff[n]='\0';
        printf("Processed sentence from server: %s\n",buff);
    }
    close(sockfd);
    return 0;
}