#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<netdb.h>
#include <time.h>
#include<signal.h>
#include<sys/wait.h>

#define MAX_QUE_CONN_NM 1024  //监听的最大数量
#define MAX_SOCK_FD FD_SETSIZE
#define PORT 7000
#define BUFFER_SIZE 9000
#define HEAD_LENGTH 10 //报文头的长度
#define JIAO_YI_LEI_XING_LENGTH 6 //交易代码的长度
#define MAX_QUE_ZI_DUAN 64
typedef struct taocanmulu
{
	int length;
	int offset;
}taocanmulu;
taocanmulu taocanmulupeizhi[MAX_QUE_ZI_DUAN]={
																													{2,0},
																													{8,2},
																													{6,10},
																													{3,16},
																													{4,19},
																													
																													{6,23},
																													{64,29},
																													{13,93}};
typedef struct dinggoutaocan
{
	int length;
	int offset;
}dinggoutaocan;
dinggoutaocan dinggoutaocanpeizhi[MAX_QUE_ZI_DUAN]={
																													{2,0},
																													{8,2},
																													{6,10},
																													{3,16}};
typedef struct yonghuchaxun
{
	int length;
	int offset;
}yonghuchaxun;
yonghuchaxun yonghuchaxunpeizhi[MAX_QUE_ZI_DUAN]={
																													{{2,0},
																													{8,2},
																													{6,10},
																													{3,16},
																													{32,19},
																													{1,51},
																													{64,52},
																													{1,116}};

void sigchld_handler(int sig)
{
	while (waitpid(-1, 0, WNOHANG) > 0);
	return;
}

int gettype(char * buf,char *temp)
{
	//char temp[8];
	int i,j=0;
	for(i=HEAD_LENGTH;i<HEAD_LENGTH + JIAO_YI_LEI_XING_LENGTH;i++)
	{
			temp[j]=buf[i];
			j++;
	}
	return 1;
}

/*读取数据包的函数*/
/*读取数据包的函数*/
int read_data(int fd,char *buf)
{

	int buflen = 0; 
    if((buflen = recv(fd,buf,BUFFER_SIZE,0) )> 0)
    {
	}
	return buflen;
}
void insert(char *send,int start ,int length,char *node)
{
	int i,j=0;
/*	if(strlen(node)>length)
		{
				printf("length error\n");
				return;
			}
*/
	for(i=start;j<strlen(node);i++)
	{
			
				send[i]=node[j];
				j++;
			
	}
	for(i=start+j;i<start+length;i++)  //bu zu  bu 'p'---------------
		send[i]=' ';
}

void chongzufabao(char *send,int n)
{
	
	
	int pos=0;
	int i=0,j=0;
	if(n==0)
	{
	insert(send,pos,taocanmulupeizhi[i].length,"GD");
	pos+=2;
	i++;
	
		insert(send,pos,taocanmulupeizhi[i].length,"00000189");
	pos+=8;
	i++;

		insert(send,pos,taocanmulupeizhi[i].length,"500004");
pos+=6;
	i++;
	
		insert(send,pos,taocanmulupeizhi[i].length,"000");
	pos+=3;
	i++;

		insert(send,pos,taocanmulupeizhi[i].length,"0002");
pos+=4;
	i++;
	j=i;
		insert(send,pos,taocanmulupeizhi[i].length,"000001");//taocandaima
pos+=6;
	i++;
	
		insert(send,pos,taocanmulupeizhi[i].length,"套餐1");//mingcheng
pos+=64;
	i++;

		insert(send,pos,taocanmulupeizhi[i].length,"0000000000100");//jiage

	pos+=13;

 
	i=j;
		insert(send,pos,taocanmulupeizhi[i].length,"000002");//chanpindaima
pos+=6;
	i++;
	
		insert(send,pos,taocanmulupeizhi[i].length,"套餐2");
pos+=64;
	i++;

		insert(send,pos,taocanmulupeizhi[i].length,"0000000000102");

	


	
	
	
	

	}
	else if(n==1)
	{
			insert(send,pos,dinggoutaocanpeizhi[i].length,"GD");
	pos+=2;
	i++;
	
		insert(send,pos,dinggoutaocanpeizhi[i].length,"00000019");
	pos+=8;
	i++;

		insert(send,pos,dinggoutaocanpeizhi[i].length,"500005");
pos+=6;
	i++;
	
		insert(send,pos,dinggoutaocanpeizhi[i].length,"000");
	

		

	}
	else if(n==2){
	insert(send,pos,yonghuchaxunpeizhi[i].length,"GD");
	pos+=2;
	i++;
	
		insert(send,pos,yonghuchaxunpeizhi[i].length,"00000117");
	pos+=8;
	i++;

		insert(send,pos,yonghuchaxunpeizhi[i].length,"500006");
pos+=6;
	i++;
	
		insert(send,pos,yonghuchaxunpeizhi[i].length,"000");
	pos+=3;
	i++;

		insert(send,pos,yonghuchaxunpeizhi[i].length,"000000000000");
pos+=32;
	i++;
	insert(send,pos,yonghuchaxunpeizhi[i].length,"A");
pos+=1;
	i++;
	
		insert(send,pos,yonghuchaxunpeizhi[i].length,"何华");
pos+=64;
	i++;
	
		insert(send,pos,yonghuchaxunpeizhi[i].length,"4");//mingcheng
pos+=1;
	i++;


 
}
}
int main()
{
	pid_t pid;
	int sendbytes;
	char sendbao[BUFFER_SIZE]; //= "laiziyinhang";
	char readbao[BUFFER_SIZE]; // +++++++++++++++++++++
 	char buf[BUFFER_SIZE];  
	//char sendbuf[BUFFER_SIZE];   
	struct sockaddr_in server_sockaddr,client_sockaddr;
	int sin_size,read_sum;
	int sockfd,client_fd,fd;
	signal(SIGCHLD, sigchld_handler); 
	memset(buf,'\0',sizeof(buf));
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_sockaddr.sin_zero),8);
	int i = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
	
	if(bind(sockfd,(struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if(listen(sockfd,MAX_QUE_CONN_NM) == -1)
	{
		perror("listen");
		exit(1);
	}
	sin_size = sizeof(struct sockaddr_in);
	while(1)
	{
		
		if((client_fd = accept(sockfd,(struct sockaddr *)&client_sockaddr,&sin_size)) == -1)
		{
		perror("accept");
		exit(1);
	}
		while ((pid = fork()) == -1);
		if (pid == 0)            //在子进程中处理
		{
			close(sockfd);			 				
		if((read_sum=read_data(client_fd,readbao)) > 0)
			{
				printf("%dBoss接受----------------%s\n",getpid(),readbao);
				char type[JIAO_YI_LEI_XING_LENGTH];
		if(!gettype(readbao,type))
			{
				printf("cant get type!");
			}
				
				printf("type----%s\n",type);
				if(!strcmp(type,"500004"))//taocanmulu
				{
					chongzufabao(sendbao,0);
				}
				else if(!strcmp(type,"500005"))//dinggoutaocan
				{
					chongzufabao(sendbao,1);
				}
				else if(!strcmp(type,"500006"))//yonghuchaxun
				{
					chongzufabao(sendbao,2);
				}
			
				else
				{
				}
				
			
			if ((sendbytes=send(client_fd, sendbao, strlen(sendbao), 0)) ==-1) 
				{
				perror("send出错！");
				exit(1);
			}

				printf("GD发送----------------%s\n",sendbao);
				exit(0);
			}
		
		}
		else
{		close(client_fd);
}

	}
				

	close(sockfd);

	return 0;
}
