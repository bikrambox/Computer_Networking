// In this program, client will encrypt the text file using a key
// server will ask for key to decrypt the text file
// Author : Rajat Sharma
// Date: 29/09/2017
// Computer Networking Laboratory, Tezpur University

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

  int clientSocket;
  char buffer[1024],buffer1[1024],key;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  // create socket
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  // creating sockaddr_in structure
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  addr_size = sizeof serverAddr;

  // Connect
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  FILE *fp,*fs;
  
  fp=fopen("rajat.txt","r");
  fs=fopen("encypted.txt","w+");

  fscanf(fp,"%[^\t]s",buffer);
 
  printf("enter the key (any character):"); 
  scanf("%c",&key);

  int i=0;

  printf("\nthe original message is:\n%s\n\n",buffer);
 
  for(i=0;i<strlen(buffer);i++)
  {
    if(buffer[i]!=key)
    {
      buffer1[i]=buffer[i]^key;
    }
    else
    {
      buffer1[i]=buffer[i];
    }
  }

  fprintf(fs,"%s",buffer1);  

  send(clientSocket,buffer1, 1024, 0);

  printf("the file was sent successfully!!!!\n");  

  return 0;
}