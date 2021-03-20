#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
void err_quit(char *m);//自定义函数声明
int main(int argc,char *argv[])
{
		int serv;//服务器端(监听)套接字
		int cli;//与客户端进行通信的套接字
		struct sockaddr_in servaddr;//保存服务器套接字的地址信息
		struct sockaddr_in cliaddr;//保存客户端套接字的地址信息
		socklen_t clilen;//客户端cliaddr长度
		char wo[] = "hello my baby!";
		if(argc!=2)
		{
				printf("Usage:%s <port> \n",argv[0]);
				exit(1);
		}
		if((serv=socket(PF_INET,SOCK_STREAM,0))<0)//创建服务器端套接字,用<判断效率更高
				err_quit("connect error");
		memset(&servaddr,0,sizeof(servaddr));//地址信息置零
		servaddr.sin_family = AF_INET;//设置地址族,单纯是为了与sockaddr结构体对齐
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//表示接受任意IP的请求连接
		servaddr.sin_port = htons(atoi(argv[1]));//自定义端口
		if(bind(serv,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)//将服务器端套接字与地址信息进行绑定
				err_quit("bind error");
		if(listen(serv,5)<0)//监听,开放等待请求队列，服务器端套接字进入监听状态
				err_quit("listen error");
		clilen = sizeof(cliaddr);
		cli = accept(serv,(struct sockaddr *)&cliaddr,&clilen);//从等待队列对头取请求进行连接
		write(cli,wo,sizeof(wo));//通信，传输数据
		close(serv);//关闭两个套接字
		close(cli);
		return 0;
}

void err_quit(char *m)//自定义函数定义
{
		fputs(m,stderr);
		fputc('\n',stderr);
		exit(1);
}
