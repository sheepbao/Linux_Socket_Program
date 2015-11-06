#if 0
	socket_server:
	sample linux local socket client communicate with another program. 
	by sheepbao
	2015.11.05
#endif

#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>   
#include <unistd.h>
#define Socket_File "/tmp/linux_socket"
#define BUFFER 1024

int main(int argc, char const *argv[])
{
	//build socket
	int socketfd=socket(PF_UNIX,SOCK_STREAM,0);
	if(socketfd < 0)
	{
		perror("can't build socket");
		return 1;
	}

	//connect socketfd
	struct sockaddr_un server_path;
	server_path.sun_family=AF_UNIX;
	strcpy(server_path.sun_path , Socket_File);

	int connect_sta=connect(socketfd , (struct sockaddr*)&server_path , sizeof(server_path));
	if(connect_sta==-1)
	{
		perror("can't connect socketfd");
		close(socketfd);
		return 1;
	} 

	//send data
	char send_data[BUFFER]={'\0'};
	strcpy(send_data,"hello!, I am from client. Nice to meet you!");
	int wlen=write(socketfd , send_data , sizeof(send_data));

	close(socketfd);
	return 0;
}