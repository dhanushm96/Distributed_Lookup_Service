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
#include <float.h>


#define SERVER_UDP_C_PORT 23695
#define SERVER_UDP_AWS_PORT 24695

#define localhost "127.0.0.1"

int main()
{
//defining UDP socket for server C
int udp_socket_c;

printf("The server C is up and running using UDP on port <%d> \n", SERVER_UDP_C_PORT);

if ((udp_socket_c = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}

struct sockaddr_in udp_c_socket_address;
udp_c_socket_address.sin_family = AF_INET;
udp_c_socket_address.sin_port = htons(SERVER_UDP_C_PORT);
udp_c_socket_address.sin_addr.s_addr = inet_addr(localhost);


//defining aws port

int udp_socket_aws;
if ((udp_socket_aws = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}


struct sockaddr_in udp_aws_socket_address;
udp_aws_socket_address.sin_family = AF_INET;
udp_aws_socket_address.sin_port = htons(SERVER_UDP_AWS_PORT);
udp_aws_socket_address.sin_addr.s_addr = inet_addr(localhost);

int udp_aws_length = sizeof(udp_aws_socket_address);



int udp_bind_socket_c;
int udp_c_length = sizeof(udp_c_socket_address);
//bind the socket to our specified IP and port
if ((udp_bind_socket_c = bind(udp_socket_c, (struct sockaddr *)&udp_c_socket_address, sizeof(udp_c_socket_address)))<0)
{
    perror("Could not bind socket"); 
        exit(1);
}
while(1)
{
//receiving msg from server AWS
float input[100];
int recv_c;
if ((recv_c= recvfrom(udp_socket_c, &input, sizeof(input),0, (struct sockaddr*) &udp_c_socket_address, &udp_c_length))<0)
{
	perror("couldnt receive d message");
	exit(1);
}
int q = (int)input[6];

printf("The server C reveived link information of link <%d>, file size <%.2f> and signal power <%.2f>\n ",q,input[4],input[5] );

//computation

float p_new;
float transmission_delay, prop_delay;
float a = log(2);

float s_n = pow(10,((0.1*input[5])-(0.1*input[3])));
float SNR = 1+ s_n;
float k= log(SNR);
float B = pow(10,6)*input[0];
//Shannon-Hartley theorem
float c = (B*k)/a;
printf("The server C finished the calculation of link <%d>\n",q );


//end to end delay
transmission_delay = (input[4]*1000)/c;
prop_delay = (input[1]*1000*1000)/(input[2]*10000000);
float end_end_delay= (transmission_delay+prop_delay);
float delay[3];
delay[0]=prop_delay;
delay[1]=transmission_delay;
delay[2]= end_end_delay;
float delay_size = sizeof(delay);
int udp_send_aws;
if((udp_send_aws=sendto(udp_socket_aws , &delay, delay_size ,0, (struct sockaddr *) &udp_aws_socket_address, udp_aws_length))<0)
	{
         perror("Could not send to AWS"); 
         exit(1);
    }
printf("The Server C finished sending the output to AWS\n");
}
	return 0;
	//closing the socket
close(udp_socket_c);

}
