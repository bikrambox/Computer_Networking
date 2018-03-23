// this program is developed to show a server client TCP connection
// Author : Rajat Sharma
// Date: 01/09/2017
// Computer Networking Laboratory, Tezpur University

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>		// socket library
#include<arpa/inet.h>		// inet_addr structure library
#include<unistd.h>			// access to POSIX OS API
 
int main(int argc , char *argv[])
{
    int socket_desc ,client_sock ,c ,read_size;
    struct sockaddr_in server , client;				// initializing structures
    char client_message[2000];						// buffer to message
     
    // Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
	
    puts("Socket created");
     
    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;					// TCP family
    server.sin_addr.s_addr = INADDR_ANY;			// ip address
    server.sin_port = htons( 8888 );				// port number
     
    // Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return;
    }

    puts("bind done");

    // Listen
    listen(socket_desc, 3);

    // Accept the incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    // Accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return;
    }

    puts("Connection accepted");

    // Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        // Send the message back to client
        write(client_sock ,client_message ,strlen(client_message));
    }

    if(read_size == 0)
    {
        puts("Client disconnected");		
        fflush(stdout);						// to clear the output buffer
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}
