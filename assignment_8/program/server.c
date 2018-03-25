// this program is UDP client server chat broadcast program
// Author : Rajat Sharma
// Date: 13/10/2017
// Computer Networking Laboratory, Tezpur University

#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

main()
{
    struct sockaddr_in cliaddr,myaddr;
    int servsock,clisock;
    char b1[100],b2[100];
 
	// create socket
	servsock=socket(AF_INET,SOCK_DGRAM,0);
    
	// creating sockaddr_in structure
	myaddr.sin_family=AF_INET;
	myaddr.sin_port=2000;
	myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	//bind
	bind(servsock,(struct sockaddr *)&myaddr,sizeof(myaddr));

	printf("\nServer ready,waiting for client....\n");
	clisock=sizeof(cliaddr);

	while(1)
	{
		recvfrom(servsock,b1,sizeof(b1),0,(struct sockaddr *) &cliaddr,&clisock);
		
		if(!(strcmp(b1,"end")))
			break;
    
		printf("\nCliaddr:%s",b1);
		printf("\nmyaddr:");
		gets(b2);
		sendto(servsock,b2,sizeof(b2),0,(struct sockaddr *) &cliaddr,clisock);
	}
}
