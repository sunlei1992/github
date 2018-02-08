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

#define PORT 5678
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
{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{8,182},{20,190},{8,210},{40,218},{40,258},{256,298}};

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

/*读卡信息查询银行出参*/
typedef struct du_ka_cha_xun_from_yinhang_zi_duan
{
	int length;
	int offset;
}du_ka_cha_xun_from_yinhang_zi_duan;
du_ka_cha_xun_from_yinhang_zi_duan dukachaxunfromyinhangpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},
{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{100,182},{40,282},{256,322},{256,578},{100,834},{40,934},{40,974},
{40,1014},{40,1054},{256,1094},{256,1350},{256,1606}};
#define DU_KA_CHA_XUN_FROM_YINHANG_MAX_ZI_DUAN_LENGTH 512   //读卡信息查询银行出参一个字段最大的字节数
#define DU_KA_CHA_XUN_FROM_YINHANG_REAL_QUE_ZI_DUAN 25     //读卡信息查询银行出参实际的字段数量
/*卡表够电银行出参*/
typedef struct ka_biao_gou_dian_from_yinhang_zi_duan
{
	int length;
	int offset;
}ka_biao_gou_dian_from_yinhang_zi_duan;
ka_biao_gou_dian_from_yinhang_zi_duan kabiaogoudianfromyinhangpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},
{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{40,182},{40,222},{12,262},{256,274},{20,530},{12,550},
{20,562},{40,582},{40,622},{40,662},{40,702},{256,742}};//994
#define KA_BIAO_GOU_DIAN_FROM_YINHANG_MAX_ZI_DUAN_LENGTH 512   //卡表够电查询银行出参一个字段最大的字节数
#define KA_BIAO_GOU_DIAN_FROM_YINHANG_REAL_QUE_ZI_DUAN 25     //卡表够电查询银行出参实际的字段数量
/*卡表够电冲正银行出参*/
typedef struct ka_biao_gou_dian_chong_zheng_from_yinhang_zi_duan
{
	int length;
	int offset;
}ka_biao_gou_dian_chong_zheng_from_yinhang_zi_duan;
ka_biao_gou_dian_chong_zheng_from_yinhang_zi_duan kabiaogoudianchongzhengfromyinhangpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{8,4},{20,12},{8,32},
{6,40},{4,46},{12,50},{8,62},{8,70},{12,78},{12,90},{40,102},{40,142},{40,182},{20,222},{20,242},{100,262},
{40,362},{256,402},{256,658},{100,914},{40,1014},{40,1054},{40,1094},{40,1134},{256,1174},{256,1430},{256,1686},{40,1942},{256,1982}};//2194
#define KA_BIAO_GOU_DIAN_CHONG_ZHENG_FROM_YINHANG_MAX_ZI_DUAN_LENGTH 512   //卡表够电冲正银行出参一个字段最大的字节数
#define KA_BIAO_GOU_DIAN_CHONG_ZHENG_FROM_YINHANG_REAL_QUE_ZI_DUAN 30     //卡表够电冲正银行出参实际的字段数量


/*补写卡银行出参*/
typedef struct bu_xie_ka_from_yinhang_zi_duan
{
	int length;
	int offset;
}bu_xie_ka_from_yinhang_zi_duan;
bu_xie_ka_from_yinhang_zi_duan buxiekafromyinhangpeizhi[MAX_QUE_ZI_DUAN] = {{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},
{8,70},{12,78},{12,90},{40,102},{40,142},{8,182},{100,190},{40,290},{256,330},{256,586},{100,842},
{40,942},{40,982},{256,1022},{40,1278},{40,1318},{256,1358},{256,1614}};//1866 1614
#define BU_XIE_KA_FROM_YINHANG_MAX_ZI_DUAN_LENGTH 512   //补写卡银行出参一个字段最大的字节数
#define BU_XIE_KA_FROM_YINHANG_REAL_QUE_ZI_DUAN 26     //卡补写银行出参实际的字段数量

