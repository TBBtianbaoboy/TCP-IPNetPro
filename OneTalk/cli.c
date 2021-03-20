#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
void err_quit(char *m);
int main(int argc,char *argv[])
{
		int sock;//与服务器端进行通信的套接字
		struct sockaddr_in servaddr;//保存服务器IP地址,端口信息
		char mes[1024];//接受服务器端传回的消息
		if(argc!=3)//参数必须是三个(包括命令)
		{
				printf("Usage:%s <IP> <port>\n",argv[0]);
				exit(1);
		}
		if((sock = socket(AF_INET,SOCK_STREAM,0))<0)//创建套接字
				err_quit("socket error");
		memset(&servaddr,0,sizeof(servaddr));//置零
		servaddr.sin_family = AF_INET;//设置地址族
		servaddr.sin_addr.s_addr = inet_addr(argv[1]);//以网络字节序将字符串转为整型
		servaddr.sin_port = htons(atoi(argv[2]));//字符串转为整型数据，再转为网络字节序
		if(connect(sock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)//连接服务器
				err_quit("connect error");
		int len = read(sock,mes,1024);//接受数据
		if(len>0)
				fputs(mes,stdout);
		close(sock);
		return 0;

}

void err_quit(char *m)
{
		fputs(m,stderr);
		fputc('\n',stderr);
		exit(1);
}
