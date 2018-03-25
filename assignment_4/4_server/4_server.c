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

	int welcomeSocket, newSocket,bytes_received=0, download_rate=0, no_of_bytes_read=0, total_bytes=0;

	float download_done=0.0;
	double exec_time;
	clock_t start, end;
	char buffer[1024], ch;

	struct sockaddr_in serverAddr,clientAddr;
	socklen_t addr_size;

	// Create Socket
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	// Creating Sockaddr_in structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  	

	// Bind
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	// Listen
	if(listen(welcomeSocket,5)==0)
		printf("Listening\n");
	else
		printf("Error\n");

	addr_size = sizeof clientAddr;

	// Connect
	newSocket = accept(welcomeSocket, (struct sockaddr *) &clientAddr, &addr_size);

	if(newSocket<0)
	{
		perror("connection failed!!!");
		exit(2);
	}

	FILE *fp,*fs;

	// Downloading by the client
	fs=fopen("fstc.txt","r");

	if(fs==NULL)
	{
		perror("error opening file!!!!");
		exit(1);
	}

	// to get the total no. of bytes in file
	while(1)
	{
		ch=fgetc(fs);
		if(ch==EOF)
			break;
		else
			total_bytes++;
	}

	printf("total bytes = %d",total_bytes);
	fseek(fs,SEEK_SET,0);

	// user input for download rate
	printf("\n\nenter the file download rate(in bytes):");
	scanf("%d",&download_rate);

	memset(buffer,'\0',sizeof(buffer));

	// for starting the clock
	start=clock();	

	while(1)
	{
		no_of_bytes_read= fread(buffer,1,download_rate,fs);
 
		if(no_of_bytes_read > 0)
		{
			send(newSocket,buffer,no_of_bytes_read,0);
			download_done+=((no_of_bytes_read*100)/(float)total_bytes);
			printf("\nbuffer=%s downloading done.....%.2f %s",buffer,download_done,"%");
		}

		if(no_of_bytes_read < download_rate)
		{
			if(feof(fs))
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

	printf("\nfile downloaded in %lf seconds\n",exec_time);

	fclose(fs);
	
	// uploading by the client
	fp=fopen("uploaded.txt","w+");

	if(fp==NULL)
	{
		perror("error creating file!!!!");
		exit(0);
	}

	memset(buffer,'\0',sizeof(buffer));

	while(1)
	{
        if((bytes_received = read(newSocket, buffer, 256)) > 0)
        {
			printf("Bytes received %d buffer=%s\n",bytes_received,buffer);
			fwrite(buffer,1,bytes_received,fp);
        }
		if(bytes_received < 0)
        {
            perror("\n Read Error \n");
            break;
        }
        if(buffer[strlen(buffer)-2]=='#')
			break;
		
		memset(buffer,'\0',sizeof(buffer));
		sleep(1);
	}

	printf("\nfile uploading by client done successfully!!!\n");
	
	fclose(fp); 
	close(newSocket);
	return 0;
}