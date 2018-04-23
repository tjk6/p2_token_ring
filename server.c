//
// Created by Tyler on 4/9/2018.
//
#include <stdio.h>          //Standard library
#include <stdlib.h>         //Standard library
#include <sys/socket.h>     //API and definitions for the sockets
#include <sys/types.h>      //more definitions
#include <netinet/in.h>     //Structures to store address information
#include <unistd.h>         //Closes the socket
#include <sys/stat.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include "server.h"

#define BUFFER_SIZE 1024

int init_sock(struct sockaddr_in *, uint16_t);

int main(int argc, char* argv[]){
    //First, ensure that there are enough arguments
    if(argc < 2){
        printf("Not enough arguments.\n");
        printf("./server <PORT_NUMBER> <NUMBER_OF_HOSTS>\n");
        exit(1);
    }
    //TODO: Make sure numHosts > 1
    //Take the arguments and save them as variables
    int portNum = (int)strtol(argv[1], NULL, 10);
    int numHosts = (int)strtol(argv[2], NULL, 10);
    if(numHosts < 2){
        printf("Not enough hosts to establish ring\n");
        exit(1);
    }

    //Other values to be used
    int server_socket;                          //Socket for the server
    int num_bytes;                              //Number of bytes in a packet message
    int curr_client;                            //Keeps track of the current client
    int next_client;                            //Keeps track of the next client
    struct sockaddr_in server_sockaddr;
    struct sockaddr_in all_addr[numHosts];      //All addresses collected from clients
    char message[BUFFER_SIZE];                  //Buffer that the host port string goes into
    char all_addr_str[numHosts][BUFFER_SIZE];   //All actual client addresses
    socklen_t sender_len;                       //Length to be obtained

    //Initialize the addrinfo struct and bind to socket
    server_socket = init_sock(&server_sockaddr,(uint16_t)portNum);
    if(bind(server_socket,(struct sockaddr*)&server_sockaddr,sizeof(server_sockaddr)) < 0){
        printf("Error binding to the socket.\n");
        printf("%s\n",strerror(errno));
        return -1;
    }

    /////////////////////////////WHILE LOOP FOR ESTABLISHING RING/////////////////////////////
    sender_len = sizeof(struct sockaddr_in);

    curr_client = 0;
    while(curr_client < numHosts){
        num_bytes = (int)recvfrom(server_socket, message, BUFFER_SIZE,0,(struct sockaddr*)&all_addr[curr_client],&sender_len);
        if(num_bytes < 0){
            printf("Could not get bytes.\n");
        }

        memset(all_addr_str[curr_client],0,BUFFER_SIZE-1);

        //Makes sure the first value starts with the token.
        if(curr_client == numHosts-1){
            sprintf(all_addr_str[curr_client],"%s:%s:1",inet_ntoa(all_addr[curr_client].sin_addr),message);
        }
        else{
            sprintf(all_addr_str[curr_client],"%s:%s:0",inet_ntoa(all_addr[curr_client].sin_addr),message);
        }
        curr_client++;
    }
    //////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////PRINTS ALL STORED ADDRESSES FOR TESTING PURPOSES/////////////////////
    printf("All stored addresses:\n");
    for(curr_client=0;curr_client<numHosts;curr_client++){
        next_client = (curr_client + 1) % numHosts;
        printf("   \"%s\" from <%d>\n",all_addr_str[curr_client],ntohs(all_addr[curr_client].sin_port));
        sendto(server_socket, all_addr_str[curr_client], sizeof(all_addr_str[curr_client]),0,(struct sockaddr*)&all_addr[next_client],sizeof(all_addr[next_client]));
    }
    //////////////////////////////////////////////////////////////////////////////////////////

    close(server_socket);
    return 0;
}


int init_sock(struct sockaddr_in *addr_struct, uint16_t port_num){
    int server_socket;

    //Address that this port will be using
    addr_struct->sin_family = AF_INET;
    addr_struct->sin_addr.s_addr = htonl(INADDR_ANY);
    addr_struct->sin_port = htons(port_num);
    if((server_socket = socket(PF_INET, SOCK_DGRAM,0)) < 0){
        printf("Could not establish socket.\n");
        return -1;
    }
    return server_socket;
}
