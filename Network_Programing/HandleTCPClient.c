//
//  HandleTCPClient.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/04/25.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;
    
    if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    
    while(recvMsgSize > 0)
    {
        if(send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");
        
        if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
    }
    
    close(clntSocket);
}
