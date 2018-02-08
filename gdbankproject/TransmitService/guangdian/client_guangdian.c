#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<unistd.h>
#include <time.h>
#include<signal.h>
#include<sys/wait.h>
//#include <iconv.h> 

#define MAX_SOCK_FD FD_SETSIZE
#define MAX_QUE_CONN_NM 1024  

#define PORT 8660
#define SOCK_AMOUNT 0   //real sock amount = 2^n

#define BUFFER_SIZE 6001 
#define MAX_QUE_ZI_DUAN 64   

	int rand_num;
	char qianjieyuetype[3][2]={"01","02","03"};
	char qianyueyewuleibie[2][2]={"00","01"};


typedef struct qian_fei_zi_duan
{
	int length;
	int offset;
}qian_fei_zi_duan;
qian_fei_zi_duan qianfeipeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{40,182},{4,222}};


typedef struct jiao_fei_zi_duan
{
	int length;
	int offset;
}jiao_fei_zi_duan;
jiao_fei_zi_duan jiaofeipeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},
{40,102},{40,142},{40,182},{4,222},{4,226},{12,230},{4,242},{40,246},{40,286},{40,326},{256,366}};
 

typedef struct che_xiao_zi_duan
{
	int length;
	int offset;
}che_xiao_zi_duan;
che_xiao_zi_duan chexiaopeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},
{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{40,182},{20,222},{12,242},{40,254},{40,294},{40,334}};



/*duizhangjiaoyipeizhi*/
typedef struct dui_zhang_zi_duan
{
		int length;
		int offset;
}dui_zhang_zi_duan;
dui_zhang_zi_duan duizhangpeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},
{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{8,182},{20,190},{256,210},{40,466},{40,506},{256,546}};

/*qianyuejieyuepeizhi*/
typedef struct qian_jie_yue_zi_duan
{
		int length;
		int offset;
}qian_jie_yue_zi_duan;
qian_jie_yue_zi_duan qianjieyuepeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},
{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{40,182},{4,222},{20,226},{4,246},{20,250},{64,270},
{40,334},{256,374},{20,630},{20,650},{256,670},{4,926},{4,930},{12,934},{12,946},{64,958},{4,1022},
{64,1026},{40,1090},
{40,1130},{40,1170},{40,1210}};

/*yanzhengmima*/
typedef struct yan_mi_zi_duan
{
		int length;
		int offset;
}yan_mi_zi_duan;
yan_mi_zi_duan yanmipeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{12,182},{12,194},{4,206},{4,210},{32,214}};


/*xiugaimima*/
typedef struct gai_mi_zi_duan
{
		int length;
		int offset;
}gai_mi_zi_duan;
gai_mi_zi_duan gaimipeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{12,182},{12,194},{4,206},{4,210},{32,214},{32,246}};

/*dukapeizhi*/
typedef struct read_card_zi_duan
{
	int length;
	int offset;
}read_card_zi_duan;
read_card_zi_duan readcardpeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{100,182},{40,282},{256,322},{256,578},{100,834},{40,934},{40,974},{40,1014},{40,1054},{256,1094},{256,1350},{256,1606}};

/*kabiaogoudian*/
typedef struct gou_dian_zi_duan
{
		int length;
		int offset;
}gou_dian_zi_duan;
gou_dian_zi_duan goudianpeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{40,182},{40,222},{12,262},{256,274},{20,530},{12,550},{20,562},{40,582},{40,622},{40,662},{40,702},{256,742}};

