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

#define SERVER_UDP_A_PORT 21695
#define SERVER_UDP_B_PORT 22695
#define SERVER_UDP_C_PORT 23695
#define SERVER_UDP_AWS_PORT 24695
#define SERVER_TCP_CLIENT_PORT 25695
#define SERVER_TCP_MONITOR_PORT 26695

#define localhost "127.0.0.1"

int main()
{
	

//defining monitor-aws server address
int tcp_monitor_socket;
if ((tcp_monitor_socket = socket(AF_INET,SOCK_STREAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}
printf("The AWS server is up and running. \n");
int monitor_socket;


struct sockaddr_in monitor_server_address;
monitor_server_address.sin_family = AF_INET;
monitor_server_address.sin_port = htons(SERVER_TCP_MONITOR_PORT);
monitor_server_address.sin_addr.s_addr = inet_addr(localhost);

int monitor_address_size = sizeof(monitor_server_address);

// Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, localhost, &monitor_server_address.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

int monitor_server_bind;
//bind the socket to our specified IP and port
if ((monitor_server_bind = bind(tcp_monitor_socket, (struct sockaddr *)&monitor_server_address, monitor_address_size))<0)
{
    perror("Could not bind monitor socket"); 
        exit(1);
}

//Listen
listen(tcp_monitor_socket, 10);

int recv_monitor;
//Accept
if ((monitor_socket = accept(tcp_monitor_socket , NULL, NULL))<0)
{
    perror("Could not accept monitor socket"); 
        exit(1);
}
int monitor_Message[2];
//receive
if ((recv_monitor = recv(monitor_socket, monitor_Message, sizeof(monitor_Message), 0))<0)
{
    perror("Could not receive monitor data"); 
        exit(1);
}


//defining UDP socket for AWS sewrver
int udp_socket;

if ((udp_socket = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}


struct sockaddr_in udp_socket_address;
udp_socket_address.sin_family = AF_INET;
udp_socket_address.sin_port = htons(SERVER_UDP_AWS_PORT);
udp_socket_address.sin_addr.s_addr = inet_addr(localhost);

int udp_aws_bind_socket;

//bind the socket to our specified IP and port(for AWS)
if ((udp_aws_bind_socket = bind(udp_socket, (struct sockaddr *)&udp_socket_address, sizeof(udp_socket_address)))<0)
{
    perror("Could not bind socket"); 
        exit(1);
}
int length_udp_aws= sizeof(udp_socket_address);

//defining socket for A
int udp_a_socket;
if ((udp_a_socket = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}

struct sockaddr_in udp_a_socket_address;
udp_a_socket_address.sin_family = AF_INET;
udp_a_socket_address.sin_port = htons(SERVER_UDP_A_PORT);
udp_a_socket_address.sin_addr.s_addr = inet_addr(localhost);

int length_udp_a= sizeof(udp_a_socket_address);

//defining socket for B
int udp_b_socket;
if ((udp_b_socket = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}

struct sockaddr_in udp_b_socket_address;
udp_b_socket_address.sin_family = AF_INET;
udp_b_socket_address.sin_port = htons(SERVER_UDP_B_PORT);
udp_b_socket_address.sin_addr.s_addr = inet_addr(localhost);

int length_udp_b= sizeof(udp_b_socket_address);

 //defining socket for c
int udp_c_socket;
if ((udp_c_socket = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}

struct sockaddr_in udp_c_socket_address;
udp_c_socket_address.sin_family = AF_INET;
udp_c_socket_address.sin_port = htons(SERVER_UDP_C_PORT);
udp_c_socket_address.sin_addr.s_addr = inet_addr(localhost);

int length_udp_c = sizeof(udp_c_socket_address);



 //Creation of client and AWS socket
int tcp_socket;
int tcp_bind;
int client_socket;

int recv_client;
int send_data;


if ((tcp_socket = socket(AF_INET,SOCK_STREAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}

//defining server address
struct sockaddr_in server_address;
server_address.sin_family = AF_INET;
server_address.sin_port = htons(SERVER_TCP_CLIENT_PORT);
server_address.sin_addr.s_addr = inet_addr(localhost);

int address_size = sizeof(server_address);


//bind the socket to our specified IP and port
if ((tcp_bind = bind(tcp_socket, (struct sockaddr *)&server_address, sizeof(server_address)))<0)
{
    perror("Could not bind socket"); 
        exit(1);
}


while(1)
{
listen(tcp_socket, 10);
float received_client_Message1[3];

//Accept
if ((client_socket = accept(tcp_socket , (struct sockaddr *) &server_address, &address_size))<0)
{
    perror("Could not accept client socket"); 
        exit(1);
}


if ((recv_client = recv(client_socket, received_client_Message1, sizeof(received_client_Message1), 0))<0)
{
    perror("Could not receive client data"); 
        exit(1);
}
int received_client_Message[2];
received_client_Message[0]= (int)received_client_Message1[0];


int link_id = received_client_Message[0];
int len = sizeof(link_id);

printf("The AWS received link ID=<%d>, size=<%.2f> and power=<%.2f> from the client using TCP over port <%d> \n", received_client_Message[0], received_client_Message1[1], received_client_Message1[2], SERVER_TCP_CLIENT_PORT);

int send_monitor_data;
if ((send_monitor_data= send(monitor_socket, &received_client_Message1 , sizeof(received_client_Message1), 0)<0))
{
	perror("Could not send monitor data"); 
        exit(1);
}
printf("The AWS sent link ID=<%d>, size=<%.2f> and power=<%.2f> to the monitor using TCP over port <%d> \n", received_client_Message[0], received_client_Message1[1], received_client_Message1[2], SERVER_TCP_MONITOR_PORT);



//  UDP sending and receiving
//sending and receiving from server A

int udp_send_serverA;
if((udp_send_serverA=sendto(udp_a_socket , &link_id, len ,0, (struct sockaddr *) &udp_a_socket_address, length_udp_a )<0))
{
        perror("Could not send to A"); 
        exit(1);
}
printf("The AWS sent link ID= <%d> to the Backend-Server <A> using UDP over port <%d>\n",received_client_Message[0],SERVER_UDP_A_PORT );
int udp_rcv_A;
float received_A_message[100];
if ((udp_rcv_A=recvfrom(udp_socket, &received_A_message, sizeof(received_A_message),0,(struct sockaddr*) &udp_socket_address,&length_udp_aws ))<0)
{
        perror("Could not send to A"); 
        exit(1);
}

//sending and receiving from server B 
int udp_send_serverB;
if((udp_send_serverB=sendto(udp_b_socket , &link_id, len ,0, (struct sockaddr *) &udp_b_socket_address, length_udp_b )<0))
{
        perror("Could not send to B"); 
        exit(1);
}
printf("The AWS sent link ID= <%d> to the Backend-Server <B> using UDP over port <%d>\n",received_client_Message[0],SERVER_UDP_B_PORT );
int udp_rcv_B;
float received_B_message[10];
if ((udp_rcv_B=recvfrom(udp_socket, &received_B_message, sizeof(received_B_message),0,(struct sockaddr*) &udp_socket_address,&length_udp_aws ))<0)
{
        perror("Could not send to B"); 
        exit(1);
}
int qw = (int)received_A_message[0];
int as = (int)received_B_message[0];
if (received_A_message[0] == 1)
{

	//found on Server A 
	printf(	"The AWS received <%d> matches from Backend Server A using UDP over port <%d> \n",qw, SERVER_UDP_A_PORT);

  	printf(	"The AWS received <%d> matches from Backend Server B using UDP over port <%d> \n",as, SERVER_UDP_B_PORT);

//setting all inputs to 1 array
float input[7];
input[0]= received_A_message[1];
input[1]= received_A_message[2]; 
input[2]= received_A_message[3];
input[3]= received_A_message[4];
input[4]= received_client_Message1[1];
input[5]= received_client_Message1[2];
input[6]=(float)received_client_Message[0];

	//send and receive from server C
int udp_send_serverC;
if((udp_send_serverC=sendto(udp_c_socket, &input, sizeof(input),0, (struct sockaddr *) &udp_c_socket_address, length_udp_c )<0))
   {
        perror("Could not send to C"); 
        exit(1);
   }
printf("The AWS sent link ID = <%d> size = <%.2f> , power <%.2f> and link information to backend server C using UDP over port <%d>\n", received_client_Message[0],received_client_Message1[1],received_client_Message1[2],SERVER_UDP_C_PORT);


 int udp_rcv_C;
float received_C_message[10];
if ((udp_rcv_C=recvfrom(udp_socket, received_C_message, sizeof(received_C_message),0,(struct sockaddr*) &udp_socket_address, &length_udp_aws ))<0)
   {
        perror("Could not rcv from server C\n"); 
        exit(1);
    }  
printf("The AWS received outputs from backend-server C using UDP over port <%d>\n",SERVER_UDP_C_PORT);

	//send the result to client
if (send_data = send(client_socket, &received_C_message , sizeof(received_C_message), 0)<0)
{
printf("There was an error in sending data. \n");
}

printf("The AWS sent detailed results to the monitor using TCP over port <%d>",SERVER_TCP_MONITOR_PORT);
printf("The AWS sent delay = <%.2f>ms to the client using the TCP over port <%d>  \n",received_C_message[2]  ,SERVER_TCP_CLIENT_PORT );

float monitor_final[4];
monitor_final[0]=1;
monitor_final[1]=received_C_message[0];
monitor_final[2]=received_C_message[1];
monitor_final[3]=received_C_message[2];
int send_monitor_data2;
if ((send_monitor_data2= send(monitor_socket, &monitor_final , sizeof(monitor_final), 0)<0))
{
	perror("Could not send monitor data"); 
        exit(1);
} 


}

if (received_B_message[0]==1)
  {//found on Server B
	printf(	"The AWS received <%d> matches from Backend Server A using UDP over port <%d> \n",qw, SERVER_UDP_A_PORT);

  	printf(	"The AWS received <%d> matches from Backend Server B using UDP over port <%d> \n",as, SERVER_UDP_B_PORT);

	printf("The AWS sent link ID = <%d> size = <%.2f> , power <%.2f> and link information to backend server C using UDP over port <%d>\n", received_client_Message[0],received_client_Message1[1],received_client_Message1[2],SERVER_UDP_C_PORT);

//setting all inputs to 1 array
float input[7];
input[0]= received_B_message[1];
input[1]= received_B_message[2]; 
input[2]= received_B_message[3];
input[3]= received_B_message[4];
input[4]= received_client_Message1[1];
input[5]= received_client_Message1[2];
input[6]= received_client_Message1[0];

  	//send and receive from server C
int udp_send_serverC;
if((udp_send_serverC=sendto(udp_c_socket, &input, sizeof(input),0, (struct sockaddr *) &udp_c_socket_address, length_udp_c )<0))
   {
        perror("Could not send to C"); 
        exit(1);
   }

 int udp_rcv_C;
float received_C_message[10];
if ((udp_rcv_C=recvfrom(udp_socket, received_C_message, sizeof(received_C_message),0,(struct sockaddr*) &udp_socket_address, &length_udp_aws ))<0)
   {
        perror("Could not rcv from server C\n"); 
        exit(1);
    }  
printf("The AWS received outputs from backend-server C using UDP over port <%d>\n",SERVER_UDP_C_PORT);
  	//send the result to client
      
if (send_data = send(client_socket, received_C_message , sizeof(received_C_message), 0)<0)
{
printf("There was an error in sending data. \n");
}

printf("The AWS sent detailed results to the monitor using TCP over port <%d>",SERVER_TCP_MONITOR_PORT);
printf("The AWS sent delay = <%.2f>ms to the client using the TCP over port <%d>  \n",received_C_message[2]  ,SERVER_TCP_CLIENT_PORT );

float monitor_final[4];
monitor_final[0]=1;
monitor_final[1]=received_C_message[0];
monitor_final[2]=received_C_message[1];
monitor_final[3]=received_C_message[2];
int send_monitor_data2;
if ((send_monitor_data2= send(monitor_socket, &monitor_final , sizeof(monitor_final), 0)<0))
{
	perror("Could not send monitor data"); 
        exit(1);
} 

}

  if(received_A_message[0]==0 && received_B_message[0] == 0	)
  {

printf("The AWS sent detailed results to the monitor using TCP over port <%d>",SERVER_TCP_MONITOR_PORT);
  	printf("\n The AWS sent No match to monitor and client using TCP over port <%d> and <%d> respectively\n",SERVER_TCP_MONITOR_PORT, SERVER_TCP_CLIENT_PORT );
        //send to client (CODE YET TO WRITE)  	
  		float monitor_final1[1];
monitor_final1[0]=0;
int send_monitor_data3;
if ((send_monitor_data3= send(monitor_socket, &monitor_final1 , sizeof(monitor_final1), 0)<0))
{
	perror("Could not send monitor data"); 
        exit(1);
}
  	float final[3] ;
  	final[0]= 0;
  	final[1]= 0;
  	final[2]= 0;
  	if (send_data = send(client_socket, &final , sizeof(final), 0)<0)
{
	perror("Could not send client last data"); 
        exit(1);
}  
}

   
}
return 0;
//closing the socket
close(udp_socket);
close(tcp_monitor_socket);
close(tcp_socket);
}
