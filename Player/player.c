//player.c

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
    Usage example, run these commands from the /Player directory: 
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

char defaultIP[17] = "127.0.0.1"; //default IP address
char defaultPort[6] = "58066"; //default Port

char msg_to_send[128];
char thisPLID[7];

int game_started = 0;

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
        defaultIP[i]=IP[i];
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
    if (atoi(Port)<58000 || atoi(Port)>59000){
        return 0;
    }
    for (int i = 0; i < strlen(Port); i++) {
        defaultPort[i] = Port[i];
    }
    return 1;
}

//USER INPUT
//getCommand(): reads a line from the stdin.
int getCommand (char *prmpt, char *buff, size_t sz) {
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

    // Otherwise remove newline and return
    buff[strlen(buff)-1] = '\0';
    return OK;
}

//validateInput(): returns 1 if valid, 0 if not. 
int validateInput(int rc, char *input){
    if (rc == TOO_LONG) {
        printf("Input too long\n");
        return 0;
    }
    
    char command[20];
    char arg[7];
    int space_found=0, space_index;

    for (int i=0; i<7; i++){
        arg[i]='\0';
    }

    int i=0;
    for (;i<strlen(input); ){
        if (input[i]==32){
            space_found=1;
            space_index=i;
        }
        if (space_found) arg[i-space_index-1]=input[i];
        i++;
    }
    if (space_found) strncpy(command, input, space_index);
    else{strncpy(command, input, i);}

    for (int i=0; i<128; i++){
        msg_to_send[i]='\0';
    }
    if (!strcmp("start", command) && space_found && !game_started){
        strcat(msg_to_send,"SNG ");
        strcat(msg_to_send, arg);

        for (int i=0; i<7; i++){
            thisPLID[i]='\0';
        }
        strcat(thisPLID, arg);
        game_started = 1;
        return 1;
    }

    if (game_started && !strcmp("quit", command)){
        strcat(msg_to_send,"QUT ");
        strcat(msg_to_send, thisPLID);
        return 1;
    }
    else{
        printf("There is no running game.\n");
    }

    return 0;
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

    if (argc>5){
        printf("Invalid argument format: invalid number of parameters\n");
        return 0;
    }
    switch (argc){
        case 2: printf("Invalid argument format: invalid number of parameters\n");
            return 0;
            break;

        case 3: n_flag=strcmp(argv[1],"-n");
            p_flag=strcmp(argv[1], "-p");
            if (n_flag!=0 && p_flag!=0) {
                printf("Invalid argument format: invalid options\n");
                return 0;
            }
            if (n_flag==0 && isIP(argv[2])!=0){}
            else if (p_flag==0 && isPort(argv[2])!=0){}
            else{
                printf("Invalid argument format: invalid parameter value\n");
                return 0;
            }
            break;

        case 4: printf("Invalid argument format: invalid number of parameters\n");
            return 0;
            break;

        case 5: n_flag=strcmp(argv[1],"-n");
            p_flag=strcmp(argv[1], "-p");

            if ((n_flag==0 && strcmp(argv[3], "-p")!=0) || 
            (p_flag==0 && strcmp(argv[3], "-n")!=0)){
                printf("Invalid argument format: invalid options\n");
                return 0;
            }
            
            if ((n_flag==0 && isIP(argv[2])!=0 && strcmp(argv[3], "-p")==0 && isPort(argv[4])!=0) || 
            (p_flag==0 && isPort(argv[2])!=0 && strcmp(argv[3], "-n")==0 && isIP(argv[4])!=0) ) {}
            else{
                printf("Invalid argument format: invalid parameter value\n");
                return 0;
            }
            break;
    }

    //USER INPUT
    /*
        While loop that handles user command input
    */

    int gC;
    char buff[100];

    while(1){
        gC = getCommand ("\nEnter command> ", buff, sizeof(buff));

        if (!strcmp(buff, "exit")){
            printf("\n- Bye! - \n");
            break;
        }
        
        if(!validateInput(gC, buff)){
            printf("Invalid command/format.\n");
            printf("To exit, write exit.\n");
            printf("See more commands and correct format in the project documentation (pagina da cadeira)\n");
        }
        else{
            printf("Sending message: [%s]\n\n",msg_to_send);
        }


        fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
        
        if(fd==-1) /*error*/exit(1);

        memset(&hints,0,sizeof hints);
        hints.ai_family=AF_INET; //IPv4
        hints.ai_socktype=SOCK_DGRAM; //UDP socket

        errcode=getaddrinfo(defaultIP,defaultPort,&hints,&res);
        if(errcode!=0) /*error*/ {
            printf("#1 Error. Try again later. \n");
            exit(1);
        }

        n=sendto(fd,msg_to_send,128,0,res->ai_addr,res->ai_addrlen);
        if(n==-1) /*error*/ exit(1);

        addrlen=sizeof(addr);
        n=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
        if(n==-1) /*error*/ exit(1);

        write(1, "Server echo: [", 14);
        write(1,buffer,n);
        write(1, "]\n", 2);


        freeaddrinfo(res);
        close(fd);

        if (!strcmp(buff, "quit")){
            printf("Game ended.\n");
            game_started = 0;
        }
    }

    return 0;
}