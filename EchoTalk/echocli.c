#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#define MAXSIZE 1024

void err_quit(const char * message);//自定义函数声明

int main(int argc,char *argv[])
{
		int sock;//与服务端进行通信的套接字
		struct sockaddr_in servaddr;//保存服务器端地址信息
		char mess[MAXSIZE];
		int stlen,getlen,midlen;

		if((sock=socket(PF_INET,SOCK_STREAM,0))<0)//建立客户端套接字
				err_quit("socket error");

		memset(&servaddr,0,sizeof(servaddr));//服务器端地址信息置零
		servaddr.sin_family = AF_INET;//设置地址族
		inet_aton(argv[1],&servaddr.sin_addr);//设置服务器端IP
		servaddr.sin_port = htons(atoi(argv[2]));//设置服务器端端口号

		if(connect(sock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)//与服务端进行连接
				err_quit("connect error");

		while(1)
		{
				fputs("Please input message:",stdout);
				fgets(mess,MAXSIZE,stdin);//从标准输入获取数据
				if(!strcmp(mess,"q\n") || !strcmp(mess,"Q\n"))
						break;//用break比用exit(1)更合理
				stlen = write(sock,mess,strlen(mess));//获取传输的数据的长度,此时不包含'\0'
				getlen = 0;
				while(getlen<stlen)//之所以要如此是因为tcp数据传输不存在数据边界
				{ 
						midlen = read(sock,&mess[getlen],MAXSIZE-1);//要减1是因为我们要在末尾手动添加'\0'
						if(midlen<0)//避免异常
								err_quit("read error");
						getlen += midlen;

				}
				mess[getlen] = '\0';
				printf("The message from serv:%s",mess);

		}
		close(sock);//关闭套接字
		return 0;
}

void err_quit(const char *message)
{
		fputs(message,stderr);
		fputc('\n',stderr);
		exit(1);
}
