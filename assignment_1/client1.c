// this program is developed to show a server client TCP connection
// Author : Rajat Sharma
// Date: 01/09/2017
// Computer Networking Laboratory, Tezpur University

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>    //socket library
#include<arpa/inet.h> 	  //inet_addr structure library
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;						// initializing a structure
    char message[1000], server_reply[2000];			// buffers to store message

	// socket creation
    sock = socket(AF_INET , SOCK_STREAM , 0);		
    
	if (sock == -1)								
    {
        printf("Could not create socket");
		return;
    }
	
    puts("Socket created");							
     
	// Prepare the sockaddr_in structure 
    server.sin_addr.s_addr = inet_addr("127.0.0.1");	// server address	
    server.sin_family = AF_INET;						// TCP family
    server.sin_port = htons( 8888 );					// port number
 
    // Connect
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    // communication via server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
         
        // Send some data
        if( send(sock ,message ,strlen(message) , 0) < 0 )
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock ,server_reply ,2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
    }
     
    close(sock);
    return 0;
}
