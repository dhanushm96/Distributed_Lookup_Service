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

#define SERVER_UDP_B_PORT 22695
#define SERVER_UDP_AWS_PORT 24695

#define localhost "127.0.0.1"

int main()
{
//defining UDP socket for server B
int udp_socket_b;


	printf("The server is B up and running using UDP on port <%d> \n", SERVER_UDP_B_PORT);

if ((udp_socket_b = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}

struct sockaddr_in udp_b_socket_address;
udp_b_socket_address.sin_family = AF_INET;
udp_b_socket_address.sin_port = htons(SERVER_UDP_B_PORT);
udp_b_socket_address.sin_addr.s_addr = inet_addr(localhost);

int udp_b_length = sizeof(udp_b_socket_address);
int udp_bind_socket_b;

//bind the socket to our specified IP and port
if ((udp_bind_socket_b = bind(udp_socket_b, (struct sockaddr *)&udp_b_socket_address, sizeof(udp_b_socket_address)))<0)
{
    perror("Could not bind socket"); 
        exit(1);
}

int udp_socket_aws;
if ((udp_socket_aws = socket(AF_INET,SOCK_DGRAM,0))<0)
{
        perror("Could not create socket"); 
        exit(1);
}
//defining aws port

struct sockaddr_in udp_aws_socket_address;
udp_aws_socket_address.sin_family = AF_INET;
udp_aws_socket_address.sin_port = htons(SERVER_UDP_AWS_PORT);
udp_aws_socket_address.sin_addr.s_addr = inet_addr(localhost);

int udp_aws_length = sizeof(udp_aws_socket_address);
while(1)
	{

//receiving msg from server AWS
int received_message;
int recv_b;
if ((recv_b= recvfrom(udp_socket_b, &received_message,sizeof(received_message),0, (struct sockaddr*) &udp_b_socket_address, &udp_b_length))<0)
{
	perror("couldnt receive d message");
	exit(1);
}

printf("The server B received input :  ");

char input_int[1024];

sprintf(input_int,"%d",received_message);


//searching link ID in .csv file

char input_string [1024];
float match_found_in_file = 0;
strcpy(input_string,input_int);
puts(input_string);
char character[1024], temp[1024]= {'\0'};
char a[100],b[100],c[100],d[100];
int x=0,y;
FILE *fp = fopen("database_b.csv","r");
if(fp == NULL) {
  perror("Error opening file");
  return(-1);
}


   while ( fgets (character, 1024, fp)!=NULL ) {
   		x=0;

      	while (character[x] != ',') {
      		temp[x] = character[x];
      		x++;
      	}
      	

      	if ( strcmp(input_string, temp) == 0){
      		
      		match_found_in_file = 1;
      		break;
      	}
      	else {
      		for (y=0;y<x;y++)
      			temp[y]='\0';
      	}
      }
  char temp_2[50];
  int start_index = x+1;
  int flag = 0, index_of_values = 0;

  for (y=start_index; y< strlen(character); y++) {
  	if (character[y] == ',' || character[y] == '\n'){
  		temp_2[index_of_values] = '\0';
  		index_of_values = 0;

  		if (flag == 0)
  			strcpy(a,temp_2);
  		else if (flag == 1)
  			strcpy(b, temp_2);
  		else if (flag == 2)
  			strcpy(c,temp_2);
  		else if (flag == 3)
  			strcpy(d, temp_2);

  		flag++;
  		continue;
  	}
  	temp_2[index_of_values] = character[y];
  	index_of_values++;
   }
   float found[5];
if (match_found_in_file==1)
{

found[1] = atof(a);
found[2] = atof(b);
found[3] = atof(c);
found[4] = atof(d);
int t = (int)match_found_in_file;
printf("The Server B has found <%d> match\n", t);
}
else
{
int t = (int)match_found_in_file;
printf("The Server B has found <%d> match \n", t);
}
	fclose(fp);

found[0] = match_found_in_file;



  printf ("The server A finished sending the output to AWS \n");

	int udp_send_serverB;
if((udp_send_serverB =sendto(udp_socket_aws , &found, sizeof(found) ,0, (struct sockaddr *) &udp_aws_socket_address, udp_aws_length))<0)
	{
         perror("Could not send to AWS"); 
         exit(1);
    }
}
  return 0;
  //closing the socket
  close(udp_socket_b);

	}