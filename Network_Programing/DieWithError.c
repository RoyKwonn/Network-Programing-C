//
//  DieWithError.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/04/25.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//


#include <stdio.h>      // for perror()
#include <stdlib.h>     // for exit()

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
