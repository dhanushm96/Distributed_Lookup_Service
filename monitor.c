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

#define SERVER_AWS_PORT 26695 /*AWS server port*/
#define localhost "127.0.0.1"
int main()
{

	int send_data;

int tcp_monitor_socket;

//Socket creation
if ((tcp_monitor_socket = socket(AF_INET, SOCK_STREAM, 0))<0)
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

    
    // //input= 1;
int connection_status;
if((connection_status=connect(tcp_monitor_socket, (struct sockaddr *) &server_address, sizeof(server_address)))<0)
{
    printf("There was an error making a connection to the AWS server. \n");
}
else {
    printf("The monitor is up and running. \n");
}

while(1)
{
	int input=2;
//Sending the data
if (send_data = send(tcp_monitor_socket, &input , sizeof(input), 0)<0)
{
printf("There was an error in sending data. \n");
}   


//receiving
int recv_data;
float x[3];
if ((recv_data= recv(tcp_monitor_socket, x, sizeof(x), 0))<0)
{
perror("error receing msg1");
}
printf("The Monitor received link Id=<%.1f> , size = <%.2f> and power <%.2f> from the AWS\n",x[0], x[1], x[2]);

int recv_data2;
float y[4];
if ((recv_data= recv(tcp_monitor_socket, y, sizeof(y), 0))<0)
{
perror("error receing msg2");
}
 if(y[0]==1)
{
	printf("The result for the link Id <%.1f>\n",x[0]);
 	printf("Tp = <%.2f> ms \n",y[1] );
 	printf("Tt = <%.2f> ms\n",y[2] );
 	printf("Delay = <%.2f> ms\n",y[3]);
}
else
 {
 	printf("Found no matches for link ID <%.1f>.\n",x[0]);
 	
 }

}
	return 0;
//closing the socket
	close(tcp_monitor_socket);
}