/*goudianchongzheng
typedef struct chong_zheng_zi_duan
{
		int length;
		int offset;
}chong_zheng_zi_duan;
chong_zheng_zi_duan chongzhengpeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{},}

*/
/*int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)  
{  
        iconv_t cd;  
        int rc;  
        char **pin = &inbuf;  
        char **pout = &outbuf;  
  
        cd = iconv_open(to_charset,from_charset);  
        if (cd==0)  
                return -1;  
        memset(outbuf,0,outlen);  
        if (iconv(cd,pin,&inlen,pout,&outlen) == -1)  
                return -1;  
        iconv_close(cd);  
        return 0;  
} */
void sigchld_handler(int sig)
{
	while (waitpid(-1, 0, WNOHANG) > 0);
	return;
}
void insert(char *send,int start ,int length,char *node)
{
	int i,j=0;
/*	if(strlen(node)>length)
		{
				printf("length error\n");
//				printf("----------%s\n",node);
				return;
			}
*/
	for(i=start;j<strlen(node);i++)
	{
			
				send[i]=node[j];
				j++;
			
	}
	for(i=start+j;i<start+length;i++)  //bu zu  bu 'p'
		send[i]=' ';
}
void makepackage(int n,char *s)//
{
	if( n== 0)//chaxun 0000
	{
			insert(s,qianfeipeizhi[0].offset,qianfeipeizhi[0].length,"0222");
			insert(s,qianfeipeizhi[1].offset,qianfeipeizhi[1].length,"0000    ");
			insert(s,qianfeipeizhi[2].offset,qianfeipeizhi[2].length,"20130614786500239996");
			insert(s,qianfeipeizhi[3].offset,qianfeipeizhi[3].length,"20131126");
			insert(s,qianfeipeizhi[4].offset,qianfeipeizhi[4].length,"103333");
			insert(s,qianfeipeizhi[5].offset,qianfeipeizhi[5].length,"0912");
			insert(s,qianfeipeizhi[6].offset,qianfeipeizhi[6].length,"BK010001");
			insert(s,qianfeipeizhi[7].offset,qianfeipeizhi[7].length,"0000");
			insert(s,qianfeipeizhi[8].offset,qianfeipeizhi[8].length,"2002038791");
			insert(s,qianfeipeizhi[9].offset,qianfeipeizhi[9].length,"96766");
			insert(s,qianfeipeizhi[10].offset,qianfeipeizhi[10].length,"830");
			insert(s,qianfeipeizhi[11].offset,qianfeipeizhi[11].length,"");
			insert(s,qianfeipeizhi[12].offset,qianfeipeizhi[12].length,"");
			
			insert(s,qianfeipeizhi[13].offset,qianfeipeizhi[13].length,"4005748548");
			insert(s,qianfeipeizhi[14].offset,qianfeipeizhi[14].length,"0001");
	}
	else if(n == 1)//jiaofei 0001
	{
			insert(s,jiaofeipeizhi[0].offset,jiaofeipeizhi[0].length,"0618");
			insert(s,jiaofeipeizhi[1].offset,jiaofeipeizhi[1].length,"0001    ");
			insert(s,jiaofeipeizhi[2].offset,jiaofeipeizhi[2].length,"2014071378650023");
			insert(s,jiaofeipeizhi[3].offset,jiaofeipeizhi[3].length,"20140713");
			insert(s,jiaofeipeizhi[4].offset,jiaofeipeizhi[4].length,"103106");
			insert(s,jiaofeipeizhi[5].offset,jiaofeipeizhi[5].length,"029 ");
			insert(s,jiaofeipeizhi[6].offset,jiaofeipeizhi[6].length,"BK010001");
			insert(s,jiaofeipeizhi[7].offset,jiaofeipeizhi[7].length,"0000");
			insert(s,jiaofeipeizhi[8].offset,jiaofeipeizhi[8].length,"2002038789");
			insert(s,jiaofeipeizhi[9].offset,jiaofeipeizhi[9].length,"96766");
			insert(s,jiaofeipeizhi[10].offset,jiaofeipeizhi[10].length,"830");
			insert(s,jiaofeipeizhi[11].offset,jiaofeipeizhi[11].length,"");
			insert(s,jiaofeipeizhi[12].offset,jiaofeipeizhi[12].length,"");
			
			insert(s,jiaofeipeizhi[13].offset,jiaofeipeizhi[13].length,"4005748548");
			insert(s,jiaofeipeizhi[14].offset,jiaofeipeizhi[14].length,"0001");
			insert(s,jiaofeipeizhi[15].offset,jiaofeipeizhi[15].length,"1");
			insert(s,jiaofeipeizhi[16].offset,jiaofeipeizhi[16].length,"        0.10");
			insert(s,jiaofeipeizhi[17].offset,jiaofeipeizhi[17].length,"0002");
			insert(s,jiaofeipeizhi[18].offset,jiaofeipeizhi[18].length,"");
			insert(s,jiaofeipeizhi[19].offset,jiaofeipeizhi[19].length,"");
			insert(s,jiaofeipeizhi[20].offset,jiaofeipeizhi[20].length,"");
			insert(s,jiaofeipeizhi[21].offset,jiaofeipeizhi[21].length,"");
		
	}
	else if(n == 2){//chexiao 0002
			
			insert(s,chexiaopeizhi[0].offset,chexiaopeizhi[0].length,"0370");
			insert(s,chexiaopeizhi[1].offset,chexiaopeizhi[1].length,"0002    ");
			insert(s,chexiaopeizhi[2].offset,chexiaopeizhi[2].length,"20130614000002100001");
			insert(s,chexiaopeizhi[3].offset,chexiaopeizhi[3].length,"20131126");
			insert(s,chexiaopeizhi[4].offset,chexiaopeizhi[4].length,"103106");
			insert(s,chexiaopeizhi[5].offset,chexiaopeizhi[5].length,"029 ");
			insert(s,chexiaopeizhi[6].offset,chexiaopeizhi[6].length,"BK0100000000");
			insert(s,chexiaopeizhi[7].offset,chexiaopeizhi[7].length,"11111111");
			insert(s,chexiaopeizhi[8].offset,chexiaopeizhi[8].length,"2002038789");
			insert(s,chexiaopeizhi[9].offset,chexiaopeizhi[9].length,"95598");
			insert(s,chexiaopeizhi[10].offset,chexiaopeizhi[10].length,"qudao");
			insert(s,chexiaopeizhi[11].offset,chexiaopeizhi[11].length,"");
			insert(s,chexiaopeizhi[12].offset,chexiaopeizhi[12].length,"");
			
			insert(s,chexiaopeizhi[13].offset,chexiaopeizhi[13].length,"2002038788");
			insert(s,chexiaopeizhi[14].offset,chexiaopeizhi[14].length,"20130614786500239984");
			insert(s,chexiaopeizhi[15].offset,chexiaopeizhi[15].length,"       25.01");
			insert(s,chexiaopeizhi[16].offset,chexiaopeizhi[16].length,"");
			insert(s,chexiaopeizhi[17].offset,chexiaopeizhi[17].length,"");
			insert(s,chexiaopeizhi[18].offset,chexiaopeizhi[18].length,"");
			insert(s,chexiaopeizhi[19].offset,chexiaopeizhi[19].length,"");
			
		}
		else if(n == 3)//duizhang 0099
		{
			insert(s,duizhangpeizhi[0].offset,duizhangpeizhi[0].length,"0798");
			insert(s,duizhangpeizhi[1].offset,duizhangpeizhi[1].length,"0099    ");	
			insert(s,duizhangpeizhi[2].offset,duizhangpeizhi[2].length,"20130614000000200002");	
			insert(s,duizhangpeizhi[3].offset,duizhangpeizhi[3].length,"20131126");	
			insert(s,duizhangpeizhi[4].offset,duizhangpeizhi[4].length,"103106");	
			insert(s,duizhangpeizhi[5].offset,duizhangpeizhi[5].length,"029 ");
			insert(s,duizhangpeizhi[6].offset,duizhangpeizhi[6].length,"BK0100000000");
			insert(s,duizhangpeizhi[7].offset,duizhangpeizhi[7].length,"11111111");
			insert(s,duizhangpeizhi[8].offset,duizhangpeizhi[8].length,"2002038789");
			insert(s,duizhangpeizhi[9].offset,duizhangpeizhi[9].length,"95598");
			insert(s,duizhangpeizhi[10].offset,duizhangpeizhi[10].length,"qudao");
			insert(s,duizhangpeizhi[11].offset,duizhangpeizhi[11].length,"");
			insert(s,duizhangpeizhi[12].offset,duizhangpeizhi[12].length,"");
			
			insert(s,duizhangpeizhi[13].offset,duizhangpeizhi[13].length,"0000");
			insert(s,duizhangpeizhi[14].offset,duizhangpeizhi[14].length,"1111");
			insert(s,duizhangpeizhi[15].offset,duizhangpeizhi[15].length,"filename");
			insert(s,duizhangpeizhi[16].offset,duizhangpeizhi[16].length,"0029");
			insert(s,duizhangpeizhi[17].offset,duizhangpeizhi[17].length,"0029");
			insert(s,duizhangpeizhi[18].offset,duizhangpeizhi[18].length,"0029");
			
		}
		else if(n == 4)//qianyue 0010
		{
				insert(s,qianjieyuepeizhi[0].offset,qianjieyuepeizhi[0].length,"1246");
				insert(s,qianjieyuepeizhi[1].offset,qianjieyuepeizhi[1].length,"2011");
				insert(s,qianjieyuepeizhi[2].offset,qianjieyuepeizhi[2].length,"20130614786500239986");
				insert(s,qianjieyuepeizhi[3].offset,qianjieyuepeizhi[3].length,"20131126");
				insert(s,qianjieyuepeizhi[4].offset,qianjieyuepeizhi[4].length,"103354");
				insert(s,qianjieyuepeizhi[5].offset,qianjieyuepeizhi[5].length,"029 ");
				insert(s,qianjieyuepeizhi[6].offset,qianjieyuepeizhi[6].length,"BK010001");
				insert(s,qianjieyuepeizhi[7].offset,qianjieyuepeizhi[7].length,"0000");
				insert(s,qianjieyuepeizhi[8].offset,qianjieyuepeizhi[8].length,"2002038788");
				insert(s,qianjieyuepeizhi[9].offset,qianjieyuepeizhi[9].length,"95598");
				insert(s,qianjieyuepeizhi[10].offset,qianjieyuepeizhi[10].length,"830");
				insert(s,qianjieyuepeizhi[11].offset,qianjieyuepeizhi[11].length,"");
				insert(s,qianjieyuepeizhi[12].offset,qianjieyuepeizhi[12].length,"");
				
				insert(s,qianjieyuepeizhi[13].offset,qianjieyuepeizhi[13].length,"2002038789");//jiaofeibianhao
				rand_num=(rand()+getpid()) % (3);
				insert(s,qianjieyuepeizhi[14].offset,qianjieyuepeizhi[14].length,"01");//01 02 03
				insert(s,qianjieyuepeizhi[15].offset,qianjieyuepeizhi[15].length,"1");
				insert(s,qianjieyuepeizhi[16].offset,qianjieyuepeizhi[16].length,"01");
				insert(s,qianjieyuepeizhi[17].offset,qianjieyuepeizhi[17].length,"1");
				insert(s,qianjieyuepeizhi[18].offset,qianjieyuepeizhi[18].length,"1");
				insert(s,qianjieyuepeizhi[19].offset,qianjieyuepeizhi[19].length,"1111111111");
				insert(s,qianjieyuepeizhi[20].offset,qianjieyuepeizhi[20].length,"陆勤付");
				insert(s,qianjieyuepeizhi[21].offset,qianjieyuepeizhi[21].length,"1");
				insert(s,qianjieyuepeizhi[22].offset,qianjieyuepeizhi[22].length,"1");
				insert(s,qianjieyuepeizhi[23].offset,qianjieyuepeizhi[23].length,"合肥供电公司");
				
				
				insert(s,qianjieyuepeizhi[24].offset,qianjieyuepeizhi[24].length,"00");//00 01
				insert(s,qianjieyuepeizhi[25].offset,qianjieyuepeizhi[25].length,"01");//00 else
				insert(s,qianjieyuepeizhi[26].offset,qianjieyuepeizhi[26].length,"1");
				insert(s,qianjieyuepeizhi[27].offset,qianjieyuepeizhi[27].length,"095598");
				insert(s,qianjieyuepeizhi[28].offset,qianjieyuepeizhi[28].length,"dianli");
				insert(s,qianjieyuepeizhi[29].offset,qianjieyuepeizhi[29].length,"000");
				insert(s,qianjieyuepeizhi[30].offset,qianjieyuepeizhi[30].length,"dianli");
				insert(s,qianjieyuepeizhi[31].offset,qianjieyuepeizhi[31].length,"");
				insert(s,qianjieyuepeizhi[32].offset,qianjieyuepeizhi[32].length,"");
				insert(s,qianjieyuepeizhi[33].offset,qianjieyuepeizhi[33].length,"");
				insert(s,qianjieyuepeizhi[34].offset,qianjieyuepeizhi[34].length,"");
		}
		else if(n == 5)//jieyue 0011
		{
				insert(s,qianjieyuepeizhi[0].offset,qianjieyuepeizhi[0].length,"478");
				insert(s,qianjieyuepeizhi[1].offset,qianjieyuepeizhi[1].length,"0011");
				insert(s,qianjieyuepeizhi[2].offset,qianjieyuepeizhi[2].length,"YYYYMMDD000000000001");
				insert(s,qianjieyuepeizhi[3].offset,qianjieyuepeizhi[3].length,"YYYYMMDD");
				insert(s,qianjieyuepeizhi[4].offset,qianjieyuepeizhi[4].length,"HHMMSS");
				insert(s,qianjieyuepeizhi[5].offset,qianjieyuepeizhi[5].length,"0029");
				insert(s,qianjieyuepeizhi[6].offset,qianjieyuepeizhi[6].length,"BK0100000000");
				insert(s,qianjieyuepeizhi[7].offset,qianjieyuepeizhi[7].length,"11111111");
				insert(s,qianjieyuepeizhi[8].offset,qianjieyuepeizhi[8].length,"22222222");
				insert(s,qianjieyuepeizhi[9].offset,qianjieyuepeizhi[9].length,"95598");
				insert(s,qianjieyuepeizhi[10].offset,qianjieyuepeizhi[10].length,"qudao");
				insert(s,qianjieyuepeizhi[11].offset,qianjieyuepeizhi[11].length,"");
				insert(s,qianjieyuepeizhi[12].offset,qianjieyuepeizhi[12].length,"");
				
				insert(s,qianjieyuepeizhi[13].offset,qianjieyuepeizhi[13].length,"1234567890");//gehuhao
				insert(s,qianjieyuepeizhi[14].offset,qianjieyuepeizhi[14].length,"12345678901");
				insert(s,qianjieyuepeizhi[15].offset,qianjieyuepeizhi[15].length,"000001");
				insert(s,qianjieyuepeizhi[16].offset,qianjieyuepeizhi[16].length,"kehumingcheng");//print
				insert(s,qianjieyuepeizhi[17].offset,qianjieyuepeizhi[17].length,"erji");//print
				insert(s,qianjieyuepeizhi[18].offset,qianjieyuepeizhi[18].length,"erjijigoumingcheng");//print
				insert(s,qianjieyuepeizhi[19].offset,qianjieyuepeizhi[19].length,"zuzhijigou");//print
				insert(s,qianjieyuepeizhi[20].offset,qianjieyuepeizhi[20].length,"guishu");
				insert(s,qianjieyuepeizhi[21].offset,qianjieyuepeizhi[21].length,"guishu");
				insert(s,qianjieyuepeizhi[22].offset,qianjieyuepeizhi[22].length,"0");
				insert(s,qianjieyuepeizhi[23].offset,qianjieyuepeizhi[23].length,"5");
				insert(s,qianjieyuepeizhi[24].offset,qianjieyuepeizhi[24].length,"0010");
				insert(s,qianjieyuepeizhi[25].offset,qianjieyuepeizhi[25].length,"opname");
				insert(s,qianjieyuepeizhi[26].offset,qianjieyuepeizhi[26].length,"031000");//id num
				insert(s,qianjieyuepeizhi[27].offset,qianjieyuepeizhi[27].length,"lianxi");
				insert(s,qianjieyuepeizhi[28].offset,qianjieyuepeizhi[28].length,"01");
		}
		else if(n ==6)//yanmi 0020
		{
						insert(s,yanmipeizhi[0].offset,yanmipeizhi[0].length,"0242");
			insert(s,yanmipeizhi[1].offset,yanmipeizhi[1].length,"0020    ");	
			insert(s,yanmipeizhi[2].offset,yanmipeizhi[2].length,"YYYYMMDD000000000001");	
			insert(s,yanmipeizhi[3].offset,yanmipeizhi[3].length,"YYYYMMDD");	
			insert(s,yanmipeizhi[4].offset,yanmipeizhi[4].length,"HHMMSS");	
			insert(s,yanmipeizhi[5].offset,yanmipeizhi[5].length,"0029");
			insert(s,yanmipeizhi[6].offset,yanmipeizhi[6].length,"BK0100000000");
			insert(s,yanmipeizhi[7].offset,yanmipeizhi[7].length,"11111111");
			insert(s,yanmipeizhi[8].offset,yanmipeizhi[8].length,"22222222");
			insert(s,yanmipeizhi[9].offset,yanmipeizhi[9].length,"95598");
			insert(s,yanmipeizhi[10].offset,yanmipeizhi[10].length,"qudao");
			insert(s,yanmipeizhi[11].offset,yanmipeizhi[11].length,"");
			insert(s,yanmipeizhi[12].offset,yanmipeizhi[12].length,"");
			
			insert(s,yanmipeizhi[13].offset,yanmipeizhi[13].length,"1234567890");
			insert(s,yanmipeizhi[14].offset,yanmipeizhi[14].length,"yijijigou");
			insert(s,yanmipeizhi[15].offset,yanmipeizhi[15].length,"erji");
			insert(s,yanmipeizhi[16].offset,yanmipeizhi[16].length,"0002");
			insert(s,yanmipeizhi[17].offset,yanmipeizhi[17].length,"mima");
			
		}
		else if(n == 7)//gaimi
		{
					insert(s,gaimipeizhi[0].offset,gaimipeizhi[0].length,"0274");
			insert(s,gaimipeizhi[1].offset,gaimipeizhi[1].length,"0021    ");	
			insert(s,gaimipeizhi[2].offset,gaimipeizhi[2].length,"YYYYMMDD000000000001");	
			insert(s,gaimipeizhi[3].offset,gaimipeizhi[3].length,"YYYYMMDD");	
			insert(s,gaimipeizhi[4].offset,gaimipeizhi[4].length,"HHMMSS");	
			insert(s,gaimipeizhi[5].offset,gaimipeizhi[5].length,"0029");
			insert(s,gaimipeizhi[6].offset,gaimipeizhi[6].length,"BK0100000000");
			insert(s,gaimipeizhi[7].offset,gaimipeizhi[7].length,"11111111");
			insert(s,gaimipeizhi[8].offset,gaimipeizhi[8].length,"22222222");
			insert(s,gaimipeizhi[9].offset,gaimipeizhi[9].length,"95598");
			insert(s,gaimipeizhi[10].offset,gaimipeizhi[10].length,"qudao");
			insert(s,gaimipeizhi[11].offset,gaimipeizhi[11].length,"");
			insert(s,gaimipeizhi[12].offset,gaimipeizhi[12].length,"");
			
			insert(s,gaimipeizhi[13].offset,gaimipeizhi[13].length,"1234567890");
			insert(s,gaimipeizhi[14].offset,gaimipeizhi[14].length,"yijijigou");
			insert(s,gaimipeizhi[15].offset,gaimipeizhi[15].length,"erji");
			insert(s,gaimipeizhi[16].offset,gaimipeizhi[16].length,"0002");
			insert(s,gaimipeizhi[17].offset,gaimipeizhi[17].length,"old_mima");
			insert(s,gaimipeizhi[18].offset,gaimipeizhi[18].length,"new_mima");
		
		}
		else if(n==8)//dukachaxun
		{
			insert(s,readcardpeizhi[0].offset,readcardpeizhi[0].length,"1858");
			insert(s,readcardpeizhi[1].offset,readcardpeizhi[1].length,"1000    ");	
			insert(s,readcardpeizhi[2].offset,readcardpeizhi[2].length,"YYYYMMDD000000000001");	
			insert(s,readcardpeizhi[3].offset,readcardpeizhi[3].length,"YYYYMMDD");	
			insert(s,readcardpeizhi[4].offset,readcardpeizhi[4].length,"HHMMSS");	
			insert(s,readcardpeizhi[5].offset,readcardpeizhi[5].length,"0029");
			insert(s,readcardpeizhi[6].offset,readcardpeizhi[6].length,"BK0100000000");
			insert(s,readcardpeizhi[7].offset,readcardpeizhi[7].length,"11111111");
			insert(s,readcardpeizhi[8].offset,readcardpeizhi[8].length,"22222222");
			insert(s,readcardpeizhi[9].offset,readcardpeizhi[9].length,"95598");
			insert(s,readcardpeizhi[10].offset,readcardpeizhi[10].length,"qudao");
			insert(s,readcardpeizhi[11].offset,readcardpeizhi[11].length,"");
			insert(s,readcardpeizhi[12].offset,readcardpeizhi[12].length,"");
			
				
			insert(s,readcardpeizhi[13].offset,readcardpeizhi[13].length,"kaxinxiwenjian");
			insert(s,readcardpeizhi[14].offset,readcardpeizhi[14].length,"qianbaowenjian");
			insert(s,readcardpeizhi[15].offset,readcardpeizhi[15].length,"feilvwenjian1");
			insert(s,readcardpeizhi[16].offset,readcardpeizhi[16].length,"feilvwenjian2");
			insert(s,readcardpeizhi[17].offset,readcardpeizhi[17].length,"fanxiequwenjian");
			insert(s,readcardpeizhi[18].offset,readcardpeizhi[18].length,"40");
			insert(s,readcardpeizhi[19].offset,readcardpeizhi[19].length,"40");
			insert(s,readcardpeizhi[20].offset,readcardpeizhi[20].length,"40");
			insert(s,readcardpeizhi[21].offset,readcardpeizhi[21].length,"40");
			insert(s,readcardpeizhi[22].offset,readcardpeizhi[22].length,"256");
			insert(s,readcardpeizhi[23].offset,readcardpeizhi[23].length,"256");
			insert(s,readcardpeizhi[24].offset,readcardpeizhi[24].length,"256");
			
		}
}

