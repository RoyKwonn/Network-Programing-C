//
//  BroadcastSender.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/25.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void DieWithError(char *errorMessage);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in broadcastAddr;
    char *broadcastIP;
    unsigned short broadcastPort;
    char *sendString;
    int broadcastPermission;
    unsigned int sendStringLen;
    
    if (argc < 4)
    {
        fprintf(stderr,"Usage: %s <IP Address> <Port> <Send String> \n", argv[0]);
        exit(1);
    }
    
    broadcastIP = argv[1];
    broadcastPort = atoi(argv[2]);
    sendString = argv[3];

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Set socket to allow broadcast */
    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST , (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0)
        DieWithError("setsockopt() failed");

    /* Construct local address structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);
    broadcastAddr.sin_port = htons(broadcastPort);

    sendStringLen = strlen(sendString);

    for (;;) /* Run forever */
    {
        /* Broadcast sendString in datagram to clients every 3 seconds*/
        if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
            DieWithError("sendto() sent a different number of bytes than expected");
        sleep(3);

    }
}
