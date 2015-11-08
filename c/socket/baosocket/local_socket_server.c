#if 0
	socket_server:
	sample linux local socket server communicate with another program. 
	by sheepbao
	2015.11.05
#endif

#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>   
#include <unistd.h>
#define Socket_File "/tmp/linux_socket"
#define MAX_CLIENT 5
#define BUFFER 1024*512

int main(int argc, char const *argv[])
{
	int socketfd;
	//build socket 
	socketfd=socket(PF_UNIX,SOCK_STREAM,0);
	if (socketfd<0)
	{
		perror("can't build socket");
		return 1;
	}
	//bind socket file description
	#if 0
	struct sockaddr_un {
	    sa_family_t sun_family;/* Address Family */
	    char         sun_path[108]; /* Pathname */
	};
	#endif

	struct sockaddr_un server_path;
	server_path.sun_family=AF_UNIX;
	strcpy(server_path.sun_path,Socket_File);

	int bind_sta=bind(socketfd,(struct sockaddr*)&server_path , sizeof(server_path));

	if (bind_sta == -1 )
	{
		perror("bind failed");
		unlink(Socket_File);
		close(socketfd);
		return 1;
	}

	//listen socket FD
	int listen_sta=listen(socketfd , MAX_CLIENT);
	printf("listen!\n");
	if(listen_sta == -1)
	{
		perror("can't listen");
		unlink(Socket_File);
		close(socketfd);
		return 1;
	}

	//accept socketfd
	int addlen=(int)sizeof(server_path);
	int accept_fd=accept(socketfd , (struct sockaddr*)&server_path , (socklen_t*)&addlen);
	if(accept_fd < 0 )
	{
		perror("can't acceptfd" );
		unlink(Socket_File);
		close(socketfd);
		return 1;
	}

	//read data or recv or recvmsg
	char rec_data[BUFFER]={'\0'};
	char file[BUFFER]={'\0'};
	int len=1;
	int filelen=0;
	while (len != 0)
	{
		int read_sta=read(accept_fd , rec_data , sizeof(rec_data));
		memcpy(file,rec_data,read_sta);
		len=read_sta;
		filelen+=read_sta;

	}
	printf("receive data from another program " );

	FILE *fw;
	fw=fopen("rec.file","w");
	fwrite(file,sizeof(char),filelen,fw);
	fclose(fw);

	
	close(socketfd);
	close(accept_fd);
	unlink(Socket_File);
	return 0;
}