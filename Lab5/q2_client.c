// 2. Write an iterative UDP client-server program where the client sends rows of a matrix, and the server
// combines them together as a matrix.

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
    int rows,cols;
    server.sin_family=AF_INET;
    server.sin_port=htons(PORTNO);
    server.sin_addr.s_addr=inet_addr(IP);
    
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        perror("Socket creation failed");
        exit(1);
    }

    printf("Enter number of rows and columns of the matrix : ");
    scanf("%d %d",&rows,&cols);
    
    sprintf(buff,"%d %d",rows,cols);    
    n=sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&server,sizeof(server));
    if(n<0){
        perror("Send failed");
        exit(1);
    }
    socklen_t len=sizeof(server);
    n=recvfrom(sockfd,buff,255,0,(struct sockaddr*)&server,&len);
    buff[n]='\0';
    if(strcmp(buff,"READY")!=0){
        printf("Did not receive READY from server. Exiting...\n");
        close(sockfd);
        exit(1);
    }
    printf("Server is ready. Sending matrix rows...\n");
    getchar();
    for(int i=0;i<rows;i++){
        printf("Enter row %d (space-separated %d integers): ",i+1,cols);

        fgets(buff,256,stdin);
        buff[strcspn(buff, "\n")] = 0; // remove newline character
        n=sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&server,sizeof(server));
        if(n<0){
            perror("Send failed");
            exit(1);
        }
    }
    printf("All rows sent. Closing socket.\n"); 

    close(sockfd);
    return 0;
}