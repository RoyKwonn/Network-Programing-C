//
//  TCPEchoServer-Thread.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/23.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include "TCPEchoServer.h"
#include <pthread.h>

void *ThreadMain(void *arg);

// Structure of arguments to pass to client thread
struct ThreadArgs
{
    int clntSock;
};

int main(int argc, char *argv[])
{
    int servSock;
    int clntSock;
    unsigned short echoServPort;
    pthread_t threadID;
    struct ThreadArgs *threadArgs;
 
    if(argc != 2)
    {
        fprintf(stderr, "Usage : %s <SERVER PORT> \n", argv[0]);
        exit(1);
    }
    
    echoServPort = atoi(argv[1]);
    servSock = CreateTCPServerSocket(echoServPort);
    
    for(;;)
    {
        clntSock = AcceptTCPConnection(servSock);
        
        // Create separate memory for client argument
        if((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
            DieWithError("malloc() failed");
        
        threadArgs->clntSock = clntSock;
        
        // Create client thread
        if(pthread_create(&threadID, NULL, ThreadMain, (void *)threadArgs) != 0)
            DieWithError("pthread_create() failed");
        
        printf("with thread %ld\n", (long int) threadID);
    }
    // NOT REACHED
}

void *ThreadMain(void *threadArgs)
{
    int clntSock;
    
    // Guarantees that thread resources are deallocated upon return
    pthread_detach(pthread_self());
    
    // Extract socket file descriptor from argument
    clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
    free(threadArgs);
    
    HandleTCPClient(clntSock);
    
    return (NULL);
}
