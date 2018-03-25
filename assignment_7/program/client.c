// In this program, client will encrypt the text file using a key
// server will ask for key to decrypt the text file
// Author : Rajat Sharma
// Date: 29/09/2017
// Computer Networking Laboratory, Tezpur University

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];

    portno = atoi(argv[2]);
	
	// create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(argv[1]);

    if (server == NULL) 
	{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
	bzero((char *) &serv_addr, sizeof(serv_addr));
    
	serv_addr.sin_family = AF_INET;
    
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    
	serv_addr.sin_port = htons(portno);
    
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
  
    int num1, num2, ans, choice;
    double num, answ;
    
  S:bzero(buffer,256);						
    n = read(sockfd,buffer,255);							//Read Server String
    if (n < 0) 
         error("ERROR reading from socket");
   
    printf("Server - %s",buffer);
    scanf("%d" , &choice);									//Enter choice
   
    write(sockfd, &choice, sizeof(int)); 	    			//Send choice to Server
    
    if(choice ==5 || choice ==6)
	{
		bzero(buffer,256);						
 
		n = read(sockfd,buffer,255);						//Read Server String
		if (n < 0) 
			error("ERROR reading from socket");
   
		printf("Server - %s\n",buffer);
		scanf("%lf" , &num);								//Enter No 1
		write(sockfd, &num, sizeof(double));     			//Send No 1 to Server
	}
	else
	{
		bzero(buffer,256);						
		n = read(sockfd,buffer,255);						//Read Server String
		if (n < 0) 
			error("ERROR reading from socket");
		
		printf("Server - %s\n",buffer);
		scanf("%d" , &num1);								//Enter No 1
		write(sockfd, &num1, sizeof(int));     				//Send No 1 to Server

		bzero(buffer,256);						
		n = read(sockfd,buffer,255);						//Read Server String
		
		if (n < 0) 
			error("ERROR reading from socket");
  
		printf("Server - %s\n",buffer);
		scanf("%d" , &num2);								//Enter No 2
		write(sockfd, &num2, sizeof(int));     				//Send No 2 to Server
	}
    	
	if (choice == 7)
		goto Q;
	
	if(choice==5 || choice==6)
	{
		read(sockfd , &answ, sizeof(double));				//Read Answer from Server
		printf("Server - The answer is : %lf\n" , answ);	//Get Answer from server
	}
	else
	{
		read(sockfd , &ans , sizeof(int));					//Read Answer from Server
		printf("Server - The answer is : %d\n" , ans);		//Get Answer from server
	}
    
	if(choice != 7)
    	goto S;
    	
Q:  printf("Exit");
     	
    close(sockfd);
    return 0;
}


