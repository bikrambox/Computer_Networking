// this program is a file transfer program for TCP server client
// Author : Rajat Sharma
// Date: 22/09/2017
// Computer Networking Laboratory, Tezpur University

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int main(){

	int clientSocket,no_of_bytes_read=0,total_bytes=0,upload_rate=0, bytes_received=0;
	double exec_time;
	clock_t start, end;
	char buffer[1024],ch;
	float upload_done=0;

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

	// connect
	if((connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size))<0)
	{
		perror("connection failed!!!");
		exit(4);
	}

	FILE *fp,*fs;

	// Downloading by the client
	fs=fopen("downloaded.txt","w+");

	if(fs==NULL)
	{
		perror("error creating file!!!!");
		exit(1);
	}

	memset(buffer,'\0',sizeof(buffer));

	while(1)
	{
        if((bytes_received = read(clientSocket, buffer, 256)) > 0)
        {
			printf("Bytes received %d buffer=%s\n",bytes_received,buffer);
			fwrite(buffer,1,bytes_received,fs);
        }
		if(bytes_received < 0)
        {
            perror("\n Read Error \n");
            exit(3);
        }
        if(buffer[strlen(buffer)-2]=='#')
			break;

        memset(buffer,'\0',sizeof(buffer));
		sleep(1);
	}
	
	printf("\nfile downloading by client done successfully!!!\n");
	fclose(fs);

	fp=fopen("fcts.txt","r");

	if(fp==NULL)
	{
		perror("error opening file!!!!");
		exit(0);
	}

	while(1)
	{
		ch=fgetc(fp);
		if(ch==EOF)
			break;
		else
			total_bytes++;
	}

	printf("total bytes = %d",total_bytes);
	fseek(fp,SEEK_SET,0);

	// user input for upload rate
	printf("\nenter the file upload rate(in bytes):");
	scanf("%d",&upload_rate);

	start=clock();

	memset(buffer,'\0',sizeof(buffer));

	while(1)
	{
		no_of_bytes_read= fread(buffer,1,upload_rate,fp);
 
		if(no_of_bytes_read > 0)
		{
			send(clientSocket,buffer,no_of_bytes_read,0);
			upload_done+=((no_of_bytes_read*100)/(float)total_bytes);
			printf("\nbuffer=%s uploading done.....%.2f %s",buffer,upload_done,"%");
		}

		if(no_of_bytes_read < 1)
		{
			if(feof(fp))
				printf("\nend of file!!!\n");
			else
				perror("\nerror in reading\n");
			break;
        }

		memset(buffer,'\0',sizeof(buffer));
		sleep(1);
	}

	end=clock();
	exec_time=((double)(end-start))/CLOCKS_PER_SEC;

	printf("\nfile uploaded in %lf seconds\n",exec_time);
	
	fclose(fp);
	close(clientSocket);

	return 0;
}