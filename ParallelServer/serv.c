/*
 * Function:effective parallel server
 * Author:TBB
 * Time:21-3/31
 * Go
 * */ 
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<signal.h>
#define BAGS 5
#define MAXSIZE 1024

void err_quit(const char *message);//error handler function
void close_child_process(int sig);//signal handler function
int main(int argc,char *argv[]) 
{
	int servsock,clisock;//server socket & server exchange socket
	struct sockaddr_in servaddr,cliaddr;//as name
	pid_t pid;//process ID
    socklen_t socklen,tulen;//address length
    int str_len;
    char buf[MAXSIZE];//store message
    int tu = 1;
	
    /*check argument*/
	if(argc!=2)
	{
		printf("Usage:%s <port>\n",argv[0]);
		exit(1);
	}

	/*
	 *Function:signal handler part
	 *Description:killed ending child process
	 */
	struct sigaction s;//store signal handler function address and others
	s.sa_handler = close_child_process;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGCHLD,&s,0);//open signal

	/*create server socket*/
	if((servsock=socket(PF_INET,SOCK_STREAM,0))<0)
		err_quit("Socket Error!");
	
    /*to make port reuse*/
    tulen = sizeof(tu);
    setsockopt(servsock,SOL_SOCKET,SO_REUSEADDR,(void*)&tu,tulen);

	/*servaddr information*/
	memset(&servaddr,0,sizeof(servaddr));//to zero
	servaddr.sin_family = AF_INET;//address family
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	
	/*bind servsock and servaddr*/
	if(bind(servsock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
		err_quit("Bind Error!");

	/*servsock  start listen*/
      	if(listen(servsock,BAGS)<0)
		err_quit("Listen Error!");
	
	while(1)//death loop
    {
        socklen = sizeof(cliaddr);
        if((clisock=accept(servsock,(struct sockaddr *)&cliaddr,&socklen))<0)
            continue;//Necessary,to avoid accept error
        else
            printf("Client Connect!...\n");
        pid = fork();//create child process
        if(pid<0)//avoid error
        {  
            close(clisock);//Necessary
            continue;//to avoid fork error
        }
        if(pid==0)//child process will do
        {
            close(servsock);//necessary
            while((str_len=read(clisock,buf,MAXSIZE))!=0)
                if(write(clisock,buf,strlen(buf))>0);
            close(clisock);
            printf("Client Over!...\n");
            return 0;//Necessary,to return ending state
        }
        else
        {
            close(clisock);
        }
	}	
    close(servsock);
    return 0;
}

void err_quit(const char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
    }

void close_child_process(int sig)
{
    int state,pid;
    if((pid=waitpid(-1,&state,WNOHANG))>0)
        printf("Child Process ID:%d  Killed Successfully\n",pid);
}
