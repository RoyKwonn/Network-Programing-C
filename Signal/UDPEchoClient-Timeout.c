//
//  UDPEchoClient-Timeout.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/22.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>          // for printf() and fprintf()
#include <sys/socket.h>     // for socket(), connect(), sendto(), and recvfrom()
#include <arpa/inet.h>      // for sockaddr_in and inet_addr()
#include <stdlib.h>         // for atoi() and exit()
#include <string.h>         // for memset()
#include <unistd.h>         // for close()
#include <errno.h>          // for errno and EINTR
#include <signal.h>         // for sigaction()


#define ECHOMAX         255         // Longest string to echo
#define TIMEOUT_SECS    2           // Seconds between retransmits
#define MAXTRIES        5           // Tries before giving up

int tries = 0;  // Count of times sent - GLOBAL for signal-handler access

void DieWithError(char *errorMessage);  // Error handling function
void CatchAlarm(int ignored);           // Handler for SIGALRM

int main(int argc, char *argv[])
{
    int sock;   // Socket descriptor
    struct sockaddr_in echoServAddr;
    struct sockaddr_in fromAddr;
    unsigned short echoServPort;
    unsigned int fromSize;
    struct sigaction myAction;
    char *servIP;
    char *echoString;
    char echoBuffer[ECHOMAX+1];
    int echoStringLen;
    int respStringLen;
    
    if((argc < 3) || (argc > 4))
    {
        fprintf(stderr, "Usage : %s <Server IP> <Echo Word> [<Echo Port] \n", argv[0]);
        exit(1);
    }
    
    servIP = argv[1];
    echoString = argv[2];
    
    if((echoStringLen = strlen(echoString)) > ECHOMAX)
        DieWithError("Echo word too long");
    
    if(argc == 4)
        echoServPort = atoi(argv[3]);
    else
        echoServPort = 7;
    
    // Create a best-effort datagram socket using UDP
    if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
    
    // Set signal handler for alarm signal
    myAction.sa_handler = CatchAlarm;
    
    if(sigaction(SIGALRM, &myAction, 0) < 0)
        DieWithError("sigaction() failed for SIGALRM");
    
    // Construct the server address structure
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port = htons(echoServPort);
    
    // Send the string to the server
    if(sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
        DieWithError("sendto() sent a different number of bytes than expected");
    
    // Get a response
    fromSize = sizeof(fromAddr);
    alarm(TIMEOUT_SECS);
    while((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize)) < 0)
        if(errno == EINTR)
        {
            if(tries < MAXTRIES)
            {
                printf("timed out, %d more tires ... \n", MAXTRIES-tries);
                if(sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
                    DieWithError("sendto() failed");
                alarm(TIMEOUT_SECS);
            }
            else
                DieWithError("No Response");
        }
        else
            DieWithError("recvfrom() failed");
    
    // recvfrom() got something -- cancel the timeout
    alarm(0);
        
    // null-terminate the received data
    echoBuffer[respStringLen] = '\0';
    printf("Received : %s\n", echoBuffer);
        
    close(sock);
    exit(0);
}

void CatchAlarm(int ignored)
{
    tries += 1;
}
