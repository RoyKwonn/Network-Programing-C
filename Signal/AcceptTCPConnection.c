//
//  AcceptTCPConnection.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/23.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void DieWithError(char *errorMessage);

int AcceptTCPConnection(int servSock)
{
    int clntSock;
    struct sockaddr_in echoClntAddr;
    unsigned int clntLen;
    
    // Set the size of the in-out parameter
    clntLen = sizeof(echoClntAddr);
    
    // Wait for a client to connect
    if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
        DieWithError("accept() failed");
    
    // clntSock is connected to a client!
    printf("Handling client %s \n", inet_ntoa(echoClntAddr.sin_addr));
    
    return clntSock;
}
