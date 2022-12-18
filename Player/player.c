
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints,*res;
struct sockaddr_in addr;
char buffer[128], usedIP[17] = "127.0.0.1", usedPort[6]="58066";

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
    for (int i = 0; i < strlen(IP); i++) {
        usedIP[i]=IP[i];
    }
    return 1;
}

int isPort(char* Port){
    for (int i = 0; i < strlen(Port); i++) {
        if (Port[i]<48 || Port[i]>57){
            return 0;
        }
    }
    if (atoi(Port)<2 || atoi(Port)>65535){
        return 0;
    }
    for (int i = 0; i < strlen(Port); i++) {
        usedPort[i] = Port[i];
    }
    return 1;
}

int main(int argc, char* argv[])
{
    int n_flag=-1, p_flag=-1;
    if (argc>5){
        printf("Invalid argument format: illegal number of parameters\n");
    }
    switch (argc){
        case 2: printf("Invalid argument format: illegal number of parameters\n");
            break;

        case 3: n_flag=strcmp(argv[1],"-n");
            p_flag=strcmp(argv[1], "-p");
            if (n_flag!=0 && p_flag!=0) {
                printf("Invalid argument format: illegal options\n");
            }
            if (n_flag==0 && isIP(argv[2])!=0){}
            else if (p_flag==0 && isPort(argv[2])!=0){}
            else{
                 printf("Invalid argument format: illegal parameter value\n");
            }
            break;

        case 4: printf("Invalid argument format: illegal number of parameters\n");
            break;

        case 5: n_flag=strcmp(argv[1],"-n");
        p_flag=strcmp(argv[1], "-p");
        if ((n_flag==0 && strcmp(argv[3], "-p")!=0) || 
        (p_flag==0 && strcmp(argv[3], "-n")!=0)){
            printf("Invalid argument format: illegal options\n");
        }
        if ((n_flag==0 && isIP(argv[2])!=0 && strcmp(argv[3], "-p")==0 && isPort(argv[4])!=0) || 
        (p_flag==0 && isPort(argv[2])!=0 && strcmp(argv[3], "-n")==0 && isIP(argv[4])!=0) 
        ) {
        }
        else{
            printf("Invalid argument format: illegal parameter value\n");
        }
        break;
    }
    
    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) /*error*/exit(1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_DGRAM; //UDP socket
    errcode=getaddrinfo(usedIP,usedPort,&hints,&res);

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