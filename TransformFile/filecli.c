
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define MAXSIZE 1024

void err_quit(const char *message);

int main(int argc,char *argv[])
{
		int sock,filesock;
		int len,sumlen;
		struct sockaddr_in servaddr;
		char mess[MAXSIZE];
		char site[50];
			
		if(argc!=3)
		{
				printf("Usage:%s <IP> <Port>\n",argv[0]);
				exit(1);
		}

		if((sock=socket(PF_INET,SOCK_STREAM,0))<0)
				err_quit("socket error");

		memset(&servaddr,0,sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr(argv[1]);
		servaddr.sin_port = htons(atoi(argv[2]));

		if(connect(sock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
				err_quit("connect error");

		fputs("Please input file way:",stdout);
		fgets(site,50,stdin);//输入末尾自动带上回车的'\n'以及'\0'
		write(sock,site,strlen(site));//未传输'\0'
		
		if((len=read(sock,mess,MAXSIZE))<0)//接受到'\0'为止，'\0'也会接受
				err_quit("read message error");
		printf("%d\n",len);	
		if(!strcmp(mess,"File Not Exist!\n"))
				printf("%s",mess);
		else
		{ 
				printf("The file data is ready to commit!\nPlease input store way:");
				fgets(site,50,stdin);
				site[strlen(site)-1] = '\0';//必须减一，将'\n'替换为'\0'
				
				if((filesock=open(site,O_CREAT|O_WRONLY|O_TRUNC))<0)
						err_quit("open error");
				
				write(filesock,mess,len);//将以'\0'结尾的文件内容写入文件
				
				printf("File accept over!\n");
				close(filesock);
		}
		close(sock);
}

void err_quit(const char *message)
{
		fputs(message,stderr);
		fputc('\n',stderr);
		exit(1);
}
