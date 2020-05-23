#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CHAR_SIZE 501 // 500바이트 + NULL = 501

void selectpk(int *p, int *q)
{
    int random;
    int i, j;
    int ck = -1, ck2 = 0;
    srand((unsigned)time(NULL));
    random = (rand() % 100); // 100까지 숫자 하나 고름
    
    for(i = random; 1; i++)
    {
        for(j = 2; j< i/2 +1; j++)
        {
            if(i%j == 0)
            {
                ck = 1;
                break;
            }
            ck = 0;
        }
        if (ck == 0 && ck2 == 1)
        {
            *q = i;
            break;
        }
        if (ck == 0 && ck2 == 0)
        {
            *p = i;
            ck2 = 1;
        }
    }// 해당 숫자 이상의 큰 소수 2개를 골라서 각각 p와 k로 사용
}

void selecte(int pi, int *e)
{
    int i, j, tmp = 1;
    
    for(i=2; i < pi; i++) // 1 < e < pi
    {
        for(j=1; j <= i; j++)
        {
            if(i%j == 0 && pi%j == 0) tmp = j;
        }
        if (tmp == 1) {
            *e = i;
            break;
        }
        tmp = 1;
    }
}

void selected(int e, int pi, int *d)
{
    int i;
    for(i=2; i<pi; i++) // 1 < d < pi
    {
        if((e*i)%pi == 1) *d = i; // e*d를 pi로 나누었을 때 1
    }
}



int main(int argc, char *argv[])
{
    int i,j;
    int sum=1;
    int word;
    char string[CHAR_SIZE]; // 입력받을 원문
    char string2[CHAR_SIZE] = {0, }; // 복호화할 파일에서 불러온 문자열
    
    int p, q; // 소수 2개 p, q
    int n;
    int pi;
    int e;
    int d;
    
    FILE *output, *input;
    output = fopen("./암호문.txt", "w"); // 암호화 된 파일
    
    selectpk(&p, &q); // p, q 적당한 소수 골라서 넣는 함수
    
    n = p*q;
    pi = (p - 1)*(q - 1);
    selecte(pi, &e); // e를 구하는 함수. e와 pi는 서로소. 1 < e < pi
    
    selected(e, pi, &d); // d를 구하는 함수. e*d를 pi로 나누었을 때 1이 나와야 함. 1 < d < pi
    
    // 여기까지 p, q를 골라 pi, e, d를 계산함.
    
    // 공개키 : n, e  비밀키 : n, d
    
    printf("암호화 할 원문을 500바이트 이내로 입력하세요 : ");
    gets(string);
    
    for(i=0; i < strlen(string); i++)
    {
        for(j=0; j < e; j++)
        {
            sum *= (int)string[i]; // c = m^e mod n
            sum %= n;
        }
        fprintf(output, "%d\n", sum);
        sum = 1;
    } // 암호화
    
    printf("암호화된 파일은 ./암호문.txt에 저장 \n");
    
    fclose(output); // 암호문 파일 종료
    
    fflush(stdin);
    
    printf("복호화를 하기위해 키 입력\n");
    getchar();
    
    // -----------------------------------------
    
    input = fopen("./암호문.txt", "r"); // 복호화할 파일(암호화된 파일)
    output = fopen("./원문.txt", "w"); // 복호화된 파일
    
    i = 0;
    while(!feof(input))
    {
        fscanf(input, "%d\n", &word);
        
        for(j=0; j < d; j++)
        {
            sum *= word; // m = c^d mod n
            sum %= n;
        }
        string2[i++] = (char)sum;
        sum = 1;
    }// 복호화
    
    fputs(string2, output);
    printf("복호화 된 파일은 ./원문.txt에 저장\n");
    
    fclose(output);
    fclose(input);
    fflush(stdin);
    
    getchar();
    
    return 0;
}
