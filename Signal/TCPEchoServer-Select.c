//
//  TCPEchoServer-Select.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/23.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include "TCPEchoServer.h"
#include <sys/time.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int *servSock;
    int maxDescriptor;
    fd_set sockSet;
    long timeout;
    struct timeval selTimeout;
    int running = 1;
    int noPorts;
    int port;
    unsigned short portNo;
    
    if(argc < 3)
    {
        fprintf(stderr, "Usage : %s <Timeout (secs.)> <Port 1> ... \n", argv[0]);
        exit(1);
    }
    
    timeout = atol(argv[1]);
    noPorts = argc - 2;
    
    // Allocate list of sockets for incoming connections
    servSock = (int *)malloc(noPorts * sizeof(int));
    
    // Initialize maxDescriptor for use by select()
    maxDescriptor = -1;
    
    // Create list of ports and sockets to handle ports
    for(port = 0; port < noPorts; port++)
    {
        // Add port to port list
        portNo = atoi(argv[port + 2]);
        
        // Create port socket
        servSock[port] = CreateTCPServerSocket(portNo);
        
        // Determine if new descriptor is the largest
        if(servSock[port] > maxDescriptor)
            maxDescriptor = servSock[port];
    }
    
    printf("Starting server : Hit return to shutdown \n");
    
    while(running)
    {
        // Zero socket descriptor vector and set for server sockets
        // This must be reset every time select() is called
        FD_ZERO(&sockSet);
        
        // Add keyboard to descriptor vector
        FD_SET(STDIN_FILENO, &sockSet);
        
        for(port = 0; port < noPorts; port++)
            FD_SET(servSock[port], &sockSet);
        
        // Timeout specification
        selTimeout.tv_sec = timeout;
        selTimeout.tv_usec = 0;
        
        // Suspend program until descriptor is ready or timeout
        if(select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
            printf("No echo requests for %ld secs ... Server still alive \n", timeout);
        else
        {
            if(FD_ISSET(0, &sockSet))
            {
                printf("Shutting down server \n");
                getchar();
                running = 0;
            }
            
            for(port = 0; port < noPorts; port++)
            {
                if(FD_ISSET(servSock[port], &sockSet))
                {
                    printf("Request on port %d : ", port);
                    HandleTCPClient(AcceptTCPConnection(servSock[port]));
                }
            }
        }
    }
    
    // Close sockets
    for(port = 0; port < noPorts; port++)
        close(servSock[port]);
    
    // Free list of sockets
    free(servSock);
    
    exit(0);
}
