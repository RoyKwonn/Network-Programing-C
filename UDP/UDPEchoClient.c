//
//  UDPEchoClient.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/20.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>          // for printf() and fprintf()
#include <sys/socket.h>     // for socket(), connect(), sendto(), and recvfrom()
#include <arpa/inet.h>      // for sockaddr_in and inet_addr()
#include <stdlib.h>         // for atoi() and exit()
#include <string.h>         // for memset()
#include <unistd.h>         // for close()



#define ECHOMAX 255         // Longest string to echo

void DieWithError(char *errorMessage);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in fromAddr;
    unsigned short echoServPort;
    unsigned int fromSize;
    char *servIP;
    char *echoString;
    char echoBuffer[ECHOMAX + 1];
    int echoStringLen;
    int respStringLen;
    
    if((argc < 3) || (argc > 4))    // Test for correct number of arguments
    {
        fprintf(stderr, "Usage : %s <Server IP> <Echo Word> [<Echo Port>] \n", argv[0]);
        exit(1);
    }
    
    servIP = argv[1];
    echoString = argv[2];
    
    if((echoStringLen = strlen(echoString)) > ECHOMAX)      // Chec input length
        DieWithError("Echo word too long");
    
    if(argc == 4)
        echoServPort = atoi(argv[3]);
    else
        echoServPort = 7;
    
    // Create a datagram/UDP socket
    
    if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
    
    
    // Construct the server address structure
    
    memset(&echoServAddr, 0, sizeof(echoServAddr));     // Zero out structure
    echoServAddr.sin_family = AF_INET;                  // Internet addr family
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   // Server IP address
    echoServAddr.sin_port = htons(echoServPort);        // Server port
    
    
    // Send the string to the server
    
    if(sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
        DieWithError("sendto() sent a different number of bytes than expected");
    
    
    // Recv a response
    
    fromSize = sizeof(fromAddr);
    if((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize)) != echoStringLen)
        DieWithError("recvfrom() failed");
    
    if(echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
    {
        fprintf(stderr, "Error : recevied a packet from unknown source. \n");
        exit(1);
    }
    
    // null-terminate the received data
    
    echoBuffer[respStringLen] = '\0';
    printf("Received : %s\n", echoBuffer);      // Print the echoed arg
    
    close(sock);
    exit(0);
        
    
    return 0;
}
