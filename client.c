#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <math.h>
#include <signal.h>

#define SERVER_AWS_PORT 25695 /*AWS server port*/
#define localhost "127.0.0.1"

int main(int argc, char *argv[])
{
float input[3];
//scanf("%d %d %d", &input[0], &input[1], &input[2]);



int tcp_server_socket;


input[0]=atof(argv[1]);
input[1]=atof(argv[2]);
input[2]=atof(argv[3]);

//Socket creation
if ((tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("Could not create client socket. \n");
        exit(1);
    }  
 
//Specify server address
struct sockaddr_in server_address;

server_address.sin_family = AF_INET;
server_address.sin_port = htons(SERVER_AWS_PORT);
server_address.sin_addr.s_addr = inet_addr(localhost);

// Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, localhost, &server_address.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }


int connection_status;
if(connection_status=connect(tcp_server_socket, (struct sockaddr *) &server_address, sizeof(server_address))<0)
{
    printf("There was an error making a connection to the AWS server. \n");
}
else {
    printf("The client is up and running. \n");
}
int data;
//Sending the data
if (data = send(tcp_server_socket, &input , sizeof(server_address), 0)<0)
{
printf("There was an error in sending data. \n");
}
int link = (int) input[0];
printf("The client sent link ID <%d>, size <%.2f>, and power <%.2f> to AWS \n", link, input[1], input[2]);


//receiving the result
int recv_data;
float output[3];
if ((recv_data= recv(tcp_server_socket, output, sizeof(output), 0))<0)


{
        perror("Could not receive message from server. \n");
        exit(1);
    } 

    if (output[2] != 0)
    {
    	printf("The delay for link <%d> is %.2f ms\n",link,output[2] );
       }
    else
    {
    	 printf("Found no matches for link <%d>\n",link);       
            }



//closing the socket
close(tcp_server_socket);


    return 0;
}
