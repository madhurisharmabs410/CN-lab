#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define PERMS0666
char fname[256];

int main()
{
	size_t n;
	char buff[512];
	int readfd,writefd;
	printf("trying to connection to server\n");
	writefd=open(FIFO1,O_WRONLY,0);
	readfd=open(FIFO2,O_RDONLY,0);
	printf("connected\n");
	printf("enter the filename to request from server:");
	scanf("%s",fname);
	write(writefd,fname,strlen(fname));
	printf("waiting for server to reply..\n");
	while((n=read(readfd,buff,512))>0)
		write(1,buff,n);
	close(readfd);
	unlink(FIFO1);
	close(writefd);
	unlink(FIFO2);
	return 0;
}
