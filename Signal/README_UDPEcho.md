# UDP ECHO Program (Server & client)

## Follow this command

```
gcc -c DieWithError.c
gcc -o SigAction.out SigAction.c DieWithError.o
gcc -o UDPEchoServer-SIGIO.out UDPEchoServer-SIGIO.c DieWithError.o
gcc -o UDPEchoClient-Timeout.o UDPEchoClient-Timeout.c DieWithError.o
./UDPEchoServer-SIGIO.out 3000
./UDPEchoClient-Timeout.o 127.0.0.1 HI 3000
```