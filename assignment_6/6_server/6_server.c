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

int main(){

  int welcomeSocket, newSocket;
  char buffer[1024],buffer1[1024],key;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  // create socket
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  // creating sockaddr_in structure
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  
  // bind
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  // listen
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  addr_size = sizeof serverStorage;

  // Accept
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  FILE *fp,*fs;

  fp=fopen("decrypted.txt","w+");
  fs=fopen("encrypted.txt","w+");

  printf("please enter the key used to encrypt at the client:");
  scanf("%c",&key);

  recv(newSocket, buffer, 1024, 0);
  fprintf(fs,"%s",buffer);

  printf("file recieved successfully and name is encrypted.txt!!!\n");
  printf("\nthe recieved encrypted message is:\n%s\n\n",buffer);

  int i=0;
   
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

  printf("file decrypted successfully and name is decrypted.txt!!!\n");
  fprintf(fp,"%s",buffer1);
  printf("\nthe decrypted message is:\n%s\n\n",buffer1);

  return 0;
}

