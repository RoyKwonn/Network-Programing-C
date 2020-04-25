# Network Programing - C언어
## Made by Seokhwan Kwon

## follow command

```
gcc -c DieWithError.c
gcc -o TCPEchoClient.out TCPEchoClient.c DieWithError.o
gcc -c HandleTCPClient.c
gcc -o TCPEchoServer.out TCPEchoServer.c DieWithError.o HandleTCPClient.o
./TCPEchoServer.out 3000
./TCPEchoClient.out 127.0.0.1 Hi 3000
