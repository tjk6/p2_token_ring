//
// Created by Tyler on 4/11/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <cygwin/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "ring_client.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define BUFFER_SIZE 1024
#define LRG_BUFFER_SIZE 4096

int create_sock();
void init_addr(struct sockaddr_in *, char *, uint16_t);
int countlines(FILE*);
void* menu_func(void*);
void print_msg_at_loc(FILE*, int);

int token, is_done;

int main(int argc, char *argv[]){
    if(argc < 5 || argc > 6){
        printf("Incorrect number of arguments.\n");
        printf("./client localhost <PORT_NUMBER> <HOST_PORT> <FILENAME>\n"); //TODO: change name later
        exit(1);
    }
    else if(argc == 5){
        //send join request
    }
    else if(argc == 6){
        //Do the -new thing
    }

    printf("Args: <%s> <%s> <%s> <%s>\nArgc: %d\n",argv[1],argv[2],argv[3],argv[4],argc);

    //First, parse args about peer's port number, the IP address and port number of the server or
    // a peer in an existing token ring.
    int server_port = (int)strtol(argv[2], NULL, 10);
    int host_port = (int)strtol(argv[3],NULL,10);
    char filename[BUFFER_SIZE];
    memset(filename,0,sizeof(filename));
    strcpy(filename,argv[4]);
    int this_socket, server_socket;
    struct sockaddr_in this_addr, server_address, neighbor_addr, new_addr;
    int serveraddr_len = sizeof(struct sockaddr_in);
    pthread_t menu_thread;

    //Initialize the address this program will run on and bind
    init_addr(&this_addr,"127.0.0.1",(uint16_t)host_port);
    this_socket = create_sock(); //TODO: Error handling.
    if(bind(this_socket,(struct sockaddr *)&this_addr,sizeof(this_addr)) < 0){
        printf("Could not bind.\n");
    }

    //Initialize the socket this program will connect to the server with.
    init_addr(&server_address, "127.0.0.1", (uint16_t)server_port);
    server_socket = create_sock();

    //Send the address of the host port to the server
    char port_str[BUFFER_SIZE];
    memset(port_str,0,BUFFER_SIZE-1);
    sprintf(port_str,"%d",host_port);
    sendto(server_socket,port_str,sizeof(port_str),0,(struct sockaddr *)&server_address,sizeof(server_address));

    //Get the response from the server and close the socket
    char response[BUFFER_SIZE];
    memset(response,0,BUFFER_SIZE-1);
    printf("Waiting...\n");
    recvfrom(server_socket,response,sizeof(response),0,(struct sockaddr *)&server_address,&serveraddr_len);
    printf("Received\n");
    close(server_socket);

    //Parse the address that the neighbor will be using and initialize the address
    char* addr = strtok(response,":");
    char* port = strtok(NULL,":");
    token = (int)strtol(strtok(NULL,":"),NULL,10);
    init_addr(&neighbor_addr,addr,(uint16_t )strtol(port,NULL,10));

    char neighbor_msg[BUFFER_SIZE];
    memset(neighbor_msg,0,sizeof(neighbor_msg));
    char this_msg[BUFFER_SIZE];
    memset(this_msg,0,sizeof(this_msg));


    /////////////////////TESTING TO SEE IF CONNECTION WAS ESTABLISHED/////////////////////

    sprintf(this_msg,"%d", host_port);
    printf("Has token?: %d\n",token);

    //Gets previous port number
    sendto(this_socket,this_msg,sizeof(this_msg),0,(struct sockaddr *)&neighbor_addr,sizeof(neighbor_addr));
    recvfrom(this_socket,neighbor_msg,sizeof(neighbor_msg),0,NULL,&serveraddr_len);

    printf("This port: %d\n",htons(this_addr.sin_port));
    printf("Next port: %d\n",htons(neighbor_addr.sin_port));
    printf("Prev port: %s\n",neighbor_msg);

    //////////////////////////////////////////////////////////////////////////////////////
    is_done = 0;
    int leaving = 0;

    sprintf(neighbor_msg,"0:0");
    pthread_create(&menu_thread,NULL,menu_func,&filename);
    while(1){
        sleep(1);
        memset(this_msg,0,sizeof(this_msg));
        sprintf(this_msg,"0:0");
        if((int)htons(neighbor_addr.sin_port) == (int)htons(this_addr.sin_port)){
            printf("You're the last host. Select e to exit.\n");
            break;
        }
        if(token == 1){
            //Send the token to the next client
            if(is_done == 1){
                sprintf(this_msg,"%d:%d",host_port,htons(neighbor_addr.sin_port));
                printf("Sending %s to %d\n",this_msg,htons(neighbor_addr.sin_port));
                sendto(this_socket,this_msg,sizeof(this_msg),0,(struct sockaddr *)&neighbor_addr,sizeof(neighbor_addr));
                break;
            }
            else{
                sprintf(this_msg,"0:0");
                printf("Sending %s to %d\n",this_msg,htons(neighbor_addr.sin_port));
                sendto(this_socket,neighbor_msg,sizeof(neighbor_msg),0,(struct sockaddr *)&neighbor_addr,sizeof(neighbor_addr));
            }
            token = 0;
        }
        else{
            //Wait to receive the token. When message is received, token becomes 1
//            memset(neighbor_msg,0,sizeof(neighbor_msg));
            recvfrom(this_socket,neighbor_msg,sizeof(neighbor_msg),0,NULL,&serveraddr_len);
            printf("Received:%s\n",neighbor_msg);


            int portno = -1;
            char* portno_ptr = strtok(neighbor_msg,":");
            if(portno_ptr != NULL){
                portno = (int)strtol(portno_ptr,NULL,10);
                printf("portno_ptr is %d\n",portno);
            }
            int next_port = -1;
            char* next_port_ptr = strtok(NULL,"\0");
            if(next_port_ptr != NULL){
                next_port = (int)strtol(next_port_ptr,NULL,10);
                printf("next_port_ptr is %d\n",next_port);
            }
//            printf("Got past the assignment\n");
//            printf("%d:%d\n",portno,next_port);
//            if(next_port == -1){
//                printf("Received port: %d\n",portno);
//                memset(this_msg,0,sizeof(this_msg));
//                sprintf(this_msg,"%s:%d:0","127.0.0.1",(int)htons(this_addr.sin_port));
//                printf("Message to new peer: %s\n",this_msg);
//                printf("Sending to: %d\n",(int)htons(new_addr.sin_port));
//                sendto(this_socket,this_msg,sizeof(this_msg),0,(struct sockaddr *)&new_addr,sizeof(new_addr));
//                next_port = portno;
//                portno = host_port;
//            }

            printf("%d comparting to %d\n",(int)htons(neighbor_addr.sin_port),portno);
            if((int)htons(neighbor_addr.sin_port) == portno ){
                printf("match!\n");
                neighbor_addr.sin_port = htons((uint16_t)next_port);
//                portno = 0;
//                next_port = 0;
            }
            sprintf(neighbor_msg,"%d:%d",portno,next_port);
            token = 1;
        }
    }
    pthread_join(menu_thread, NULL);
    printf("Joined successful!\n");

    close(this_socket);
    return(0);
}

