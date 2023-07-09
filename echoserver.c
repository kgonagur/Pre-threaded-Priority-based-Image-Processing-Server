/* 
 * echoservert_pre.c - A prethreaded concurrent echo server
 */
/* $begin echoservertpremain */
#include "csapp.h"
#include "sbuf.h"
#include <time.h>
#define NTHREADS  4
#define SBUFSIZE  16

void echo(int connfd);
void *threadTask(void *vargp);

sbuf_t sbuf; /* shared buffer of connected descriptors */

int main(int argc, char **argv) 
{
    int i, listenfd, connfd, port, minPriority, mainThreadPriority, policy;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sched_param mainThreadParam;
    struct sockaddr_in clientaddr;
    pthread_t tid; 

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    /* MAIN-THREAD WITH LOW PRIORITY */
	mainThreadParam.sched_priority = sched_get_priority_max(SCHED_FIFO);
	mainThreadPriority = mainThreadParam.sched_priority;
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &mainThreadParam);
	pthread_getschedparam (pthread_self(), &policy, &mainThreadParam);


    port = atoi(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE); //line:conc:pre:initsbuf
    listenfd = Open_listenfd(argv[1]);

    for (i = 0; i < NTHREADS; i++)  /* Create worker threads */ //line:conc:pre:begincreate
	    Pthread_create(&tid, NULL, threadTask, NULL);               //line:conc:pre:endcreate

    while (1) { 
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }
}

void *threadTask(void *vargp) 
{
    pthread_t threadId = pthread_self();
    struct sched_param param;
    time_t start_time, cur_time;
    int delay = 3;
    int priority;

    param.sched_priority = sched_get_priority_min(SCHED_FIFO);
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
	// ret = pthread_getschedparam (threadId, &policy, &param);
	priority = param.sched_priority;	
	printf("Thread %ld priority: %d \n", threadId, priority);
    
    Pthread_detach(threadId);
    printf("Thread %ld waiting for %d seconds before servicing requests\n", threadId, delay);
    time(&start_time);
    do
    {
        time(&cur_time);
        // printf("cur_time: %ld\n", cur_time);
    }
    while((cur_time - start_time) < delay);

    while (1) { 
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */ //line:conc:pre:removeconnfd
        echo(connfd);                /* Service client */
        Close(connfd);
        
        printf("Thread %ld waiting for %d seconds before servicing next requests\n", threadId, delay);
        time(&start_time);
        do
        {
            time(&cur_time);
            // printf("cur_time: %ld\n", cur_time);
        }
        while((cur_time - start_time) < delay);
    }
}
/* $end echoservertpremain */
