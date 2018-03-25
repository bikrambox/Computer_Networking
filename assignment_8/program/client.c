// this program is UDP client server chat broadcast program
// Author : Rajat Sharma
// Date: 13/10/2017
// Computer Networking Laboratory, Tezpur University

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>

main()
{
    struct sockaddr_in client,server;
    int s,n;
    char b1[100],b2[100];
   
	// create socket
	s=socket(AF_INET,SOCK_DGRAM,0);
        
	// creating sockaddr_in structure
	server.sin_family=AF_INET;
    server.sin_port=2000;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    printf("\nClient ready....\n");
    n=sizeof(server);
  
	while(1)
	{
		printf("\nClient:");
        gets(b2);
    
		sendto(s,b2,sizeof(b2),0,(struct sockaddr *)&server,n);
        
		if(strcmp(b2,"end")==0)
            break;
      
		recvfrom(s,b1,sizeof(b1),0,NULL,NULL);
		printf("\nServer:%s",b1);
    }
}
