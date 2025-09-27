// 1. Write an iterative TCP client-server program where the client accepts a sentence from the user and
// sends it to the server. The server will check for duplicate words in the string. Server will find number of
// occurrences of duplicate words present and remove the duplicate words by retaining single occurrence
// of the word and send the resultant sentence to the client. The client displays the received data on the
// client screen. The process repeats until the user enter the string “Stop”. Then both the processes
// terminate.

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

int main(){
	int sockfd,flag,newsockfd,n;
	socklen_t clilen;
	char buff[256];
	struct sockaddr_in server,client;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_port=htons(PORTNO);
	server.sin_addr.s_addr=INADDR_ANY;
	flag=bind(sockfd,(struct sockaddr*)&server,sizeof(server));
	if(flag<0){
		perror("Bind failed");
		exit(1);
	}
	flag=listen(sockfd,5);
	if(flag<0){
		perror("Listen failed");
		exit(1);
	}
	printf("Server started and listening...\n");
	while(1){
		printf("Server waiting for sentence...\n");
		clilen=sizeof(client);
		newsockfd=accept(sockfd,(struct sockaddr*)&client,&clilen);
		if(newsockfd<0){
			perror("Accept failed");
			exit(1);
		}
		n=read(newsockfd,buff,255);
		buff[n]='\0';
		if (strcmp(buff,"Stop")==0){
			printf("Termination signal received. Exiting...\n");
			close(newsockfd);
			break;
		}
		printf("Received sentence: %s\n",buff);

		// Process to remove duplicate words
		char *words[100];
		int count[100]={0};
		int word_count=0;
		char *token=strtok(buff," ");
		while(token!=NULL){
			int found=0;
			for(int i=0;i<word_count;i++){
				if(strcmp(words[i],token)==0){
					count[i]++;
					found=1;
					break;
				}
			}
			if(!found){
				words[word_count]=token;
				count[word_count]=1;
				word_count++;
			}
			token=strtok(NULL," ");
		}

		char result[256]="";
		for(int i=0;i<word_count;i++){
			strcat(result,words[i]);
			strcat(result," ");
			if(count[i]>1){
				printf("Word '%s' occurred %d times\n",words[i],count[i]);
			}
		}
		result[strlen(result)-1]='\0'; // Remove trailing space

		n=write(newsockfd,result,strlen(result));
		if(n<0){
			perror("Write failed");
			exit(1);
		}
		printf("Processed sentence sent: %s\n",result);
		close(newsockfd);
	}
	
	return 0;
}

