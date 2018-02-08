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
#define PORT 9217
#define BUFFER_SIZE 9000
#define HEAD_LENGTH 25 //报文头的长度
#define JIAO_YI_LEI_XING_LENGTH 6 //交易代码的长度
#define MAX_QUE_ZI_DUAN 64
typedef struct qian_fei_fan_hui_zi_duan 
{
int length;
int offset;
}qian_fei_fan_hui_zi_duan;
qian_fei_fan_hui_zi_duan qianfeifanhuipeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{16,4},{5,20},{4,25},{128,29},{32,157},{16,189},{32,205},{64,237},{128,301},{64,429},{18,493},{18,511},{4,529},{16,533},{6,549},{18,555},{18,573},{64,591}};

typedef struct jiao_fei_fan_hui_zi_duan 
{
int length;
int offset;
}jiao_fei_fan_hui_zi_duan;
jiao_fei_fan_hui_zi_duan jiaofeifanhuipeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{16,4},{5,20},{4,25},{128,29}};

typedef struct che_xiao_fan_hui_zi_duan 
{
int length;
int offset;
}che_xiao_fan_hui_zi_duan;
che_xiao_fan_hui_zi_duan chexiaofanhuipeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{16,4},{5,20},{4,25},{128,29}};

typedef struct dui_zhang_fan_hui_zi_duan
{
		int length;
		int offset;		
}dui_zhang_fan_hui_zi_duan;
dui_zhang_fan_hui_zi_duan duizhangfanhuipeizhi[MAX_QUE_ZI_DUAN]={{4,0},{16,4},{5,20},{4,25},{128,29}};

typedef struct qian_jie_yue_fan_hui_zi_duan
{
		int length;
		int offset;		
}qian_jie_yue_fan_hui_zi_duan;
qian_jie_yue_fan_hui_zi_duan qianjieyuefanhuipeizhi[MAX_QUE_ZI_DUAN]={{4,0},{16,4},{5,20},{4,25},{128,29},{16,157}};







typedef struct du_ka_cha_xun_from_elec_zi_duan
{
	int length;
	int offset;
}du_ka_cha_xun_from_elec_zi_duan;
du_ka_cha_xun_from_elec_zi_duan dukachaxunfromelecpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{16,20},{5,36},{8,41},{128,49},{32,177},{16,209},
{64,225},{16,289},{64,305},{10,369},{10,379},{10,389},{10,399},{10,409},{8,419},{18,427},{18,445},{18,463},{32,481},{8,513},{18,521},
{16,539},{18,555},{1,573}};



typedef struct ka_biao_gou_dian_from_elec_zi_duan
{
	int length;
	int offset;
}ka_biao_gou_dian_from_elec_zi_duan;
ka_biao_gou_dian_from_elec_zi_duan kabiaogoudianfromelecpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{16,20},{5,36},{4,41},{128,45},{8,173},{18,181},
{18,199},{98,217},{24,315},{264,339},{268,603},{264,871},{268,1135},{106,1403},{16,1509},{16,1525},{16,1541}};


typedef struct ka_biao_gou_dian_chong_zheng_from_elec_zi_duan
{
	int length;
	int offset;
}ka_biao_gou_dian_chong_zheng_from_elec_zi_duan;
ka_biao_gou_dian_chong_zheng_from_elec_zi_duan kabiaogoudianchongzhengfromelecpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{16,20},{5,36},{4,41},
{128,45},{1,173}};


typedef struct bu_xie_ka_from_elec_zi_duan
{
	int length;
	int offset;
}bu_xie_ka_from_elec_zi_duan;
bu_xie_ka_from_elec_zi_duan buxiekafromelecpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{16,20},{5,36},{4,41},{128,45},{8,173},{18,181},
{98,199},{24,297},{264,321},{268,585},{264,853},{268,1117},{106,1385},{16,1491},{16,1507},{16,1523}};

