#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main(void)
{
int stat=0;
int pid;
struct sockaddr_in SockAddr;
struct sockaddr_in CliAddr;

int AccSock;

unsigned char MessageBuff[1024];
char ReadFileBuff;

FILE *HtmlFile;


memset((void *)&MessageBuff,0,sizeof(MessageBuff));
memset((void *)&CliAddr,0,sizeof(CliAddr));

SockAddr.sin_family=AF_INET;
SockAddr.sin_port=htons(666);
SockAddr.sin_addr.s_addr=INADDR_ANY;
    

    int SrvSock=socket(AF_INET,SOCK_STREAM,0);
    if(SrvSock==-1)
	{
	printf("Socket creation error!\r\n");
	return -1;
	}
    printf("Created main socket: %d\r\n",SrvSock);
    stat=bind(SrvSock,(struct sockaddr *)&SockAddr,sizeof(SockAddr));
    if(stat!=0)
    {
	printf("Bind error!\r\n");
	return stat;
    }
    printf("Bind ok!\r\n");
    stat=listen(SrvSock,10240);
    if(stat!=0)
    {
    printf("Listen error!\r\n");
    return stat;
    }
    

    

    while(1)
    {
    socklen_t AddrLen=sizeof(CliAddr);
    AccSock=accept(SrvSock,(struct sockaddr *)&CliAddr,&AddrLen);
    printf("Accepted: %s\r\n",inet_ntoa(CliAddr.sin_addr));

    HtmlFile=fopen("index.html","r");
    if(HtmlFile==NULL)
    {
    printf("Error opening file!\r\n");
    return -1;
    }

    sprintf(MessageBuff,"HTTP/1.1 200 OK\r\n \
Content-type: text/html\n\n");
    write(AccSock,MessageBuff,strlen(MessageBuff));

    while(!feof(HtmlFile))
    {
    fread((char *)&ReadFileBuff,1,1,HtmlFile);
    write(AccSock,(char *)&ReadFileBuff,sizeof(ReadFileBuff));
    }

    fclose(HtmlFile);

    shutdown(AccSock,SHUT_RDWR);
    close(AccSock);
    printf("Connection closed!\r\n");
    }
}
