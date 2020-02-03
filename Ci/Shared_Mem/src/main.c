#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define SHARED_MEMORY_OBJECT_NAME  "shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE  128
#define SHM_CREATE 1
#define SHM_PRINT 3
#define SHM_CLOSE 4
void usage(const char* s)
{
	fprintf(stderr,"Usage: %s [print] [create] [write] [read] [close] 'text\n'",s);
}
int main(int argc, char** argv)
{
	int shm, len, cmd, mode = 0;
	char* addr;
	if(argc < 2 )
	{
		usage(argv[0]);
		return -1;
	}

	if((!strcmp(argv[1],"create") || !strcmp(argv[1],"write")) && (argc == 3))
	{
		len = strlen(argv[2]);
		len = (len <= SHARED_MEMORY_OBJECT_SIZE ? len: SHARED_MEMORY_OBJECT_SIZE);
		mode = O_CREAT;
		cmd = SHM_CREATE;
	}else if(!strcmp(argv[1],"print"))
	{
		cmd = SHM_PRINT;
	}else if(!strcmp(argv[1],"close"))
	{
		cmd = SHM_CLOSE;
	} else
	{
		usage(argv[0]);
		return -1;
	}

	if((shm = shm_open(SHARED_MEMORY_OBJECT_NAME, mode|O_RDWR,0777)) == -1)
	{	
		perror("shm_open()");
		return -1;
	}

	if(cmd == SHM_CREATE)
	{
		if(ftruncate(shm, SHARED_MEMORY_OBJECT_SIZE+1) == -1)
		{
			perror("ftrancate()");
			return -1;
		}
	}
	
	addr = mmap(0,SHARED_MEMORY_OBJECT_SIZE+1,PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
	if(addr == (char*)-1)
	{
		perror("mmap()");
		return -1;

	}
	
	switch(cmd){
	
	case SHM_CREATE:
		memcpy(addr,argv[2],len);
		addr[len] = '\0';
		fprintf(stdout,"Shared memory filled in. Run %s print to see values.\n",argv[0]);
		break;
	case SHM_PRINT:
		fprintf(stdout,"Got from shared memory: %s\n",addr);
		break;
	default:
		fprintf(stderr,"FUCK YOU BITCH\n");
		break;
	}
	munmap((void*) addr,(size_t) SHARED_MEMORY_OBJECT_NAME);
	close(shm);
	if(cmd == SHM_CLOSE)
	{
		shm_unlink(SHARED_MEMORY_OBJECT_NAME);
	}
	return 0;
}