typedef struct jiao_fei_piaoju_chaxun_zi_duan
{
		int length;
		int offset;		
}jiao_fei_piaoju_chaxun_zi_duan;
jiao_fei_piaoju_chaxun_zi_duan jiaofeipiaojuchaxunpeizhi[MAX_QUE_ZI_DUAN]={{4,0},{16,4},{5,20},{4,25},{128,29},{4,157},{16,161},{1,177},{128,178}};
typedef struct piao_ju_da_yin_xinxi_zi_duan
{
		int length;
		int offset;		
}piao_ju_da_yin_xinxi_zi_duan;
piao_ju_da_yin_xinxi_zi_duan piaojudayinxinxipeizhi[MAX_QUE_ZI_DUAN]={{4,0},{16,4},{5,20},{1000,25}};


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

/*读取数据包的函数*/
/*读取数据包的函数*/
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
	//for(i=start+j;i<start+length;i++)  //bu zu  bu 'p'---------------定长，后面不用填
	//	send[i]=' ';
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
	insert(send,pos,qianfeifanhuipeizhi[i].length,"0001");
	pos+=4;
	i++;
	//insert(send,pos++,1,"|");
	//rand_num=(rand()+pid) % (2);//chansheng suiji shu
		insert(send,pos,qianfeifanhuipeizhi[i].length,"887D16841EB744A9");
	pos+=16;
	i++;
	//insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"00141");///////////////////////原来的520-1，因为后面少了一个|
	pos+=5;
	i++;
	//insert(send,pos++,1,"|");
	//rand_num=(rand()+pid) % (2);//chansheng suiji shu
		insert(send,pos,qianfeifanhuipeizhi[i].length,"0000");//jieguodaima
	pos+=4;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"交易成功!");
	pos+=13;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"000000");
	pos+=6;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"34401");
	pos+=5;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"5131100067");
	pos+=10;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"何华");
	pos+=6;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"西安市");
	pos+=9;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"甜水井七十号三号楼三单元402室");
	pos+=42;
	i++;
	insert(send,pos++,1,"|");
	
	
	
		insert(send,pos,qianfeifanhuipeizhi[i].length,"2501");
	pos+=4;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"0");
	pos+=1;
	i++;
	insert(send,pos++,1,"|");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"0001");
	pos+=4;
	i++;
	insert(send,pos++,1,"|");
	
		insert(send,pos,qianfeifanhuipeizhi[i].length,"177879470");
	pos+=9;
	i++;
	insert(send,pos++,1,"&");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"201308");
	pos+=6;
	i++;
	insert(send,pos++,1,"&");	
	insert(send,pos,qianfeifanhuipeizhi[i].length,"1629");
	pos+=4;
	i++;
	insert(send,pos++,1,"&");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"0");
	pos+=1;
	i++;
	insert(send,pos++,1,"&");
	insert(send,pos,qianfeifanhuipeizhi[i].length,"");
	pos+=0;
	i++;
	insert(send,pos++,1,"&");
	insert(send,pos++,1,";");
