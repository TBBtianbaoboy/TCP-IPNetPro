/*
*Function:I/O ways to read and write data
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#define MAXSIZE 1024

void err_quit(const char *message);
void write_data(int sock,char *buf);
void read_data(int sock,char *buf);
int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in servaddr;
    char buf[MAXSIZE];
    pid_t pid;

    if(argc!=3)
    {
        printf("Usage:%s <IP> <Port>\n",argv[0]);
        exit(1);
    }

    if((sock=socket(PF_INET,SOCK_STREAM,0))<0)
        err_quit("Socket Error!");
    
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
        err_quit("Connect Error!");
    
    pid = fork();
    if(pid<0)
        err_quit("Fork Error!");
    if(pid==0)
        write_data(sock,buf);
    else
        read_data(sock,buf);
    close(sock);
    return 0;
}

void err_quit(const char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(0);
}
void write_data(int sock,char *buf)
{
    while(1)
    {
        if(fgets(buf,MAXSIZE,stdin)>0);
        if(!strcmp(buf,"\n"))
            printf("You Can't Input Empty Message.\n");
        else if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
        {
            shutdown(sock,SHUT_WR);//close write buffer
            return;
        }
        else
        {
            if(write(sock,buf,strlen(buf))>0);
        }
    }
}

void read_data(int sock,char *buf)
{
    int size;
    while(1)
    {
        printf("The Message From Server:");
        size = read(sock,buf,MAXSIZE);
        if(size==0)
            return;
        buf[size] = '\0';
        fputs(buf,stdout);
    }
}
