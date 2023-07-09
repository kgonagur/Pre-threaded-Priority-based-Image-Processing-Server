/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
#include <stdio.h>
#include <time.h>

void echo(int connfd) 
{
    time_t seconds;
  
     // Stores time seconds
    time(&seconds);
    size_t n; 
    char buf[18000]; 
    char result[20],gif[]=".gif";
    
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    sprintf(result, "%d", (int)seconds);
    
    n=rio_readnb(&rio,buf,18000);
    printf("server received %d bytes\n", (int)n);
    FILE *fw = fopen(strcat(result,gif), "wb+");
    fwrite(buf, 1, 18000, fw);
    //while((n = Rio_readlineb(&rio, buf, 18000)) != 0) { //line:netp:echo:eof
	//printf("server received %d bytes\n", (int)n);
	//Rio_writen(connfd, buf, n);
    //}
    fclose(fw);
}
/* $end echo */

