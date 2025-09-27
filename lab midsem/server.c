#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 10200
#define IP "127.0.01"

struct server_reply{
    char arr1[100];
    int p;
    char arr2[50];
};

struct client_msg{
    char arr[50];
    long int reg;
};

int main(){
    int status,sockfd,newsockfd;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server, client;
    socklen_t clilen;
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr(IP);
    status=bind(sockfd,(struct sockaddr*)&server,sizeof(server));
    if (status<0){
        perror("Bind failed");
        exit(1);
    }
    struct server_reply rep;
    struct client_msg msg;
    listen(sockfd,5);
    while(1){
        newsockfd=accept(sockfd,(struct sockaddr*)&client,&clilen);
        if(fork()==0){
            recv(newsockfd,(struct client_msg*)&msg,sizeof(msg),0);
            sprintf(rep.arr1,"Operation Sindhoor 2025");
            strcpy(rep.arr2,msg.arr);
            rep.p=getpid();
            send(newsockfd,(struct server_reply*)&rep,sizeof(rep),0);
            close(newsockfd);
        }else{
            close(newsockfd);
        }
    }
    close(sockfd);
    return 0;

}