//server.c

//RC LEIC-a
//Grupo 66

//Projeto 1

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

char defaultPort[6]="58066"; //default Port
int udp_fd,udp_errcode;
ssize_t udp_n;
socklen_t udp_addrlen;
struct addrinfo udp_hints,*udp_res;
struct sockaddr_in udp_addr;
char udp_buffer[128];

int tcp_fd,newtcp_fd,tcp_errcode;
ssize_t tcp_n;
socklen_t tcp_addrlen;
struct addrinfo tcp_hints,*tcp_res;
struct sockaddr_in tcp_addr;
char tcp_buffer[128];

//VALIDATION OF APP ARGUMENTS 
//isVerboseParamValid: returns 0 if 3rd param is -v
int isVerboseParamValid(char* param){
    if (strcmp(param,"-v")){
        printf("Invalid argument format: invalid parameter value\n");
        printf("Try -v, instead of %s\n", param);
    }
    return strcmp(param,"-v");
}

//isPortParamValid: returns 0 if 3rd param is -p
int isPortParamValid(char* param){
    if (strcmp(param,"-p")){
        printf("Invalid argument format: invalid parameter value\n");
        printf("Try -p, instead of %s\n", param);
    }
    return strcmp(param,"-p");
}

//isPortNumParamValid: returns 0 if a Port is invalid, 1 otherwise. 
int isPortNumParamValid(char* param){
    for (int i = 0; i < strlen(param); i++) {
        if (param[i]<48 || param[i]>57){
            printf("Invalid argument format: invalid parameter value\n");
            printf("Invalid character, must be a number: %c\n", param[i]);
            return 0;
        }
    }
    if (atoi(param)<58000 || atoi(param)>59000){
        printf("Invalid argument format: invalid parameter value\n");
        printf("Invalid number, try between 58000 and 59000: %s\n", param);
        return 0;
    }
    for (int i = 0; i < strlen(param); i++) {
        defaultPort[i] = param[i];
    }
    return 1;
}

int main(int argc, char* argv[]){
    //VALIDATION OF APP ARGUMENTS 
    /* 
        Check if the arguments used when running the app are valid. This
        server uses the default Port if one isn't specified. 

    */

   if (argc>5){
        printf("Invalid argument format: invalid number of parameters\n");
    }
    switch (argc){
        case 2: printf("word file is %s\n", argv[1]);
            break;

        case 3:
            if (isVerboseParamValid(argv[2])){
                return 0;
            }
            break;

        case 4:
            if (isPortParamValid(argv[2])){
                return 0;
            }
            if (!isPortNumParamValid(argv[3])){
                return 0;
            }
            break;

        case 5:
            if (isPortParamValid(argv[2])){
                return 0;
            }
            if (!isPortNumParamValid(argv[3])){
                return 0;
            }
            if (isVerboseParamValid(argv[4])){
                return 0;
            }
            break;

    }

    //UDP SERVER
    /*
        
    */

    udp_fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(udp_fd==-1) /*error*/exit(1);

    memset(&udp_hints,0,sizeof udp_hints);
    udp_hints.ai_family=AF_INET; // IPv4
    udp_hints.ai_socktype=SOCK_DGRAM; // UDP socket
    udp_hints.ai_flags=AI_PASSIVE;

    udp_errcode=getaddrinfo(NULL,defaultPort,&udp_hints,&udp_res);
    if(udp_errcode!=0) /*error*/ exit(1);

    udp_n=bind(udp_fd,udp_res->ai_addr, udp_res->ai_addrlen);
    if(udp_n==-1) /*error*/ exit(1);

    while (1){
        udp_addrlen=sizeof(udp_addr);

        udp_n=recvfrom(udp_fd,udp_buffer,128,0,(struct sockaddr*)&udp_addr,&udp_addrlen);
        if(udp_n==-1)/*error*/exit(1);
        write(1,"received: ",10);
        write(1,udp_buffer,udp_n);

        udp_n=sendto(udp_fd,udp_buffer,udp_n,0,(struct sockaddr*)&udp_addr,udp_addrlen);
        if(udp_n==-1)/*error*/exit(1);
    }
    freeaddrinfo(udp_res);
    close(udp_fd);

    //TCP SERVER
    /*
        
    */




    return 0;
}