/*票据查询*/
typedef struct piao_ju_cha_xun_from_yinhang_zi_duan
{
	int length;
	int offset;
}piao_ju_cha_xun_from_yinhang_zi_duan;
piao_ju_cha_xun_from_yinhang_zi_duan piaojuchaxunfromyinhangpeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},
{8,70},{12,78},{12,90},{40,102},{40,142},{20,182},{40,202},{40,242},{256,282}};

/*票据打印*/
typedef struct piao_ju_da_yin_from_yinhang_zi_duan
{
	int length;
	int offset;
}piao_ju_da_yin_from_yinhang_zi_duan;
piao_ju_da_yin_from_yinhang_zi_duan piaojudayinfromyinhangpeizhi[MAX_QUE_ZI_DUAN]={{4,0},{8,4},{20,12},{8,32},{6,40},{4,46},{12,50},{8,62},
{8,70},{12,78},{12,90},{40,102},{40,142},{20,182},{16,202},{40,218},{256,258}};



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
void makepackage(char* n,char *s)//
{
	if(strcmp(n,"0")==0)//chaxun 0000
	{
			insert(s,qianfeipeizhi[0].offset,qianfeipeizhi[0].length,"0222");
			insert(s,qianfeipeizhi[1].offset,qianfeipeizhi[1].length,"0000    ");
			insert(s,qianfeipeizhi[2].offset,qianfeipeizhi[2].length,"2013061478650023");
			insert(s,qianfeipeizhi[3].offset,qianfeipeizhi[3].length,"20131126");
			insert(s,qianfeipeizhi[4].offset,qianfeipeizhi[4].length,"103333");
			insert(s,qianfeipeizhi[5].offset,qianfeipeizhi[5].length,"029 ");
			insert(s,qianfeipeizhi[6].offset,qianfeipeizhi[6].length,"BK010001");
			insert(s,qianfeipeizhi[7].offset,qianfeipeizhi[7].length,"0000");
			insert(s,qianfeipeizhi[8].offset,qianfeipeizhi[8].length,"2002038791");
			insert(s,qianfeipeizhi[9].offset,qianfeipeizhi[9].length,"95598");
			insert(s,qianfeipeizhi[10].offset,qianfeipeizhi[10].length,"830");
			insert(s,qianfeipeizhi[11].offset,qianfeipeizhi[11].length,"");
			insert(s,qianfeipeizhi[12].offset,qianfeipeizhi[12].length,"");
			
			insert(s,qianfeipeizhi[13].offset,qianfeipeizhi[13].length,"2002038788");
			insert(s,qianfeipeizhi[14].offset,qianfeipeizhi[14].length,"0000");
	}
	else if(strcmp(n,"1")==0)//jiaofei 0001
	{
			insert(s,jiaofeipeizhi[0].offset,jiaofeipeizhi[0].length,"0618");
			insert(s,jiaofeipeizhi[1].offset,jiaofeipeizhi[1].length,"0001    ");
			insert(s,jiaofeipeizhi[2].offset,jiaofeipeizhi[2].length,"2013061478650030");
			insert(s,jiaofeipeizhi[3].offset,jiaofeipeizhi[3].length,"20131126");
			insert(s,jiaofeipeizhi[4].offset,jiaofeipeizhi[4].length,"103106");
			insert(s,jiaofeipeizhi[5].offset,jiaofeipeizhi[5].length,"029 ");
			insert(s,jiaofeipeizhi[6].offset,jiaofeipeizhi[6].length,"BK010112");
			insert(s,jiaofeipeizhi[7].offset,jiaofeipeizhi[7].length,"0000");
			insert(s,jiaofeipeizhi[8].offset,jiaofeipeizhi[8].length,"2002038789");
			insert(s,jiaofeipeizhi[9].offset,jiaofeipeizhi[9].length,"95598");
			insert(s,jiaofeipeizhi[10].offset,jiaofeipeizhi[10].length,"830");
			insert(s,jiaofeipeizhi[11].offset,jiaofeipeizhi[11].length,"");
			insert(s,jiaofeipeizhi[12].offset,jiaofeipeizhi[12].length,"");
			
			insert(s,jiaofeipeizhi[13].offset,jiaofeipeizhi[13].length,"2002038788");
			insert(s,jiaofeipeizhi[14].offset,jiaofeipeizhi[14].length,"0000");
			insert(s,jiaofeipeizhi[15].offset,jiaofeipeizhi[15].length,"1");
			insert(s,jiaofeipeizhi[16].offset,jiaofeipeizhi[16].length,"000000016.30");
			insert(s,jiaofeipeizhi[17].offset,jiaofeipeizhi[17].length,"0002");
			insert(s,jiaofeipeizhi[18].offset,jiaofeipeizhi[18].length,"");
			insert(s,jiaofeipeizhi[19].offset,jiaofeipeizhi[19].length,"");
			insert(s,jiaofeipeizhi[20].offset,jiaofeipeizhi[20].length,"");
			insert(s,jiaofeipeizhi[21].offset,jiaofeipeizhi[21].length,"");
		
	}
	else if(strcmp(n,"2")==0){//chexiao 0002
			
			insert(s,chexiaopeizhi[0].offset,chexiaopeizhi[0].length,"0370");
			insert(s,chexiaopeizhi[1].offset,chexiaopeizhi[1].length,"0002    ");
			insert(s,chexiaopeizhi[2].offset,chexiaopeizhi[2].length,"2013061400000211");
			insert(s,chexiaopeizhi[3].offset,chexiaopeizhi[3].length,"20131126");
			insert(s,chexiaopeizhi[4].offset,chexiaopeizhi[4].length,"103106");
			insert(s,chexiaopeizhi[5].offset,chexiaopeizhi[5].length,"029 ");
			insert(s,chexiaopeizhi[6].offset,chexiaopeizhi[6].length,"BK010112");
			insert(s,chexiaopeizhi[7].offset,chexiaopeizhi[7].length,"11111111");
			insert(s,chexiaopeizhi[8].offset,chexiaopeizhi[8].length,"2002038789");
			insert(s,chexiaopeizhi[9].offset,chexiaopeizhi[9].length,"95598");
			insert(s,chexiaopeizhi[10].offset,chexiaopeizhi[10].length,"qudao");
			insert(s,chexiaopeizhi[11].offset,chexiaopeizhi[11].length,"");
			insert(s,chexiaopeizhi[12].offset,chexiaopeizhi[12].length,"");
			
			insert(s,chexiaopeizhi[13].offset,chexiaopeizhi[13].length,"2002038788");
			insert(s,chexiaopeizhi[14].offset,chexiaopeizhi[14].length,"2013061478650023");
			insert(s,chexiaopeizhi[15].offset,chexiaopeizhi[15].length,"1");
			insert(s,chexiaopeizhi[16].offset,chexiaopeizhi[16].length,"");
			insert(s,chexiaopeizhi[17].offset,chexiaopeizhi[17].length,"");
			insert(s,chexiaopeizhi[18].offset,chexiaopeizhi[18].length,"");
			insert(s,chexiaopeizhi[19].offset,chexiaopeizhi[19].length,"");
			
		}
		else if(strcmp(n,"3")==0)//duizhang 0099
		{
			insert(s,duizhangpeizhi[0].offset,duizhangpeizhi[0].length,"0550");
			insert(s,duizhangpeizhi[1].offset,duizhangpeizhi[1].length,"0098    ");	
			insert(s,duizhangpeizhi[2].offset,duizhangpeizhi[2].length,"2013061400000022");	
			insert(s,duizhangpeizhi[3].offset,duizhangpeizhi[3].length,"20140514");	
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
			insert(s,duizhangpeizhi[15].offset,duizhangpeizhi[15].length,"20140314");
			insert(s,duizhangpeizhi[16].offset,duizhangpeizhi[16].length,"0029");
			insert(s,duizhangpeizhi[17].offset,duizhangpeizhi[17].length,"0029");
			insert(s,duizhangpeizhi[18].offset,duizhangpeizhi[18].length,"0029");
			
		}
		else if(strcmp(n,"4")==0)//qianyue 0010
		{
				insert(s,qianjieyuepeizhi[0].offset,qianjieyuepeizhi[0].length,"1246");
				insert(s,qianjieyuepeizhi[1].offset,qianjieyuepeizhi[1].length,"2011");
				insert(s,qianjieyuepeizhi[2].offset,qianjieyuepeizhi[2].length,"2013061478650023");
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
		else if(strcmp(n,"5")==0)//jieyue 0011
		{
				insert(s,qianjieyuepeizhi[0].offset,qianjieyuepeizhi[0].length,"478");
				insert(s,qianjieyuepeizhi[1].offset,qianjieyuepeizhi[1].length,"0011");
				insert(s,qianjieyuepeizhi[2].offset,qianjieyuepeizhi[2].length,"YYYYMMDD00000000");
				insert(s,qianjieyuepeizhi[3].offset,qianjieyuepeizhi[3].length,"YYYYMMDD");
				insert(s,qianjieyuepeizhi[4].offset,qianjieyuepeizhi[4].length,"HHMMSS");
				insert(s,qianjieyuepeizhi[5].offset,qianjieyuepeizhi[5].length,"029 ");
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
		else if(strcmp(n,"6")==0)//读卡信息查询
		{
				insert(s,dukachaxunfromyinhangpeizhi[0].offset,dukachaxunfromyinhangpeizhi[0].length,"1858");
				insert(s,dukachaxunfromyinhangpeizhi[1].offset,dukachaxunfromyinhangpeizhi[1].length,"1000");	
				insert(s,dukachaxunfromyinhangpeizhi[2].offset,dukachaxunfromyinhangpeizhi[2].length,"20130614248724");	
				insert(s,dukachaxunfromyinhangpeizhi[3].offset,dukachaxunfromyinhangpeizhi[3].length,"20140219");	
				insert(s,dukachaxunfromyinhangpeizhi[4].offset,dukachaxunfromyinhangpeizhi[4].length,"094033");	
				insert(s,dukachaxunfromyinhangpeizhi[5].offset,dukachaxunfromyinhangpeizhi[5].length,"0917");
				insert(s,dukachaxunfromyinhangpeizhi[6].offset,dukachaxunfromyinhangpeizhi[6].length,"BK010112");
				insert(s,dukachaxunfromyinhangpeizhi[7].offset,dukachaxunfromyinhangpeizhi[7].length,"0000");
				insert(s,dukachaxunfromyinhangpeizhi[8].offset,dukachaxunfromyinhangpeizhi[8].length,"0000");
				insert(s,dukachaxunfromyinhangpeizhi[9].offset,dukachaxunfromyinhangpeizhi[9].length,"95598");
				insert(s,dukachaxunfromyinhangpeizhi[10].offset,dukachaxunfromyinhangpeizhi[10].length,"830");
				insert(s,dukachaxunfromyinhangpeizhi[11].offset,dukachaxunfromyinhangpeizhi[11].length,"");
				insert(s,dukachaxunfromyinhangpeizhi[12].offset,dukachaxunfromyinhangpeizhi[12].length,"");
				
				insert(s,dukachaxunfromyinhangpeizhi[13].offset,dukachaxunfromyinhangpeizhi[13].length,"68010027028300000001000000000004000010000000000000000100000100121801037100635000576501d316");
				insert(s,dukachaxunfromyinhangpeizhi[14].offset,dukachaxunfromyinhangpeizhi[14].length,"00001c2000000001");
				insert(s,dukachaxunfromyinhangpeizhi[15].offset,dukachaxunfromyinhangpeizhi[15].length,"680100fc00004983000049830000498300004983000049830000498300004983000049830000498300004983000049830000498300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
				insert(s,dukachaxunfromyinhangpeizhi[16].offset,dukachaxunfromyinhangpeizhi[16].length,"680100fc00004983000049830000498300004983000049830000498300004983000049830000498300004983000049830000498300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
				insert(s,dukachaxunfromyinhangpeizhi[17].offset,dukachaxunfromyinhangpeizhi[17].length,"6811002b04000001000001001218010371006350005765000027d80000000100000000010004010000001401211115b216");
				insert(s,dukachaxunfromyinhangpeizhi[18].offset,dukachaxunfromyinhangpeizhi[18].length,"");
				insert(s,dukachaxunfromyinhangpeizhi[19].offset,dukachaxunfromyinhangpeizhi[19].length,"");
				insert(s,dukachaxunfromyinhangpeizhi[20].offset,dukachaxunfromyinhangpeizhi[20].length,"");
				insert(s,dukachaxunfromyinhangpeizhi[21].offset,dukachaxunfromyinhangpeizhi[21].length,"");
				insert(s,dukachaxunfromyinhangpeizhi[22].offset,dukachaxunfromyinhangpeizhi[22].length,"");
				insert(s,dukachaxunfromyinhangpeizhi[23].offset,dukachaxunfromyinhangpeizhi[23].length,"");
				insert(s,dukachaxunfromyinhangpeizhi[24].offset,dukachaxunfromyinhangpeizhi[24].length,"");
		}
		else if(strcmp(n,"7")==0)//卡表购电
		{
				insert(s,kabiaogoudianfromyinhangpeizhi[0].offset,kabiaogoudianfromyinhangpeizhi[0].length,"0994");
				insert(s,kabiaogoudianfromyinhangpeizhi[1].offset,kabiaogoudianfromyinhangpeizhi[1].length,"1001");	
				insert(s,kabiaogoudianfromyinhangpeizhi[2].offset,kabiaogoudianfromyinhangpeizhi[2].length,"2013061478650034");	
				insert(s,kabiaogoudianfromyinhangpeizhi[3].offset,kabiaogoudianfromyinhangpeizhi[3].length,"20131126");	
				insert(s,kabiaogoudianfromyinhangpeizhi[4].offset,kabiaogoudianfromyinhangpeizhi[4].length,"103354");	
				insert(s,kabiaogoudianfromyinhangpeizhi[5].offset,kabiaogoudianfromyinhangpeizhi[5].length,"029 ");
				insert(s,kabiaogoudianfromyinhangpeizhi[6].offset,kabiaogoudianfromyinhangpeizhi[6].length,"BK010112");
				insert(s,kabiaogoudianfromyinhangpeizhi[7].offset,kabiaogoudianfromyinhangpeizhi[7].length,"0000");
				insert(s,kabiaogoudianfromyinhangpeizhi[8].offset,kabiaogoudianfromyinhangpeizhi[8].length,"2002038788");
				insert(s,kabiaogoudianfromyinhangpeizhi[9].offset,kabiaogoudianfromyinhangpeizhi[9].length,"95598");
				insert(s,kabiaogoudianfromyinhangpeizhi[10].offset,kabiaogoudianfromyinhangpeizhi[10].length,"830");
				insert(s,kabiaogoudianfromyinhangpeizhi[11].offset,kabiaogoudianfromyinhangpeizhi[11].length,"");
				insert(s,kabiaogoudianfromyinhangpeizhi[12].offset,kabiaogoudianfromyinhangpeizhi[12].length,"");
				
				insert(s,kabiaogoudianfromyinhangpeizhi[13].offset,kabiaogoudianfromyinhangpeizhi[13].length,"1234567890");//电力：用户编号（非卡号）
				insert(s,kabiaogoudianfromyinhangpeizhi[14].offset,kabiaogoudianfromyinhangpeizhi[14].length,"1224243244");//资金结算编号
				insert(s,kabiaogoudianfromyinhangpeizhi[15].offset,kabiaogoudianfromyinhangpeizhi[15].length,"000000000123");//缴费金额
				insert(s,kabiaogoudianfromyinhangpeizhi[16].offset,kabiaogoudianfromyinhangpeizhi[16].length,"卡密文件");//卡密文件
				insert(s,kabiaogoudianfromyinhangpeizhi[17].offset,kabiaogoudianfromyinhangpeizhi[17].length,"用户卡序列号");//用户卡序列号
				insert(s,kabiaogoudianfromyinhangpeizhi[18].offset,kabiaogoudianfromyinhangpeizhi[18].length,"用户卡随机数");
				insert(s,kabiaogoudianfromyinhangpeizhi[19].offset,kabiaogoudianfromyinhangpeizhi[19].length,"电能表标识");
				insert(s,kabiaogoudianfromyinhangpeizhi[20].offset,kabiaogoudianfromyinhangpeizhi[20].length,"");
				insert(s,kabiaogoudianfromyinhangpeizhi[21].offset,kabiaogoudianfromyinhangpeizhi[21].length,"");
				insert(s,kabiaogoudianfromyinhangpeizhi[22].offset,kabiaogoudianfromyinhangpeizhi[22].length,"");
				insert(s,kabiaogoudianfromyinhangpeizhi[23].offset,kabiaogoudianfromyinhangpeizhi[23].length,"");
				insert(s,kabiaogoudianfromyinhangpeizhi[24].offset,kabiaogoudianfromyinhangpeizhi[24].length,"");
			
		}
		else if(strcmp(n,"8")==0)//卡表 购电冲正
		{
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[0].offset,kabiaogoudianchongzhengfromyinhangpeizhi[0].length,"2234");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[1].offset,kabiaogoudianchongzhengfromyinhangpeizhi[1].length,"1002");	
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[2].offset,kabiaogoudianchongzhengfromyinhangpeizhi[2].length,"2013061478650036");	
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[3].offset,kabiaogoudianchongzhengfromyinhangpeizhi[3].length,"20131126");	
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[4].offset,kabiaogoudianchongzhengfromyinhangpeizhi[4].length,"103354");	
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[5].offset,kabiaogoudianchongzhengfromyinhangpeizhi[5].length,"029 ");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[6].offset,kabiaogoudianchongzhengfromyinhangpeizhi[6].length,"BK010001");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[7].offset,kabiaogoudianchongzhengfromyinhangpeizhi[7].length,"0000");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[8].offset,kabiaogoudianchongzhengfromyinhangpeizhi[8].length,"2002038788");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[9].offset,kabiaogoudianchongzhengfromyinhangpeizhi[9].length,"95598");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[10].offset,kabiaogoudianchongzhengfromyinhangpeizhi[10].length,"830");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[11].offset,kabiaogoudianchongzhengfromyinhangpeizhi[11].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[12].offset,kabiaogoudianchongzhengfromyinhangpeizhi[12].length,"");
				
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[13].offset,kabiaogoudianchongzhengfromyinhangpeizhi[13].length,"987654321");//用户编号 40	
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[14].offset,kabiaogoudianchongzhengfromyinhangpeizhi[14].length,"2013061478650033");//原交易流水
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[15].offset,kabiaogoudianchongzhengfromyinhangpeizhi[15].length,"01");//电能卡标识
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[16].offset,kabiaogoudianchongzhengfromyinhangpeizhi[16].length,"卡信息文件");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[17].offset,kabiaogoudianchongzhengfromyinhangpeizhi[17].length,"钱包文件");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[18].offset,kabiaogoudianchongzhengfromyinhangpeizhi[18].length,"费率文件1");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[19].offset,kabiaogoudianchongzhengfromyinhangpeizhi[19].length,"费率文件2");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[20].offset,kabiaogoudianchongzhengfromyinhangpeizhi[20].length,"返写区文件");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[21].offset,kabiaogoudianchongzhengfromyinhangpeizhi[21].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[22].offset,kabiaogoudianchongzhengfromyinhangpeizhi[22].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[23].offset,kabiaogoudianchongzhengfromyinhangpeizhi[23].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[24].offset,kabiaogoudianchongzhengfromyinhangpeizhi[24].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[25].offset,kabiaogoudianchongzhengfromyinhangpeizhi[25].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[26].offset,kabiaogoudianchongzhengfromyinhangpeizhi[26].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[27].offset,kabiaogoudianchongzhengfromyinhangpeizhi[27].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[28].offset,kabiaogoudianchongzhengfromyinhangpeizhi[28].length,"");
				insert(s,kabiaogoudianchongzhengfromyinhangpeizhi[29].offset,kabiaogoudianchongzhengfromyinhangpeizhi[29].length,"");

		}
		else if(strcmp(n,"9")==0)
		{
				insert(s,buxiekafromyinhangpeizhi[0].offset,buxiekafromyinhangpeizhi[0].length,"1866");
				insert(s,buxiekafromyinhangpeizhi[1].offset,buxiekafromyinhangpeizhi[1].length,"1003");
				insert(s,buxiekafromyinhangpeizhi[2].offset,buxiekafromyinhangpeizhi[2].length,"2013061478650023");
				insert(s,buxiekafromyinhangpeizhi[3].offset,buxiekafromyinhangpeizhi[3].length,"20131126");
				insert(s,buxiekafromyinhangpeizhi[4].offset,buxiekafromyinhangpeizhi[4].length,"103354");
				insert(s,buxiekafromyinhangpeizhi[5].offset,buxiekafromyinhangpeizhi[5].length,"029 ");
				insert(s,buxiekafromyinhangpeizhi[6].offset,buxiekafromyinhangpeizhi[6].length,"BK010001");
				insert(s,buxiekafromyinhangpeizhi[7].offset,buxiekafromyinhangpeizhi[7].length,"0000");
				insert(s,buxiekafromyinhangpeizhi[8].offset,buxiekafromyinhangpeizhi[8].length,"2002038788");
				insert(s,buxiekafromyinhangpeizhi[9].offset,buxiekafromyinhangpeizhi[9].length,"95598");
				insert(s,buxiekafromyinhangpeizhi[10].offset,buxiekafromyinhangpeizhi[10].length,"830");
				insert(s,buxiekafromyinhangpeizhi[11].offset,buxiekafromyinhangpeizhi[11].length,"");
				insert(s,buxiekafromyinhangpeizhi[12].offset,buxiekafromyinhangpeizhi[12].length,"");


				insert(s,buxiekafromyinhangpeizhi[13].offset,buxiekafromyinhangpeizhi[13].length,"01");
				insert(s,buxiekafromyinhangpeizhi[14].offset,buxiekafromyinhangpeizhi[14].length,"卡信息文件");
				insert(s,buxiekafromyinhangpeizhi[15].offset,buxiekafromyinhangpeizhi[15].length,"钱包文件");
				insert(s,buxiekafromyinhangpeizhi[16].offset,buxiekafromyinhangpeizhi[16].length,"费率文件1");
				insert(s,buxiekafromyinhangpeizhi[17].offset,buxiekafromyinhangpeizhi[17].length,"费率文件2");
				insert(s,buxiekafromyinhangpeizhi[18].offset,buxiekafromyinhangpeizhi[18].length,"返写区文件");
				insert(s,buxiekafromyinhangpeizhi[19].offset,buxiekafromyinhangpeizhi[19].length,"用户卡序号");
				insert(s,buxiekafromyinhangpeizhi[20].offset,buxiekafromyinhangpeizhi[20].length,"用户卡随机数");
				insert(s,buxiekafromyinhangpeizhi[21].offset,buxiekafromyinhangpeizhi[21].length,"卡表密文信息");
				insert(s,buxiekafromyinhangpeizhi[22].offset,buxiekafromyinhangpeizhi[22].length,"");
				insert(s,buxiekafromyinhangpeizhi[23].offset,buxiekafromyinhangpeizhi[23].length,"");
				insert(s,buxiekafromyinhangpeizhi[24].offset,buxiekafromyinhangpeizhi[24].length,"");
				insert(s,buxiekafromyinhangpeizhi[25].offset,buxiekafromyinhangpeizhi[25].length,"");
			
		}
			else if(strcmp(n,"10")==0)
		{
				insert(s,piaojuchaxunfromyinhangpeizhi[0].offset,piaojuchaxunfromyinhangpeizhi[0].length,"0534");
				insert(s,piaojuchaxunfromyinhangpeizhi[1].offset,piaojuchaxunfromyinhangpeizhi[1].length,"0022");
				insert(s,piaojuchaxunfromyinhangpeizhi[2].offset,piaojuchaxunfromyinhangpeizhi[2].length,"2013061478650023");
				insert(s,piaojuchaxunfromyinhangpeizhi[3].offset,piaojuchaxunfromyinhangpeizhi[3].length,"20131126");
				insert(s,piaojuchaxunfromyinhangpeizhi[4].offset,piaojuchaxunfromyinhangpeizhi[4].length,"103354");
				insert(s,piaojuchaxunfromyinhangpeizhi[5].offset,piaojuchaxunfromyinhangpeizhi[5].length,"029 ");
				insert(s,piaojuchaxunfromyinhangpeizhi[6].offset,piaojuchaxunfromyinhangpeizhi[6].length,"BK010001");
				insert(s,piaojuchaxunfromyinhangpeizhi[7].offset,piaojuchaxunfromyinhangpeizhi[7].length,"0000");
				insert(s,piaojuchaxunfromyinhangpeizhi[8].offset,piaojuchaxunfromyinhangpeizhi[8].length,"2002038788");
				insert(s,piaojuchaxunfromyinhangpeizhi[9].offset,piaojuchaxunfromyinhangpeizhi[9].length,"95598");
				insert(s,piaojuchaxunfromyinhangpeizhi[10].offset,piaojuchaxunfromyinhangpeizhi[10].length,"830");
				insert(s,piaojuchaxunfromyinhangpeizhi[11].offset,piaojuchaxunfromyinhangpeizhi[11].length,"");
				insert(s,piaojuchaxunfromyinhangpeizhi[12].offset,piaojuchaxunfromyinhangpeizhi[12].length,"");


				insert(s,piaojuchaxunfromyinhangpeizhi[13].offset,piaojuchaxunfromyinhangpeizhi[13].length,"11112343");//原交易流水
				insert(s,piaojuchaxunfromyinhangpeizhi[14].offset,piaojuchaxunfromyinhangpeizhi[14].length,"");
				insert(s,piaojuchaxunfromyinhangpeizhi[15].offset,piaojuchaxunfromyinhangpeizhi[15].length,"");
				insert(s,piaojuchaxunfromyinhangpeizhi[16].offset,piaojuchaxunfromyinhangpeizhi[16].length,"");
			
			
		}
				else if(strcmp(n,"11")==0)
		{
				insert(s,piaojudayinfromyinhangpeizhi[0].offset,piaojudayinfromyinhangpeizhi[0].length,"0510");
				insert(s,piaojudayinfromyinhangpeizhi[1].offset,piaojudayinfromyinhangpeizhi[1].length,"0023");
				insert(s,piaojudayinfromyinhangpeizhi[2].offset,piaojudayinfromyinhangpeizhi[2].length,"2013061478650036");
				insert(s,piaojudayinfromyinhangpeizhi[3].offset,piaojudayinfromyinhangpeizhi[3].length,"20131126");
				insert(s,piaojudayinfromyinhangpeizhi[4].offset,piaojudayinfromyinhangpeizhi[4].length,"103354");
				insert(s,piaojudayinfromyinhangpeizhi[5].offset,piaojudayinfromyinhangpeizhi[5].length,"029 ");
				insert(s,piaojudayinfromyinhangpeizhi[6].offset,piaojudayinfromyinhangpeizhi[6].length,"BK010001");
				insert(s,piaojudayinfromyinhangpeizhi[7].offset,piaojudayinfromyinhangpeizhi[7].length,"0000");
				insert(s,piaojudayinfromyinhangpeizhi[8].offset,piaojudayinfromyinhangpeizhi[8].length,"2002038788");
				insert(s,piaojudayinfromyinhangpeizhi[9].offset,piaojudayinfromyinhangpeizhi[9].length,"95598");
				insert(s,piaojudayinfromyinhangpeizhi[10].offset,piaojudayinfromyinhangpeizhi[10].length,"830");
				insert(s,piaojudayinfromyinhangpeizhi[11].offset,piaojudayinfromyinhangpeizhi[11].length,"");
				insert(s,piaojudayinfromyinhangpeizhi[12].offset,piaojudayinfromyinhangpeizhi[12].length,"");


				insert(s,piaojudayinfromyinhangpeizhi[13].offset,piaojudayinfromyinhangpeizhi[13].length,"11112343");//原交易流水
				insert(s,piaojudayinfromyinhangpeizhi[14].offset,piaojudayinfromyinhangpeizhi[14].length,"00001");
				insert(s,piaojudayinfromyinhangpeizhi[15].offset,piaojudayinfromyinhangpeizhi[15].length,"");
				insert(s,piaojudayinfromyinhangpeizhi[16].offset,piaojudayinfromyinhangpeizhi[16].length,"");
			
			
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
						makepackage(argv[2],buf);
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
