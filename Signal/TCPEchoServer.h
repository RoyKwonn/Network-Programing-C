//
//  TCPEchoServer.h
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/23.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

/*
#ifndef TCPEchoServer_h
#define TCPEchoServer_h


#endif // TCPEchoServer_h
*/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void DieWithError(char *errorMessage);
void HandleTCPClient(int clntSocket);
int CreateTCPServerSocket(unsigned short port);
int AcceptTCPConnection(int servSock);
