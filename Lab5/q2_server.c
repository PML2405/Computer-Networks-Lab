// 2. Write an iterative UDP client-server program where the client sends rows of a matrix, and the server
// combines them together as a matrix.

//this is server code
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
#define maxdm 10

int main(){
	int sockfd,flag,newsockfd,n;
	socklen_t clilen;
    int matrix[maxdm][maxdm];
    int rows,cols;
	char buff[256];
	struct sockaddr_in server,client;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	server.sin_family=AF_INET;
	server.sin_port=htons(PORTNO);
	server.sin_addr.s_addr=INADDR_ANY;
	flag=bind(sockfd,(struct sockaddr*)&server,sizeof(server));
	if(flag<0){
		perror("Bind failed");
		exit(1);
	}
    printf("UDP server started and waiting for matrix data...\n");
    clilen=sizeof(client);

    n=recvfrom(sockfd,buff,255,0,(struct sockaddr*)&client,&clilen);
    buff[n]='\0';
    sscanf(buff,"%d %d",&rows,&cols);
    printf("Matrix dimensions received: %d x %d\n",rows,cols);

    strcpy(buff,"READY");
    sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&client,clilen);
    printf("Sent READY signal to client.\n");
    printf("Receiving matrix rows...\n");
    for(int i=0;i<rows;i++){
        n=recvfrom(sockfd,buff,255,0,(struct sockaddr*)&client,&clilen);
        buff[n]='\0';
        printf("Received row %d: %s\n",i+1,buff);
        char *token=strtok(buff," ");
        for(int j=0;j<cols;j++){
            if(token!=NULL){
                matrix[i][j]=atoi(token);
                token=strtok(NULL," ");
            }
        }
        
    }

    printf("Complete matrix received:\n");
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }

    close(sockfd);  
                
	
	return 0;
}

