#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define NAMEDPIPE "/tmp/named_pipe"
#define BUFSIZE 128

int main(void)
{
	int fd, len;
	char buf[BUFSIZE];

	//FILE* f_io = fopen()
	if( remove(NAMEDPIPE) < 0)
		perror("remove()");

	if(mkfifo(NAMEDPIPE,0777))
	{
		perror("mkfifo()");
		return -1;
	}
	fprintf(stdout,"%s was created successfully\n",NAMEDPIPE);

	if((fd = open(NAMEDPIPE,O_RDONLY)) <= 0)
	{
		perror("open()");
		return -1;
	}
	fprintf(stdout,"%s is opened\n",NAMEDPIPE);
	do{
		 memset(buf,'\0',BUFSIZE-1);
		if((len = read(fd,buf,BUFSIZE-1)) <= 0)
		{	perror("read()");
			close(fd);
			remove(NAMEDPIPE);
			return 0;
		}
		fprintf(stdout,"Incoming message (%d): %s\n",fd,buf);			
	}
	while(1);
}
