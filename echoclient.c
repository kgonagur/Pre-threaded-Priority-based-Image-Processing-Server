/*
gcc -g echoclient.c ../csapp.h ../csapp.c -lpthread -o client.out

gcc -g echoclient.c csapp.h csapp.c -lpthread -o client.out
*/

#include <stdlib.h>
#include <stdio.h>
#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, buf[18000];
    // int port;
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    // port = atoi(argv[2]);
    FILE *fr = fopen("godzilla.gif", "rb+");
    fread(buf, 18000, 1, fr);
    //fprintf("length: %d\n", strlen(buf));
    clientfd = Open_clientfd(host, argv[2]);
    
    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, buf, 18000);
    
    
    //while (Fgets(buf, MAXLINE, stdin) != NULL) {
       // Rio_writen(clientfd, buf, strlen(buf));
       // Rio_readlineb(&rio, buf, MAXLINE);
      //  Fputs(buf, stdout);
   // }
    fclose(fr);
    Close(clientfd);
    exit(0);

}

