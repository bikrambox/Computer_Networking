// this program is a file transfer program for UDP server client
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
#include <time.h>

int main(){

	int welcomeSocket, newSocket,bytes_received=0, download_rate=0, no_of_bytes_read=0, total_bytes=0;

	float download_done=0.0;
	double exec_time;
	clock_t start, end;
	char buffer[1024],filename[256], ch, temp[5];

	struct sockaddr_in serverAddr,clientAddr;
	socklen_t addr_size;

	// create socket
	welcomeSocket = socket(PF_INET, SOCK_DGRAM, 0);

	// creating sockaddr_in structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Bind
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	addr_size = sizeof clientAddr;

	memset(filename,'\0',sizeof(filename));

	recvfrom(welcomeSocket,filename,256,0,(struct sockaddr *)&clientAddr, &addr_size);
	printf("recieved filename=%s\n",filename);

	FILE *fp,*fs;

	// Downloading by the client

	fs=fopen(filename,"r");

	if(fs==NULL)
	{
		perror("error opening file!!!!");
		exit(1);
	}
 
	printf("file is present at the server side!!!\n");

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

	printf("\n\nenter the file download rate(in bytes):");
	scanf("%d",&download_rate);

	memset(buffer,'\0',sizeof(buffer));

	start=clock();	

	while(1)
	{ 
		no_of_bytes_read= fread(buffer,1,download_rate,fs);	
		sprintf(temp,"%d",strlen(buffer));

		sendto(welcomeSocket,temp,5,0,(struct sockaddr *)&clientAddr,addr_size);
 
		if(no_of_bytes_read > 0)
		{
			sendto(welcomeSocket,buffer,no_of_bytes_read,0,(struct sockaddr *)&clientAddr,addr_size);         	
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
        //read(newSocket, buffer, 256)
		if((bytes_received=recvfrom(welcomeSocket,buffer,256,0,(struct sockaddr *)&clientAddr, &addr_size)) > 0)
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
	close(welcomeSocket);

	return 0;
}