int create_sock(){
    int sock;
    if((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0){
        printf("Could not create socket.\n");
        return -1;
    }
    return sock;
}

void init_addr(struct sockaddr_in *addr_struct, char *ip_addr, uint16_t port_num){
    //Address that this port will be using
    addr_struct->sin_family = AF_INET;
    addr_struct->sin_addr.s_addr = inet_addr(ip_addr);
    addr_struct->sin_port = htons(port_num);
}

void* menu_func(void* filename_ptr){
    char *filename = (char*)filename_ptr;
    printf("Filename: %s\n",filename);
    FILE *fp = fopen(filename,"a+");
    if(fp == NULL){
        printf("Could not open file\n");
    }
    int msgNum = -1;
    char selection[BUFFER_SIZE];
    char selectChar = ' ';
    char input_msg[LRG_BUFFER_SIZE];
    char message[LRG_BUFFER_SIZE];
    while(1){
        printf("Enter your selection. For help, type h\n");
        scanf(" %[^\n]",selection);
        printf("Your selection: %s\n",strtok(selection," \n"));

        selectChar = selection[0];

        switch(selectChar){
            case 'w':
                //Write the message TODO: PUT INTO A FUNCTION LATER
                printf("Enter your message\n");
                fflush(stdin);
                scanf(" %[^\n]",input_msg);
                while(token == 0); //Wait for the token to be 1
                fprintf(fp,"<message n=%d>%s</message>\r\n",countlines(fp) + 1,input_msg);
                fflush(fp);
                break;
            case 'r':
                while(token == 0);
                char* num_str;
                if((num_str = strtok(NULL," \n")) != NULL){
                    msgNum = (int)strtol(num_str,NULL,10); //TODO: Change to a tokenized string
                    printf("Getting message number %d\n",msgNum);
                    print_msg_at_loc(fp, msgNum);
                }
                else{
                    printf("No value given for read. Use r <number> where <number> is the specific line you want to read.\n");
                }
                //Get the message
                break;
            case 'l':
                while(token == 0);
                printf("Messages in range: 0-%d\n",countlines(fp));
                //Get the number of the most recent message and print it as the range.
                break;
            case 'e':
                //Exit the ring.
                is_done = 1;
                fclose(fp);
                return 0;
            case 'h':
                printf("To write a new message, type w\n");
                printf("To read a particular message, type r and then space, then the value of the message\n");
                printf("    (Example, to read message #4, type: r 4)\n");
                printf("To display a numeric range of the messages available, type l\n");
                printf("To exit, type e\n");
                break;
            default:
                printf("Invalid input. For help, type h.");
                break;
        }
    }

}
#pragma clang diagnostic pop

int countlines(FILE* fp){
    int ch;
    int msg_num = 0;
    rewind(fp);
    for(ch = getc(fp); ch != EOF; ch = getc(fp)){
        if(ch == '\n'){
            msg_num++;
        }
    }
    return msg_num;
}

void print_msg_at_loc(FILE* fp, int messageno){
    int line_num = 1;
    char message[LRG_BUFFER_SIZE];
    rewind(fp);

    while(fgets(message, sizeof(message), fp) != NULL){
        if(line_num == messageno){
            printf("%s\n", message);
            return;
        }
        line_num++;
    }
}