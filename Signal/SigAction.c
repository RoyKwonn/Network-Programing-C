//
//  SigAction.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/06/21.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>      // for printf()
#include <signal.h>     // for sigaction()
#include <unistd.h>     // for pause()
#include <stdlib.h>     // for exit()

void DieWithError(char *errorMessage);          // Error handling function
void InterruptSignalHandler(int signalType);    // Interrupt signal(ctrl + c) handling function


int main(int argc, char *argv[])
{
    struct sigaction handler; // sigaction 구조체의 멤버는 void (* sa_handler)(int); sigset_t sa_mask; int sa_flags; 총 3가지다
    
    // Set InterruptSignalHandler() as handler function
    handler.sa_handler = InterruptSignalHandler; // 함수 주소 넣어주기
    
    // Create mask that mask all signals
    if(sigfillset(&handler.sa_mask) < 0)    // 모두 마스크하라 = 모두 블락킹하라
        DieWithError("sigfillset() failed");
    
    // NO flags
    handler.sa_flags = 0;
    
    // Set signal handling for interrupt signals
    if(sigaction(SIGINT, &handler, 0) < 0)
        DieWithError("sigaction() failed");
    
    for(;;)
        pause();        // suspend program until signal received
    exit(0);
}

/*
 void InterruptSignalHandler(int signalType) // 컨트롤 C가 한번 출력된다.
 {
     printf("Interrupt Received. \n");
     sleep(3);
 }
*/
 
 // 수정
void InterruptSignalHandler(int ignored) // 컨트롤 C가 두번 출럭된다.
{
    printf("Interrupt Received. \n");
    sleep(3);
}
