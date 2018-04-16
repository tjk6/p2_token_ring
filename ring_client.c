//
// Created by Tyler on 4/11/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <w32api/psdk_inc/_ip_types.h>
#include <cygwin/in.h>
#include <string.h>
#include "ring_client.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    if(argc < 4 || argc > 5){
        printf("Incorrect number of arguments.\n");
        printf("./client localhost <PORT_NUMBER> <HOST_PORT> <FILENAME>\n"); //TODO: change name later
        exit(1);
    }
    else if(argc == 4 && !strcmp(argv[5],"-new")){
        //send join request
    }

    int servPort = (int)strtol(argv[2], NULL, 10);

    //First, parse args about peer's port number, the IP address and port number of the server or
    // a peer in an existing token ring.


    int client_socket;
    client_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(client_socket < 0){
        printf("Error opening socket");
    }
    char buffer[BUFFER_SIZE];

    //Change this to take in the address given by the parse
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons((u_short)servPort);
    server_address.sin_addr.s_addr = INADDR_ANY;

    struct hostent *hp;

//    sendto(client_socket,"TOKEN", sizeof("TOKEN"),0, (
//    struct server_address), );


    int msgNum = -1;
    char* selection;
    char selectChar = ' ';
    while(selectChar != 'e'){
        sscanf("%s",selection);
        printf("Your selection: %s\n",selection);

        selectChar = selection[0];

        switch(selectChar){
            case 'w':
                //Write the message
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
                break;
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