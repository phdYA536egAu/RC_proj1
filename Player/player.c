
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define PORT "58066"

int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints,*res;
struct sockaddr_in addr;
char buffer[128];

//#define invalid_format_msg= "./player [-n GSIP] [-p GSport], \nwhere:\nGSIP is the IP address of the machine where the game server (GS) runs. This is an optional argument. If this argument is omitted, the GS should be running on the same machine.\nGSport is the well-known port (TCP and UDP) where the GS accepts requests. This is an optional argument. If omitted, it assumes the value 58066.\n";

int isIP(char* IP){
    char num[4]="";
    int j=0;
    int num_bytes=0;
    for (int i = 0; i < strlen(IP); i++) {
        if (IP[i]!='.'){
            num[j]=IP[i];
            j++;
        }
        else{
            if (atoi(num)<0 || atoi(num)>255){
                return 0;
            }
            j=0;
            num_bytes++;
        }
    }
    if (num_bytes!=3){
        return 0;
    }
    
    return 1;
}

int main(int argc, char* argv[])
{
    switch (argc){
        case 2: printf("Invalid argument format: illegal number of parameters\n");
            break;

        case 3: if (strcmp(argv[1], "-n")!=0 && strcmp(argv[1], "-p")!=0) {
            printf("Invalid argument format: illegal options\n");
            }
            if (strcmp(argv[1], "-n ")!=0 && isIP(argv[2])!=0)
            {}
            else{ printf("Invalid argument format: illegal parameter value\n"); }
            
            break;

        case 4: break;

    }
    
    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) /*error*/exit(1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_DGRAM; //UDP socket
    errcode=getaddrinfo("127.0.0.1",PORT,&hints,&res);

    if(errcode!=0) /*error*/ exit(1);
    n=sendto(fd,"Hello!\n",7,0,res->ai_addr,res->ai_addrlen);

    if(n==-1) /*error*/ exit(1);
    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,128,0,
    (struct sockaddr*)&addr,&addrlen);
    if(n==-1) /*error*/ exit(1);
    write(1,"echo: ",6); write(1,buffer,n);

    freeaddrinfo(res);
    close(fd);

    return 0;
}