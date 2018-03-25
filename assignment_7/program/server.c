// In this program, server will contain a scientific calculator functions
// client will send a expression to get the result from server
// Author : Rajat Sharma
// Date: 06/10/2017
// Computer Networking Laboratory, Tezpur University

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

	// create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  

	bzero(&serv_addr,8);

	// creating sockaddr_in structure
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));
 
	//bind
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1) 	
		error("binding");

	//listen
	listen(sockfd,5);
    clilen=sizeof(cli_addr);
    
	//accept
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);

    int choice;

 S: n = write(newsockfd,"Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Sine\n6.Cosine\n7.Exit\n",strlen("Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n6.Sine\n7.Cosine\n8.Exit\n"));           
    if (n < 0) 
		error("ERROR writing to socket");
    
	read(newsockfd, &choice, sizeof(int));				
    printf("Client - Choice is : %d\n" , choice);
     
	int num1 , num2 , ans;
	double num,answ;
	
	if(choice==5|| choice==6)
	{
		write(newsockfd,"Enter number : ",strlen("Enter number : "));
		read(newsockfd,&num1,sizeof(double));
		printf("Client number is :%lf\n",num);
	}
	else
	{
		n = write(newsockfd,"Enter Number 1 :\n ",strlen("Enter Number 1 :\n"));        
		read(newsockfd, &num1, sizeof(int));				
		printf("Client - Number 1 is : %d\n" , num1);
             
		n = write(newsockfd,"Enter Number 2 : \n ",strlen("Enter Number 2 : \n "));           
		read(newsockfd, &num2, sizeof(int));			
		printf("Client - Number 2 is : %d\n" , num2);   
	}
     
    switch(choice)
    {
     	case 1:
     		ans = num1 + num2;
     		break;
     	case 2:
     		ans = num1 -num2;
     		break;
     	case 3:
     		ans = num1*num2;
     		break;
     	case 4:
     		ans = num1/num2;
     		break;
     	case 5 :
			printf("Test:Sin(30): %.2lf\n",sin(30));
			answ=sin(num);
			break;
		case 6:
			answ=cos(num);
			break;
		case 7:
			goto Q;
     		break;
     }
     
	 if(choice==5||choice==6)
	 {
		write(newsockfd,&answ,sizeof(double));
	 }
	 else
		write(newsockfd,&ans,sizeof(int));
		 
     if(choice !=7)
     	goto S;
    
 Q:  close(newsockfd);
     close(sockfd);
     return 0; 
}