/*	i=14;
		insert(send,pos,qianfeifanhuipeizhi[i].length,"177879478");
	pos+=9;
	i++;
	insert(send,pos++,1,"&");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"201309");
	pos+=6;
	i++;
	insert(send,pos++,1,"&");	
	insert(send,pos,qianfeifanhuipeizhi[i].length,"800");
	pos+=3;
	i++;
	insert(send,pos++,1,"&");
		insert(send,pos,qianfeifanhuipeizhi[i].length,"72");
	pos+=2;
	i++;
	insert(send,pos++,1,"&");
	insert(send,pos,qianfeifanhuipeizhi[i].length,"");
	pos+=0;
	i++;
	insert(send,pos++,1,"&");
	insert(send,pos++,1,";");
*/	
	insert(send,pos++,1,"|");
	//printf("pos%d\n",strlen(send));
	
	}
	else if(n==1)
	{
			insert(send,pos,jiaofeifanhuipeizhi[0].length,"0001");
	pos+=4;
	//insert(send,pos++,1,"|");
	//rand_num=(rand()+pid) % (2);//chansheng suiji shu
			insert(send,pos,jiaofeifanhuipeizhi[1].length,"410DC8AA2889D54A");
	pos+=16;
	//insert(send,pos++,1,"|");
		insert(send,pos,jiaofeifanhuipeizhi[2].length,"00019");
	pos+=5;
	//insert(send,pos++,1,"|");
	//rand_num=(rand()+pid) % (2);//chansheng suiji shu
		insert(send,pos,jiaofeifanhuipeizhi[3].length,"0000");
	pos+=4;
	insert(send,pos++,1,"|");
		insert(send,pos,jiaofeifanhuipeizhi[4].length,"缴费成功!");
	pos+=13;
	insert(send,pos++,1,"|");
	}
	else if(n==2){
			insert(send,pos,chexiaofanhuipeizhi[0].length,"0001");
			pos+=4;
			//insert(send,pos++,1,"|");
			//rand_num=(rand()+pid) % (2);//chansheng suiji shu
			insert(send,pos,chexiaofanhuipeizhi[1].length,"410DC8AA2889D54A");
			pos+=16;
		//	insert(send,pos++,1,"|");
			insert(send,pos,chexiaofanhuipeizhi[2].length,"00019");
			pos+=5;
		//	insert(send,pos++,1,"|");
			//rand_num=(rand()+pid) % (2);//chansheng suiji shu
			insert(send,pos,chexiaofanhuipeizhi[3].length,"0000");
			pos+=4;
			insert(send,pos++,1,"|");
			insert(send,pos,chexiaofanhuipeizhi[4].length,"冲正成功!");
			pos+=13;
			insert(send,pos++,1,"|");
		
		}
		else if(n==3)
		{
				insert(send,pos,duizhangfanhuipeizhi[0].length,"0001");
				pos+=4;
				//insert(send,pos++,1,"|");
				//rand_num=(rand()+pid) % (2);//chansheng suiji shu
				insert(send,pos,duizhangfanhuipeizhi[1].length,"410DC8AA2889D54A");
				pos+=16;
				//insert(send,pos++,1,"|");
				insert(send,pos,duizhangfanhuipeizhi[2].length,"00025");
				pos+=5;
			//	insert(send,pos++,1,"|");
			//	rand_num=(rand()+pid) % (2);//chansheng suiji shu
				insert(send,pos,duizhangfanhuipeizhi[3].length,"0000");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,duizhangfanhuipeizhi[4].length,"代收对账成功!");
				pos+=19;
				insert(send,pos++,1,"|");
			
		}
		else if(n==4)//qianyue jieyue
		{
							insert(send,pos,qianjieyuefanhuipeizhi[0].length,"0001");
				pos+=4;
				//insert(send,pos++,1,"|");
				//rand_num=(rand()+pid) % (2);//chansheng suiji shu
				insert(send,pos,qianjieyuefanhuipeizhi[1].length,"3075F3CE8370C8F2");
				pos+=16;
				//insert(send,pos++,1,"|");
				insert(send,pos,qianjieyuefanhuipeizhi[2].length,"00025");
				pos+=5;
				//insert(send,pos++,1,"|");
				//rand_num=(rand()+pid) % (2);//chansheng suiji shu
				insert(send,pos,qianjieyuefanhuipeizhi[3].length,"0000");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,qianjieyuefanhuipeizhi[4].length,"签约成功!");
				pos+=13;
				insert(send,pos++,1,"|");
				insert(send,pos,qianjieyuefanhuipeizhi[5].length,"34401");
				pos+=5;
				insert(send,pos++,1,"|");
			
		}
		else if(n==5)//dukachaxun
		{
			char tempa[]="1015454A34D7C540F91F001330000|交易成功!||61404|浐营供电所|6350005765|安存海|0|49.83|49.83|49.83|49.83|00|3000|0|0|006350005765|1|0|6350006353|0|1|";
			strcpy(send,tempa);
/*			insert(send,pos,dukachaxunfromelecpeizhi[0].length,"0001");
				pos+=4;
				insert(send,pos,dukachaxunfromelecpeizhi[1].length,"410DC8AA2889D54A");
				pos+=16;
				insert(send,pos,dukachaxunfromelecpeizhi[2].length,"00131");
				pos+=5;
			
				insert(send,pos,dukachaxunfromelecpeizhi[3].length,"0000");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[4].length,"交易成功!");
				pos+=13;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[5].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[6].length,"34401");
				pos+=5;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[7].length,"0001");//
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[8].length,"5147006392");
				pos+=10;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[9].length,"0001");//
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[10].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[11].length,"0.000000");
				pos+=8;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[12].length,"0.000000");
				pos+=8;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[13].length,"0.565300");
				pos+=8;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[14].length,"0.000000");
				pos+=8;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[15].length,"01");
				pos+=2;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[16].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[17].length,"0.000000");
				pos+=8;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[18].length,"0.000000");
				pos+=8;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[19].length,"1");
				pos+=1;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[20].length,"1");
				pos+=1;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[21].length,"0");
				pos+=1;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[22].length,"0000000352");
				pos+=10;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[23].length,"42951");
				pos+=5;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[24].length,"1");
				pos+=1;
				insert(send,pos++,1,"|");
				
	*/		
		}
			else if(n==6)//kabiaogoudian
		{
		char temp[]="1015FF317ECEC7712CE9013820000|交易成功!|2|10.0|52100|68010027028000000001000000000012000010000000000000000100000100163500063500635000576502c616319226F1|000003e80000000200AA0078|680100fc000049830000498300004983000049830000498300004983000049830000498300004983000049830000498300004983000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002C530D0E|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008d163A2B1074|680100fc0000498300004983000049830000498300004983000049830000498300004983000049830000498300004983000049830000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000019A86194|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008d165391216F|000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000007A2341CE|D34F6606E0E407A9|75B3ABE190C73E55|072F7E671D0A0CDC|";
			strcpy(send,temp);
			/*	insert(send,pos,kabiaogoudianfromelecpeizhi[0].length,"1015");
				pos+=4;
				insert(send,pos,kabiaogoudianfromelecpeizhi[1].length,"EF1023863467B26B");
				pos+=16;
				insert(send,pos,kabiaogoudianfromelecpeizhi[2].length,"01377");
				pos+=5;
			
				insert(send,pos,kabiaogoudianfromelecpeizhi[3].length,"0000");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[4].length,"交易成功!");
				pos+=13;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[5].length,"2");
				pos+=1;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[6].length,"1.0");
				pos+=3;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[7].length,"50500");
				pos+=5;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[8].length,"6801002702800000000100000000001200001000000000000
0000100000100163500063500635000576502c616619E4BE1");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[9].length,"0000006400000002F516D80A");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[10].length,"680100fc00004983000049830
00049830000498300004983000049830000498300004983000
04983000049830000498300004983000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
0000000000000000000000000000000F0A7B8B7");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[11].length,"0000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000008d16
54CD2736");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[12].length,"680100fc000049830000498300004983000049830
00049830000498300004983000049830000498300004983000
04983000049830000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
000000000000000B6D47B20");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[13].length,"00000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
0000000000000000000000000000008d16649502EA");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[14].length,"0000000
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000BA00ECF6");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[15].length,"243CE094062A0358");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[16].length,"FF7930C3007817D2");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,kabiaogoudianfromelecpeizhi[17].length,"F359DFA1CDB8F16F");
				pos+=0;
				insert(send,pos++,1,"|");
		
		*/		
			
		}
		
		else if(n==7)//goudianchongzheng
		{
			
				insert(send,pos,dukachaxunfromelecpeizhi[0].length,"0001");
				pos+=4;
				insert(send,pos,dukachaxunfromelecpeizhi[1].length,"410DC8AA2889D54A");
				pos+=16;
				insert(send,pos,dukachaxunfromelecpeizhi[2].length,"00021");
				pos+=5;
			
				insert(send,pos,dukachaxunfromelecpeizhi[3].length,"0000");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[4].length,"交易成功!");
				pos+=13;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[5].length,"0");
				pos+=1;
				insert(send,pos++,1,"|");
				
		}
		
		else if(n==8)//buxieka
		{
			
				insert(send,pos,dukachaxunfromelecpeizhi[0].length,"0001");
				pos+=4;
				insert(send,pos,dukachaxunfromelecpeizhi[1].length,"410DC8AA2889D54A");
				pos+=16;
				insert(send,pos,dukachaxunfromelecpeizhi[2].length,"00083");
				pos+=5;
			
				insert(send,pos,dukachaxunfromelecpeizhi[3].length,"0000");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[4].length,"交易成功!");
				pos+=13;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[5].length,"2");
				pos+=1;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[6].length,"1.2");
				pos+=3;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[7].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[8].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[9].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[10].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[11].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[12].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[13].length,"");
				pos+=0;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[14].length,"3846080237345314");
				pos+=16;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[15].length,"76572C6602A0A774");
				pos+=16;
				insert(send,pos++,1,"|");
				insert(send,pos,dukachaxunfromelecpeizhi[16].length,"0B06F6D94B6A479D");
				pos+=16;
				insert(send,pos++,1,"|");
				
			
		}
		else if(n==9)//票据查询
		{
			
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[0].length,"0001");
				pos+=4;
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[1].length,"410DC8AA2889D54A");
				pos+=16;
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[2].length,"00088");
				pos+=5;
			
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[3].length,"0000");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[4].length,"交易成功!");
				pos+=13;
				insert(send,pos++,1,"|");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[5].length,"0003");
				pos+=4;
				insert(send,pos++,1,"|");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[6].length,"1");
				pos+=1;
				insert(send,pos++,1,"&");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[7].length,"1");
				pos+=1;
				insert(send,pos++,1,"&");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[8].length,"第一张发票");
				pos+=15;
				insert(send,pos++,1,"&");
				insert(send,pos++,1,";");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[6].length,"2");
				pos+=1;
				insert(send,pos++,1,"&");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[7].length,"2");
				pos+=1;
				insert(send,pos++,1,"&");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[8].length,"第二张发票");
				pos+=15;
				insert(send,pos++,1,"&");
				insert(send,pos++,1,";");
				
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[6].length,"3");
				pos+=1;
				insert(send,pos++,1,"&");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[7].length,"2");
				pos+=1;
				insert(send,pos++,1,"&");
				insert(send,pos,jiaofeipiaojuchaxunpeizhi[8].length,"第三张发票");
				pos+=15;
				insert(send,pos++,1,"&");
				insert(send,pos++,1,";");
				
				insert(send,pos++,1,"|");
				
			
		}
		else if(n==10)//票据打印
		{
			
				insert(send,pos,piaojudayinxinxipeizhi[0].length,"0001");
				pos+=4;
				insert(send,pos,piaojudayinxinxipeizhi[1].length,"410DC8AA2889D54A");
				pos+=16;
				insert(send,pos,piaojudayinxinxipeizhi[2].length,"00022");
				pos+=5;
				insert(send,pos,piaojudayinxinxipeizhi[3].length,"这是一个xml文件");
				pos+=21;
				insert(send,pos++,1,"|");
			
			
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
				printf("%d电网接受----------------%s\n",getpid(),readbao);
				char type[JIAO_YI_LEI_XING_LENGTH];
				if(!gettype(readbao,type))
				{
					printf("cant get type!");
				}
				printf("type----%s\n",type);
				if(!strcmp(type,"200001"))//chaxun
				{
					chongzufabao(sendbao,0);
				}
				else if(!strcmp(type,"200002"))//jiaofei
				{
					chongzufabao(sendbao,1);
				}
				else if(!strcmp(type,"200003"))//chexiao
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
				else if(!strcmp(type,"200005"))//读卡信息查询
				{
					chongzufabao(sendbao,5);
				}
				else if(!strcmp(type,"200006"))//卡表购电
				{
					chongzufabao(sendbao,6);
				}
				else if(!strcmp(type,"200007"))//卡表购电冲正
				{
					chongzufabao(sendbao,7);
				}
				else if(!strcmp(type,"200008"))//补写卡
				{
					chongzufabao(sendbao,8);
				}
					else if(!strcmp(type,"200009"))//票据查询
				{
					chongzufabao(sendbao,9);
				}
					else if(!strcmp(type,"200010"))//票据打印
				{
					chongzufabao(sendbao,10);
				}
				
			
				if ((sendbytes=send(client_fd, sendbao, strlen(sendbao), 0)) ==-1) 
				{
					perror("send出错！");
					exit(1);
				}

				printf("%d电网发送----------------[%d][%s]\n",getpid(),strlen(sendbao),sendbao);
				exit(0);
				//while(1);
			}
		
		}
		else
{		close(client_fd);
}

	}
				

	close(sockfd);

	return 0;
}
