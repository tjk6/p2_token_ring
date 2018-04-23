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
int get_next_addr(int, struct sockaddr_in *, struct sockaddr_in *);
void* menu_func(void*);

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
    struct sockaddr_in this_addr, server_address, neighbor_addr;
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
    recvfrom(server_socket,response,sizeof(response),0,(struct sockaddr *)&server_address,&serveraddr_len);
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
        if((int)htons(neighbor_addr.sin_port) == (int)htons(this_addr.sin_port)){
            printf("Break!\n");
            break;
        }
        if(token == 1){
            //Send the token to the next client
            if(is_done == 1){
                sprintf(this_msg,"%d:%d",host_port,htons(neighbor_addr.sin_port));
                sendto(this_socket,this_msg,sizeof(this_msg),0,(struct sockaddr *)&neighbor_addr,sizeof(neighbor_addr));
                break;
            }
            else{
                sprintf(this_msg,"0:0");
                sendto(this_socket,neighbor_msg,sizeof(neighbor_msg),0,(struct sockaddr *)&neighbor_addr,sizeof(neighbor_addr));
            }
            token = 0;
        }
        else{
            //Wait to receive the token. When message is received, token becomes 1
            memset(neighbor_msg,0,sizeof(neighbor_msg));
            recvfrom(this_socket,neighbor_msg,sizeof(neighbor_msg),0,NULL,&serveraddr_len);
            int portno = (int)strtol(strtok(neighbor_msg,":"),NULL,10);
            int next_port = (int)strtol(strtok(NULL,"\0"),NULL,10);
            if((int)htons(neighbor_addr.sin_port) == portno ){
                neighbor_addr.sin_port = htons((uint16_t)next_port);
                portno = 0;
                next_port = 0;
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
    printf("Made it to the function at least...\n");
    char *filename = (char*)filename_ptr;
    printf("Filename: %s\n",filename);
    FILE *fp = fopen(filename,"a+");
    if(fp == NULL){
        printf("Could not open file\n");
    }
    int msgNum = -1;
    char selection[BUFFER_SIZE];
    char selectChar = ' ';
    char message[LRG_BUFFER_SIZE];
    while(1){
        printf("Enter your selection. For help, type h\n");
        scanf("%s",selection);
        printf("Your selection: %s\n",selection);

        selectChar = selection[0];

        switch(selectChar){
            case 'w':
                //Write the message
                printf("Enter your message\n");
                if(token == 1){
                    sprintf(message,"<message n=%d>",msgNum);
                    fgets(message, LRG_BUFFER_SIZE, stdin);
                    scanf("%s",message);
                    sprintf(message,"</message>");
                    printf("Your message: %s",message);
                    fputs(message, fp);
                }
                break;
            case 'r':
                msgNum = selection[2];
                //Get the message
                break;
            case 'l':
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