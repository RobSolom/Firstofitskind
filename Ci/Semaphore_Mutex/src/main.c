#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

static unsigned int counter; /*shared resource*/
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char* readline(void);

void* incr_counter(void* p)
{
    do
    {
        usleep(10);
        pthread_mutex_lock(&mutex);
        counter++;
        fprintf(stdout,"%d\n",counter);
        sleep(1);
        pthread_mutex_unlock(&mutex);
    } while(1);

}
void* reset_counter(void* p)
{
    char* buf;
    int num = 0;
    int rc;
    pthread_mutex_lock(&mutex); /*block mutex just to show message*/
    printf("Enter the number and press 'Enter' to initialize counter with new value anytime.\n");
    sleep(3);
    pthread_mutex_unlock(&mutex);
    do{
        if((buf = readline()) == NULL)
        {   perror("realine()");
            break;
        }
        num = atoi(buf);
        if((rc = pthread_mutex_trylock(&mutex)) == EBUSY)
        {
            fprintf(stderr,"Mutex is already busy.\nLet's try to lock with pthread_mutex_lock().\n");
            pthread_mutex_lock(&mutex);
        }else if(rc == 0) 
        {
            printf("WOW! You are on time!\n");
        } else 
        {
            fprintf(stderr,"ERROR: %d\n",rc);
            break;
        }
        counter = num;
        printf("New values for counter: %d.\n",counter);
        pthread_mutex_unlock(&mutex);
    } while(1);
    
    free(buf); /* free memory allocated bu readline */

}

int main(void)
{    
    pthread_t thread_1;
    pthread_t thread_2;
    
    pthread_create(&thread_1,NULL,(void*)&incr_counter,NULL);
    pthread_create(&thread_2,NULL,(void*)&reset_counter,NULL);
    pthread_join(thread_2,NULL);
    
    return 0;
    
}


char* readline(void)
{
    char* src = (char*) malloc(64*sizeof(char));
    src = (char*) memset(src,'\0',64);
    ssize_t tmp;
    if(src == NULL)
    {
        perror("malloc()");
        return NULL;
    }
    if((tmp = read(0,src,1)) < 0)
    {
        perror("read()");
        return NULL;
    }
    
    *(src+tmp) = '\0';

    return (src-tmp);

}