int main(int argc, char *argv[]){
	
	
	
	char buf[BUFFER_SIZE];
	char recvbuf[BUFFER_SIZE];
	
	int sockfd, sendbytes,recvbytes;

	struct hostent *host;
	struct sockaddr_in serv_addr;
	pid_t sock_mount;
	signal(SIGCHLD, sigchld_handler); 
	
	
	
	
		
		/*do send package*/

						if (argc < 3) {
							fprintf(stderr,"Please enter the server's hostname! ");
							exit(1);
						}
						if((host=gethostbyname(argv[1]))==NULL) {
							perror("gethostbyname");
							exit(1);
						}

						//sprintf(buf,"%s",argv[2]);
						/****the mount of sockets***/
						int j;
						for(j=0;j<SOCK_AMOUNT;j++)
						{
							sock_mount=fork();
						}

						
							if(sock_mount==-1)
							printf("fork error");
						
							
						 
							
							
						if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
							perror("socket");
							exit(1);
						}

						serv_addr.sin_family=AF_INET;
						serv_addr.sin_port=htons(PORT);

						serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
						bzero(&(serv_addr.sin_zero),8);


						

					
						//else if(result == 0){

						if (connect(sockfd, (struct sockaddr *)&serv_addr,
						sizeof(struct sockaddr)) == -1) 
						{
								perror("connect");
								exit(1);
						}
					
			
				
			
						srand((int) time(0));
						rand_num=(rand()+sock_mount) % (5);//chansheng suiji shu
						/*size_t inlen,outlen;
						char buftemp[BUFFER_SIZE];
						memset(buftemp,'\0',sizeof(buftemp));
						makepackage(4,buftemp);
						inlen=strlen(buftemp);
						outlen=inlen;
						code_convert("utf-8","gb2312",buftemp,inlen,buf,outlen);
								*/
								memset(buf,'\0',sizeof(buf));
						makepackage(1,buf);
								if ((sendbytes=send(sockfd, buf, strlen(buf), 0)) ==-1) 
								{
						
										perror("send");
										exit(1);
								}
								printf("------------[%d]----[%s]\n",sendbytes,buf);
								memset(buf,'\0',sizeof(buf));
							
							
								
							
								memset(recvbuf,'\0',sizeof(recvbuf));
								if((recvbytes = recv(sockfd,recvbuf,BUFFER_SIZE,0))<0)
								{
									
										perror("recv");
										exit(1);
									
								}
									printf("------------[%d]--[%s]\n",recvbytes,recvbuf);
										memset(recvbuf,'\0',sizeof(recvbuf));
								
							
							
							close(sockfd);
				
					

	exit(0);
}
