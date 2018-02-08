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

#define MAX_QUE_CONN_NM 1024  //�������������
#define MAX_SOCK_FD FD_SETSIZE
#define PORT 3381
#define BUFFER_SIZE 9000
#define HEAD_LENGTH 10 //����ͷ�ĳ���
#define JIAO_YI_LEI_XING_LENGTH 6 //���״���ĳ���
#define MAX_QUE_ZI_DUAN 64


typedef struct tao_can_fan_hui_zi_duan 
{
int length;
int offset;
}tao_can_fan_hui_zi_duan;
tao_can_fan_hui_zi_duan taocanfanhuipeizhi[MAX_QUE_ZI_DUAN] = {{2,0},{8,2},{6,10},{3,16},{4,19},{6,23},{64,29},{13,93}};

/*
typedef struct jie_yue_fan_hui_zi_duan
{
		int length;
		int offset;		
}jie_yue_fan_hui_zi_duan;
jie_yue_fan_hui_zi_duan jieyuefanhuipeizhi[MAX_QUE_ZI_DUAN]={{4,0},{16,4},{5,20},{4,25},{128,29}};
*/
char suijiziduan1[2][5]={"1234","5432"};
char jieguodaima[2][4]={"0011","0099"};//0011-chenggong  0099-shibai
//char jieguoshuoming[2][]={""};]

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

/*��ȡ���ݰ��ĺ���*/
/*��ȡ���ݰ��ĺ���*/
int read_data(int fd,char *buf)
{
/*	int i,sum=0;
	int buflen;
	char temp[BUFFER_SIZE];
	int sendbytes;
	memset(temp,0,sizeof(temp));
	
	while((buflen = recv(fd,temp,BUFFER_SIZE,0) )> 0)
	{
//	  sendbytes=send(fd, temp, strlen(temp), 0);
							
						//printf("%s\n",buf);
						//memset(buf,0,sizeof(buf));		
		for(i=0;i<buflen;i++)
		{
			buf[sum++] = temp[i]; 
		}
		
	}
//	printf("---------------------------------\n");
	return sum;	
*/
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
	for(i=start+j;i<start+length;i++)  //bu zu  bu 'p'---------------���������治����
		send[i]=' ';
}

void chongzufabao(char *send,int n)
{
	
	int pid=getpid();
	int rand_num;
	srand((int) time(0));
	rand_num=(rand()+pid) % (2);//chansheng suiji shu
	
	//printf("rand num----%d\n",rand_num);
	
	int pos=0;
	int i=0;

	if(n==0)
	{
	insert(send,pos,taocanfanhuipeizhi[i].length,"GD");
	pos+=2;
	i++;
	insert(send,pos,taocanfanhuipeizhi[i].length,"00000355");
	pos+=8;
	i++;
	insert(send,pos,taocanfanhuipeizhi[i].length,"500004");
	pos+=6;
	i++;
    insert(send,pos,taocanfanhuipeizhi[i].length,"000");
	pos+=3;
	i++;
	insert(send,pos,taocanfanhuipeizhi[i].length,"0004");
	pos+=4;
	i++;
	insert(send,pos,taocanfanhuipeizhi[i].length,"021365");
	pos+=6;
	i++;
	insert(send,pos,taocanfanhuipeizhi[i].length,"������в����ײ�");
	pos+=64;
	i++;
	insert(send,pos,taocanfanhuipeizhi[i].length,"         0.10");
	pos+=13;
	
	
	insert(send,pos,6,"021626");
	pos+=6;
	i++;
	insert(send,pos,64,"�������ֵ������ն�2014����A����330Ԫ��");
	pos+=64;
	i++;
	insert(send,pos,13,"       330.00");
	pos+=13;
	
	
	insert(send,pos,6,"021627");
	pos+=6;
	i++;
	insert(send,pos,64,"���и��廥������488�ײ�");
	pos+=64;
	i++;
	insert(send,pos,13,"       488.00");
	pos+=13;
	
	
	
	insert(send,pos,6,"021564");
	pos+=6;
	i++;
	insert(send,pos,64,"�������ֵ������ն�2014����A����330Ԫ��");
	pos+=64;
	i++;
	insert(send,pos,13,"       330.00");
	pos+=13;
	memset(send,'\0',sizeof(send));
	strcpy(send,"GD000006875000040000008021648�������ֵ��Ӹ��ն�2014����A����150Ԫ��                                 150.00021626�������ֵ������ն�2014����A����330Ԫ��                                 330.00021627�������ֵ������ն�2014����B����360Ԫ��                                 360.00022773���ֵ���2015���ն�����C��12���£�388Ԫ��                               388.00023299e+TV���廥���������ն������ײ�12���£�420Ԫ��                          420.00022279���ֵ��Ӳ�����456���ն��ײ�12���£�456Ԫ��                             456.00023280e+TV���廥���������ն������ײ�12���£�480Ԫ��                          480.00021629���и���ֱ��488���ն��ײ�                                              488.00");
}
else if(n==1)
{
	insert(send,pos,2,"GD");
	pos+=2;
	i++;
	insert(send,pos,8,"00000117");
	pos+=8;
	i++;
	insert(send,pos,6,"500006");
	pos+=6;
	i++;
    insert(send,pos,3,"000");
	pos+=3;
	i++;
	insert(send,pos,32,"SXʡ��˾");
	pos+=32;
	i++;
	insert(send,pos,1,"A");
	pos+=1;
	i++;
	insert(send,pos,64,"�������");
	pos+=64;
	i++;
	insert(send,pos,1,"4");
	pos+=1;
	memset(send,'\0',sizeof(send));
	strcpy(send,"GD00000117500006000XA��������                      A������                                                          4");

	}
	
	else if(n==2)
{
	insert(send,pos,2,"GD");
	pos+=2;
	i++;
	insert(send,pos,8,"00000019");
	pos+=8;
	i++;
	insert(send,pos,6,"500005");
	pos+=6;
	i++;
    insert(send,pos,3,"000");
	pos+=3;
	
	}
	printf("[%drow][%d][%s]\n",__LINE__,strlen(send),send);
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
		if (pid == 0)            //���ӽ����д���
		{
			close(sockfd);			 				
			if((read_sum=read_data(client_fd,readbao)) > 0)
			//if(scanf("%s",&readbao));
			{
				printf("%d������----------------%s\n",getpid(),readbao);
				char type[JIAO_YI_LEI_XING_LENGTH];
				if(!gettype(readbao,type))
				{
					printf("cant get type!");
				}
				printf("type----%s\n",type);
				if(!strcmp(type,"500004"))//Ŀ¼�ײ�
				{
					chongzufabao(sendbao,0);
				}
				else if(!strcmp(type,"500006"))//�û���ѯ
				{
					chongzufabao(sendbao,1);
				}
				else if(!strcmp(type,"500005"))//chexiao
				{
					chongzufabao(sendbao,2);
				}
				else if(!strcmp(type,"200004"))//duizhang
				{
					chongzufabao(sendbao,3);
				}
				else if(!strcmp(type,"200021"))//qianyue jieyue
				{
					chongzufabao(sendbao,4);
				}
				else
				{
				}
				
			
				if ((sendbytes=send(client_fd, sendbao, strlen(sendbao), 0)) ==-1) 
				{
					perror("send����");
					exit(1);
				}

				printf("%d��������----------------%s\n",getpid(),sendbao);
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
