/*文件传输服务端
 *author:TBB
 *daytime:3/21/2021
 *read()函数和write()函数若只传输一个'\0'(空文件)，则返回0，其他情况'\0'也占一个字节进行传输
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define MAXSIZE 1024
#define BAG 15

void err_quit(const char *message);

int main(int argc,char *argv[])
{
		int servsock,clisock;
		struct sockaddr_in servaddr,cliaddr;
		socklen_t clilen;
		char mess[MAXSIZE];
		char site[50];
		int len;
		int filesock;
		char err[] = "File Not Exist!\n";

		if(argc!=2)
		{
				printf("Usage:%s <port>\n",argv[0]);
				exit(1);
		}

		if((servsock=socket(PF_INET,SOCK_STREAM,0))<0)
				err_quit("socket error");

		memset(&servaddr,0,sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(atoi(argv[1]));

		if(bind(servsock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
				err_quit("bind error");

		if(listen(servsock,BAG)<0)
				err_quit("listen error");

		clilen = sizeof(cliaddr);

		for(;;)
		{
				if((clisock=accept(servsock,(struct sockaddr *)&cliaddr,&clilen))<0)
						err_quit("accept error");
				printf("Connected from->IP:%s,Port:%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));//输出客户端的地址信息
				
				if((len=read(clisock,site,50))<0)//此处读取也没有'\0'
						err_quit("read way error");
				site[len-1] = '\0';//减一是为了将'\n'替换为'\0'

				if((filesock=open(site,O_RDONLY))<0)//只读方式打开文件出错
						write(clisock,err,sizeof(err));//'\0'也会传输过去
				else
				{ 
						if((len=read(filesock,mess,MAXSIZE))<0)//文件末尾的'\0'也会读取
								err_quit("read data error");
						printf("%d\n",len);
						write(clisock,mess,len);//必须把'\0'也传输过去
				}
				close(filesock);
				close(clisock);
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
