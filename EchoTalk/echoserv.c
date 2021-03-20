#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#define BAG 15 //队列长度
#define MAXSIZE 1024

void err_quit(const char * message);

int main(int argc,char *argv[])
{
		int servsock,clisock;//服务器端套接字与通信套接字
		struct sockaddr_in servaddr,cliaddr;//服务器端地址与客户端地址
		socklen_t addrlen;//客户端地址信息长度
		char mess[MAXSIZE];//保存转发数据
		int size;
		if(argc!=2)
		{
				printf("Usage:%s <port>\n",argv[0]);
				exit(1);
		}

		if((servsock = socket(PF_INET,SOCK_STREAM,0))<0)//建立服务器端套接字
				err_quit("connect error");

		memset(&servaddr,0,sizeof(servaddr));//置零
		servaddr.sin_family = AF_INET;//设置地址族
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//设置任意IP都可以请求连接
		servaddr.sin_port = htons(atoi(argv[1]));//设置端口号

		if(bind(servsock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)//绑定套接字与地址信息
				err_quit("bind error");
		
		if(listen(servsock,BAG)<0)//服务器端套接字进入监听状态，等待连接队列
				err_quit("listen error");
		
		addrlen = sizeof(cliaddr);
		for(;;)
		{
				if((clisock = accept(servsock,(struct sockaddr *)&cliaddr,&addrlen))<0)//从等待队列中取队头进行连接
						err_quit("accept error");
				
				while((size = read(clisock,mess,MAXSIZE))>0)//转发回客户端
						write(clisock,mess,size);

				close(clisock);//关闭进行通信的套接字

		}
		close(servsock);//关闭服务器端套接字
		return 0;
}

void err_quit(const char * mess)
{
		fputs(mess,stderr);
		fputc('\n',stderr);
		exit(1);
}
