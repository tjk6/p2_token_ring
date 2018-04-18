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
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include "server.h"

#define BUFFER_SIZE 1024

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(int argc, char* argv[]){
    //First, ensure that there are enough arguments
    if(argc < 2){
        printf("Not enough arguments.\n");
        printf("./server <PORT_NUMBER> <NUMBER_OF_HOSTS>\n");
        exit(1);
    }
    //Take the arguments and save them as variables
    int portNum = (int)strtol(argv[1], NULL, 10);
    int numHosts = (int)strtol(argv[2], NULL, 10);

    //Other values to be used
    int server_socket;                          //Socket for the server
    char buffer[BUFFER_SIZE];                   //Buffer for reading
    struct addrinfo hints, *server_info, *tempinfo;    //Stores info about the address
    int num_bytes;                              //Number of bytes in a packet message
    struct sockaddr_in server_sockaddr;
    struct sockaddr_in all_addr[numHosts];      //All addresses(using structs)      TODO: Use one of these.
    char *all_addr_str[numHosts];               //All addresses(in string form)
    socklen_t sender_len;                       //Length to be obtained
    char host_address[INET6_ADDRSTRLEN];

    //Initialize the addrinfo struct
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons((uint16_t)portNum);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if((server_socket = socket(PF_INET, SOCK_DGRAM,0)) == -1){
        printf("Could not establish socket.\n");
        return -1;
    }
    if(bind(server_socket,(struct sockaddr*)&server_sockaddr,sizeof(server_sockaddr)) < 0){
        printf("Error binding to the socket.\n");
        return -1;
    }




//    //Initialize the addrinfo struct
//    memset(&hints,0,sizeof(hints));
//    hints.ai_family = AF_INET;                  //IPv4
//    hints.ai_socktype = SOCK_DGRAM;             //This is UDP. Sending Datagrams.
//    hints.ai_protocol = 0;
//    hints.ai_flags = AI_PASSIVE;                //Use host IP
//    if(getaddrinfo(NULL, argv[1], &hints, &server_info) != 0){
//        printf("Could not get address info of this server: \n\t%s\n", strerror(errno));
//        return(1);
//    }
//
//    for(tempinfo = server_info; tempinfo != NULL; tempinfo = tempinfo->ai_next){
//        server_socket = socket(tempinfo->ai_family,tempinfo->ai_socktype,tempinfo->ai_protocol);
//        if(server_socket < 0){
//            printf("Could not open socket on this addrinfo\n");
//            continue;
//        }
//        if(bind(server_socket, tempinfo->ai_addr,tempinfo->ai_addrlen) < 0){
//            close(server_socket);
//            printf("Error in binding\n");
//            continue;
//        }
//        break;
//    }




//    char server_name[BUFFER_SIZE];
//    gethostname(server_name,BUFFER_SIZE-1);
//    printf("Server Address: <%s:",server_name);
//    printf("%d>\n", ntohs(((struct sockaddr_in*)server_info)->sin_port));
//    freeaddrinfo(server_info);
//    if(tempinfo == NULL){
//        printf("Could not bind...\n");
//        return 1;
//    }

    ///////////////////////////////////////////////////////////////////////////////////////////

    //define the address the message is coming from
    struct sockaddr_in sender_address;

//    struct sockaddr_storage host_addr_storage[numHosts];  //Stores the hosts addresses todo make sure this is correct
//    sender_len = sizeof(host_addr_storage);

//    char host_name[BUFFER_SIZE];
//    gethostname(host_name,BUFFER_SIZE - 1);

    char message[BUFFER_SIZE];

    sender_len = sizeof(struct sockaddr_in);
    num_bytes = (int)recvfrom(server_socket, message, BUFFER_SIZE,0,(struct sockaddr*)&all_addr[0],&sender_len);
    if(num_bytes < 0){
        printf("Could not get bytes.\n");
    }

    printf("Recieved: %s\n",message);

    printf("Address: <%s:",inet_ntoa((all_addr[0].sin_addr)));
    printf("%d>\n",htons(all_addr[0].sin_port));

    char addr_msg[BUFFER_SIZE];
    memset(addr_msg,0,BUFFER_SIZE-1);
    sprintf(addr_msg,"%s:%d",inet_ntoa(all_addr[0].sin_addr),htons(all_addr[0].sin_port));

    printf("Sending: %s\n", addr_msg);
    sendto(server_socket, addr_msg, sizeof(addr_msg),0,(struct sockaddr*)&all_addr[0],sizeof(all_addr[0]));


    /*
//    struct in_port_t host_address_id; //TODO: Undo if using host_address_id again
    int num_joined = 0;
    struct sockaddr *curr_sock_addr;

    //Get the ip addresses of all of the hosts
    while(num_joined < numHosts){
//        num_bytes = (int)recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (
//        struct sockaddr*)&host_addr_storage[num_joined], &sender_len);
        if(num_bytes < 0){
            printf("Error receiving bytes");
            close(server_socket);
            return(1);
        }

//        curr_sock_addr = (struct sockaddr*)&host_addr_storage[num_joined];  //TODO: Undo if using host_address_id again
//        host_address_id = (((struct sockaddr_in*)curr_sock_addr)->sin_port);
//        printf("Received packet from %s\n", inet_ntop(host_addr_storage[num_joined].ss_family, &host_address_id, host_address,sizeof(host_address)));

        memset(message,0,BUFFER_SIZE-1);
        sprintf(message,"You are user number %d",num_joined);
//        sendto(server_socket,message, sizeof(message),0,(struct sockaddr*)&host_addr_storage[num_joined], sizeof(struct sockaddr));

        num_joined++;
    }

    //Give the hosts the ip addresses of their neighbor to establish the ring
    num_joined = 0;
    char address_buffer[4096];
    int next_host;
    while(num_joined <  numHosts){
        next_host = (num_joined + 1) % numHosts;
//        sprintf(address_buffer, "%s", inet_ntop(host_addr_storage[next_host].ss_family, &host_address_id, host_address,sizeof(host_address))); //TODO: Undo if using host_address_id again
        printf("sending: %s",address_buffer);
//        sendto(server_socket,address_buffer, sizeof(address_buffer),0,(struct sockaddr*)&host_addr_storage[num_joined], sizeof(struct sockaddr));

        num_joined++;
    }


    */

    close(server_socket);
    return 0;
}
#pragma clang diagnostic pop
