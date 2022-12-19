
//RC LEIC-a
//Grupo 66

//Projeto 1

/*
    This user app interacts with the server according to the project
    requirements in order to play the Hangman game. This app is meant
    to be used by a player to play the game. 

    Using the commands specified in the project requirements, the user
    is able to play a Hangman game. 

    Structure:
    1. VALIDATION OF APP ARGUMENTS 
    2. USER INPUT

    Resources:
    Makefile. 
    Usage example, run these commands in the /Player directory: 
    make clean
    make
    ./player

    See project requirements for more info.
*/


#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints,*res;
struct sockaddr_in addr;
char buffer[128];

char usedIP[17] = "127.0.0.1"; //default IP address
char usedPort[6]="58066"; //default Port

//#define invalid_format_msg= "./player [-n GSIP] [-p GSport], \nwhere:\nGSIP is the IP address of the machine where the game server (GS) runs. This is an optional argument. If this argument is omitted, the GS should be running on the same machine.\nGSport is the well-known port (TCP and UDP) where the GS accepts requests. This is an optional argument. If omitted, it assumes the value 58066.\n";

//VALIDATION OF APP ARGUMENTS 
//isIP: returns 1 if an IP is valid, 0 otherwise.
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

//isPort: returns 0 if a Port is valid, 0 otherwise. 
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

//USER INPUT
//getLine: reads a line from the stdin.
static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If line is too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

int main(int argc, char* argv[])
{
    //VALIDATION OF APP ARGUMENTS 
    /* 
        Check if the arguments used when running the app are valid.
        This app uses the default IP and the default Port if they aren't
        specified in the arguments. 

        Supporting Methods
        int isIP(char* IP): returns 1 if an IP is valid, 0 otherwise.
        int isPort(char* Port): returns 0 if a Port is valid, 0 otherwise. 

    */

    int n_flag=-1, p_flag=-1;
    int quit_app=0;
    int rc;
    char buff[3];

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

    //USER INPUT
    /*
        While loop that handles user command input
    */

    while(quit_app != 1){
        rc = getLine ("Enter string> ", buff, sizeof(buff));
        if (rc == NO_INPUT) {
            // Extra NL since my system doesn't output that on EOF.
            printf("\nNo input\n");
            return 1;
        }

        if (rc == TOO_LONG) {
            printf("Input too long [%s]\n", buff);
            return 1;
        }

        printf("OK [%s]\n", buff);
        

        int str_size=0;
        for (; buff[str_size] != '\0'; ++str_size);
        printf("String len: %d", str_size);

        for(int l=0; l<str_size; ++l)
        printf("String: %c", buff[l]);

        fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
        if(fd==-1) /*error*/exit(1);

        memset(&hints,0,sizeof hints);
        hints.ai_family=AF_INET; //IPv4
        hints.ai_socktype=SOCK_DGRAM; //UDP socket
        errcode=getaddrinfo(usedIP,usedPort,&hints,&res);

        if(errcode!=0) /*error*/ exit(1);

        n=sendto(fd,buff,str_size,0,res->ai_addr,res->ai_addrlen);

        if(n==-1) /*error*/ exit(1);
        addrlen=sizeof(addr);
        n=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
        if(n==-1) /*error*/ exit(1);
        str_size=0;
        for (; buff[str_size] != '\0'; ++str_size);
        write(1,"echo: ",str_size+6); write(1,buffer,n);

        freeaddrinfo(res);
        close(fd);

        quit_app = 1;

    }

    return 0;
    
    

    


    

    return 0;
}