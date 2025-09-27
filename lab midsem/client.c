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
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr(IP);
    
    connect(sockfd,(struct sockaddr*)&server,sizeof(server));
    struct server_reply rep;
    struct client_msg msg;
    printf("Enter name:\n");
    fgets(msg.arr,49,stdin);
    printf("Enter reg:\n");
    scanf("%ld",&msg.reg);
    send(sockfd,(struct client_msg*)&msg,sizeof(msg),0);
    recv(sockfd,(struct server_reply*)&rep,sizeof(rep),0);
    printf("Reply from server:\n");
    printf("MSG: %s\n",rep.arr1);
    printf("PID: %d\n",rep.p);
    printf("Name: %s\n",rep.arr2);

    close(sockfd);
    
}