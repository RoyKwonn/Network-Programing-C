//
//  CreateTCPServerSocket.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/23.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXPENDING 5

void DieWithError(char *errorMessage);

int CreateTCPServerSocket(unsigned short port)
{
    int sock;
    struct sockaddr_in echoServAddr;
    
    // Create socket for incoming connections
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    
    // Construct local address structure
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(port);
    
    // Bind to the local address
    if(bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
    
    // Mark the socket so it will listen for incoming connections
    if(listen(sock, MAXPENDING) < 0)
        DieWithError("listen() failed");
    
    return sock;
}
