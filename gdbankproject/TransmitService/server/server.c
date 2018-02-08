#include"server.h"
#include<time.h>
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
#include<signal.h>
#include<sys/wait.h>
#include <mysql.h>
#include <stdbool.h>
#include <iconv.h>
int main()
{
	//	int x;

    int forkpid;
    /*	int dameonforkpid;
	dameonforkpid = fork();
	if(dameonforkpid < 0)
	{
	perror("fork");
	exit(1);
	}
	else if(dameonforkpid > 0)
	{
	exit(0);
	}
	else
	{
	setsid();
	chdir("/");
	umask(0);
	int wenjian;
	for(wenjian=0;wenjian<getdtablesize();wenjian++)
	{
	close(wenjian);
	}
	*/
  /*  forkpid = fork();
    if(forkpid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if(forkpid == 0)
    {
		exit(0);
        //execl("/home/server/project/server2/server","server",NULL);
    }
    else
    {*/
		iconv_t cd;
		int sendtoelecorguangdian;
		char sendguangdianbuf[BUFFER_SIZE]; // 发送给广电的数据
        char readguangdianbuf[BUFFER_SIZE]; // 接收广电发送的数据
        char sendelecbuf[BUFFER_SIZE]; // 发送给电网的数据
        char readelecbuf[BUFFER_SIZE]; // 接收电网发送的数据
        char readbankbuf[BUFFER_SIZE]; // 接收银行发送的数据
        char sendbankbuf[BUFFER_SIZE]; // 发送给银行的数据
        char sendelecbuftemp[BUFFER_SIZE]; // 发送给电网的数据
        char readelecbuftemp[BUFFER_SIZE]; // 接收电网发送的数据
        char readbankbuftemp[BUFFER_SIZE]; // 接收银行发送的数据
        char sendbankbuftemp[BUFFER_SIZE]; // 发送给银行的数据
        struct sockaddr_in client_sockaddr;
        int sin_size,read_sum;
        int sockfd,client_fd,fd;
        pid_t pid;
        int inlen,outlen;

        sin_size = sizeof(struct sockaddr_in);
        sockfd = start_server();
        signal(SIGCHLD, sigchld_handler);
        signal(SIGALRM, sigalrm_handler);
        while(1)
        {
            if((client_fd = accept(sockfd,(struct sockaddr *)&client_sockaddr,&sin_size)) == -1)
            {
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
                write_record();
                exit(1);
            }
            while ((pid = fork()) == -1);
            if (pid == 0)            //在子进程中处理
            {
                close(sockfd);
                memset(STRING_FOR_RECORD,'\0',sizeof(STRING_FOR_RECORD));
               //OutputRedirect();//输出重定向
                 time_t timep;
				time(&timep);
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s[进程%d START][函数%s:%d行]\n",ctime(&timep),getpid(),__FUNCTION__,__LINE__);    //输出当前系统时间
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]connect OK!!!---------\n",getpid());
                read_sum = read_data_from_bank(client_fd,readbankbuf);	             				//读取银行发送的数据
                sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][receive from bank][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readbankbuf),readbankbuf);
                //write_record(readbankbuf,0);
				
				//write_record();
                if((sendtoelecorguangdian=chong_zu_bank_data(read_sum,readbankbuf,sendelecbuf)) == -1)						//将银行发送的数据进行处理
				{
					close(client_fd);
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
					write_record();
					exit(0);
				}
				/*
				 *1--电力
				 *2--广电 套餐目录查询和用户查询
				 *3--广电 套餐订购
				 *4--电力直接返回银行
				 */
				if(sendtoelecorguangdian==1)
				{
					fd=send_data_to_elec(sendelecbuf);   									//将处理后的数据发送给电网
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend to elec---------\n[%d][%s]\n",getpid(),strlen(sendelecbuf),sendelecbuf);
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][send to elec][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendelecbuf),sendelecbuf);
					//write_record(sendelecbuf,1);
					//write_record();
					read_sum=read_data_from_elec(fd,readelecbuf);										//接收电网返回的数据
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dreceive from elec--------\n[%d][%s]\n",getpid(),strlen(readelecbuf),readelecbuf);
					 sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][receive from elec][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readelecbuf),readelecbuf);
                
					//write_record(readelecbuf,2);
					//write_record();
					if(chong_zu_elec_data(sendelecbuf,readelecbuf,sendbankbuf) == -1) 			//将电网返回的数据进行处理
					{
						close(client_fd);
						sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
						write_record();
						exit(0);
					}
					
					send_data(client_fd,sendbankbuf);                                       //将处理后的数据发给银行
					 sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][send to bank][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendbankbuf),sendbankbuf);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend to bank---------\n[%d][%s]\n",getpid(),strlen(sendbankbuf),sendbankbuf);
					sleep(2);
					//close(client_fd);
					write_mysql(readbankbuf);
					write_record();
				}
				else if(sendtoelecorguangdian==2)
				{
					strcpy(sendguangdianbuf,sendelecbuf);
					fd=send_data_to_guangdian(sendguangdianbuf);   									//将处理后的数据发送给广电
					 sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][send to guangdian][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendguangdianbuf),sendguangdianbuf);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]send to guangdian---------\n[%s]\n",getpid(),strlen(sendguangdianbuf),sendguangdianbuf);
					//write_record(sendguangdianbuf,4);
					//write_record();
					read_sum=read_data_from_guangdian(fd,readguangdianbuf);										//接收电网返回的数据
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][receive from guangdian][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readguangdianbuf),readguangdianbuf);
                  //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]receive from guangdian--------\n[%s]\n",getpid(),strlen(readguangdianbuf),readguangdianbuf);

					//write_record(readguangdianbuf,5);
					//write_record();
					if(chong_zu_guangdian_data(sendguangdianbuf,readguangdianbuf,sendbankbuf) == -1) 			//将电网返回的数据进行处理
					{
						close(client_fd);
						sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
						write_record();
						exit(0);
					}
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend to bank---------\n[%d][%s]\n",getpid(),strlen(sendbankbuf),sendbankbuf);
					send_data(client_fd,sendbankbuf);                                       //将处理后的数据发给银行
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][send to bank][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendbankbuf),sendbankbuf);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend to bank---------\n[%d][%s]\n",getpid(),strlen(sendbankbuf),sendbankbuf);
				
					write_record();
					sleep(2);
					//close(client_fd);
				}
				else if(sendtoelecorguangdian==3)
				{
					strcpy(sendbankbuf,sendelecbuf);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend to bank---------\n[%d][%s]\n",getpid(),strlen(sendbankbuf),sendbankbuf);
					send_data(client_fd,sendbankbuf);                                       //将处理后的数据发给银行
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][send to bank][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendbankbuf),sendbankbuf);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend to bank---------\n[%d][%s]\n",getpid(),strlen(sendbankbuf),sendbankbuf);
					
					write_record();
					//close(client_fd);
				}
				
               return 0;
            }
            else
            {
                close(client_fd);
            }
        }
        close(sockfd);

        return 0;
   // }
    return 0;
	//}
}


int start_server()
{
    struct sockaddr_in server_sockaddr;
    int sockfd;
    int i = 1;

    /* create socket link */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        exit(-1);
    }

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(LOCALBANKPORT);
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(server_sockaddr.sin_zero, 8);

   // setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

    if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr)) == -1)
    {
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        exit(-1);
    }

    if (listen(sockfd, MAX_QUE_CONN_NM) == -1)
    {
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        exit(-1);
    }

    return sockfd;
}



/*判断是哪种类型的请求包的函数*/
int chong_zu_bank_data(int sum,char buf[],char *send)
{
	//	int baolen;
	//	int i;
	//	int tmp_sum;
	int fanhuizhi;
    char type[JIAO_YI_MA_LENGTH + 1];
	//	tmp_sum=sum;
	//	while(sum > 0)
	//	{
	//		baolen = stringtoint(buf,qianfeipeizhi[0].offset,qianfeipeizhi[0].offset+qianfeipeizhi[0].length-1);
	//	    sum -= qianfeipeizhi[0].length;
    if(!gettype(buf,type))
    {
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][can't get type!]\n",__FUNCTION__,__LINE__);
    }
	//		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"------type------%s\n",type);
    memset(send,'\0',sizeof(send));
    if(!strcmp(type,"0000    "))
    {
        fanhuizhi=qian_fei_cha_xun(buf,send);
        //		write_mysql(0,send);
    }
    else if(!strcmp(type,"0001    "))
    {

        fanhuizhi=jiao_fei_qing_qiu(buf,send);
        //	write_mysql(1,send);
    }
    else if(!strcmp(type,"0002    "))
    {
        fanhuizhi=che_xiao_fu_wu(buf,send);
        //	write_mysql(2,send);
    }
    else if(!strcmp(type,"0099    "))
    {
        fanhuizhi=dui_zhang_jiao_yi(buf,send);
        //	write_mysql(3,send);
    }
    else if(!strcmp(type,"2011    "))
    {
        fanhuizhi=dai_kou_bank_qian_jie_yue_jiao_yi(buf,send);
        //	write_mysql(4,send);
    }
    else if(!strcmp(type,"1000    "))
    {
        fanhuizhi=du_ka_xin_xi_cha_xun_from_yinhang(buf,send);
        //	write_mysql(4,send);
    }
    else if(!strcmp(type,"1001    "))
    {
        fanhuizhi=ka_biao_gou_dian_from_yinhang(buf,send);
        //	write_mysql(4,send);
    }
    else if(!strcmp(type,"1002    "))
    {
        fanhuizhi=ka_biao_gou_dian_chong_zheng_from_yinhang(buf,send);
        //	write_mysql(4,send);
    }
    else if(!strcmp(type,"1003    "))
    {
        fanhuizhi=bu_xie_ka_from_yinhang(buf,send);
        //	write_mysql(4,send);
    }
	else if(!strcmp(type,"0022    "))
    {
        fanhuizhi=jiao_fei_piao_ju_cha_xun_from_yinhang(buf,send);
        //	write_mysql(4,send);
    }
	else if(!strcmp(type,"0023    "))
    {
        fanhuizhi=piao_ju_da_yin_xin_xi_cha_xun_from_yinhang(buf,send);
        //	write_mysql(4,send);
    }
    else
    {
        return -1;
    }
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"fanhuizhi--%d\n",fanhuizhi);
    return fanhuizhi;
}

int chong_zu_elec_data(char *sendelecbuf,char *readelecbuf,char *sendbankbuf)
{
    char temp_readelecbuf[BUFFER_SIZE];
    char type[JIAO_YI_LEI_XING_LENGTH+1];
    int i;
    int len;
    len=strlen(readelecbuf);
    for(i=0; i<len; i++)
    {
        temp_readelecbuf[i]=readelecbuf[i];
    }
    if(!gettypeelec(sendelecbuf,type))
    {
       sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][can't get type!]\n",__FUNCTION__,__LINE__);
    }
	   // sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"------type------%s\n",type);
    memset(sendbankbuf,'\0',sizeof(sendbankbuf));
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"进程二接收电网---------------%s\n",readelecbuf);
    if(!strcmp(type,"200001"))
    {
        qian_fei_fan_hui(temp_readelecbuf,sendbankbuf);
        //	write_mysql(5,sendbankbuf);
    }
    else if(!strcmp(type,"200002"))
    {
        jiao_fei_fan_hui(temp_readelecbuf,sendbankbuf);
        //	write_mysql(6,sendbankbuf);
    }
    else if(!strcmp(type,"200003"))
    {
        che_xiao_fan_hui(temp_readelecbuf,sendbankbuf);
        //	write_mysql(7,sendbankbuf);
    }
    else if(!strcmp(type,"200004"))
    {
        dui_zhang_fan_hui(temp_readelecbuf,sendbankbuf);
        //	write_mysql(8,sendbankbuf);
    }
    else if(!strcmp(type,"200021"))
    {
        dai_kou_bank_qian_jie_yue_fan_hui(temp_readelecbuf,sendbankbuf);
        //	write_mysql(9,sendbankbuf);
    }
    else if(!strcmp(type,"200005"))
    {
        du_ka_xin_xi_cha_xun_from_elec(temp_readelecbuf,sendbankbuf);
        //	write_mysql(9,sendbankbuf);
    }
    else if(!strcmp(type,"200006"))
    {
        ka_biao_gou_dian_from_elec(temp_readelecbuf,sendbankbuf);
        //	write_mysql(9,sendbankbuf);
    }
    else if(!strcmp(type,"200007"))
    {
        ka_biao_gou_dian_chong_zheng_from_elec(temp_readelecbuf,sendbankbuf);
        //	write_mysql(9,sendbankbuf);
    }
    else if(!strcmp(type,"200008"))
    {
        bu_xie_ka_from_elec(temp_readelecbuf,sendbankbuf);
        //	write_mysql(9,sendbankbuf);
    }
	else if(!strcmp(type,"200009"))
    {
        jiao_fei_piao_ju_cha_xun_from_elec(temp_readelecbuf,sendbankbuf);
        //	write_mysql(9,sendbankbuf);
    }
    else if(!strcmp(type,"200010"))
    {
	   piao_ju_da_yin_xin_xi_cha_xun_from_elec(temp_readelecbuf,sendbankbuf);
    }
	else
	{
		return -1;
	}
    return 1;

}

int chong_zu_guangdian_data(char *sendelecbuf,char *readelecbuf,char *sendbankbuf)
{
	char temp_readelecbuf[BUFFER_SIZE];
	char type[JIAO_YI_LEI_XING_LENGTH+1];
	int i;
	int len;
	len=strlen(readelecbuf);
	for(i=0;i<len;i++)
	{
		temp_readelecbuf[i]=readelecbuf[i];
	}
	if(!gettypeguangdian(sendelecbuf,type))
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][can't get type!]\n",__FUNCTION__,__LINE__);
	}
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"------type------%s\n",type);
	memset(sendbankbuf,'\0',sizeof(sendbankbuf));
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"进程二接收电网---------------%s\n",readelecbuf);
	if(!strcmp(type,"500004"))
	{
		guangdian_taocanmulu_fan_hui(temp_readelecbuf,sendbankbuf);
	}
	else if(!strcmp(type,"500005"))
	{
		guangdian_dinggoutaocan_fan_hui(temp_readelecbuf,sendbankbuf);
	}
	else if(!strcmp(type,"500006"))
	{
		guangdian_yonghuchaxun_fan_hui(temp_readelecbuf,sendbankbuf);
	}
	else
	{
		return -1;
	}
	return 1;

}

//广电套餐目录查询返回包解析函数
int guangdian_taocanmulu_fan_hui(char *buf,char *send)
{
	int i,j,k,t;
	int count;
	for(i=0;i<GUANGDIAN_SEND_TAOCANMULU_REAL_QUE_ZI_DUAN;i++)
	{
		if(i<=4)
		{
			memset(guangdiansendtaocanmulujiexi[i].value,'\0',sizeof(guangdiansendtaocanmulujiexi[i].value));
			strncpy(guangdiansendtaocanmulujiexi[i].value,buf+guangdiansendtaocanmulu[i].offset,guangdiansendtaocanmulu[i].length);
			guangdiansendtaocanmulujiexi[i].value[guangdiansendtaocanmulu[i].length] = '\0';
			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(guangdiansendtaocanmulujiexi[i].value),guangdiansendtaocanmulujiexi[i].value);
			if(i==3)
			{
				if(strcmp(guangdiansendtaocanmulujiexi[i].value,"000") !=0 )  //返回信息不是交易成功
				{
					i++;
					memset(guangdiansendtaocanmulujiexi[i].value,'\0',sizeof(guangdiansendtaocanmulujiexi[i].value));
					strncpy(guangdiansendtaocanmulujiexi[i].value,buf+guangdiansendtaocanmulu[i].offset,guangdiansendtaocanmulu[i].length);
					guangdiansendtaocanmulujiexi[i].value[guangdiansendtaocanmulu[i].length] = '\0';
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(guangdiansendtaocanmulujiexi[i].value),guangdiansendtaocanmulujiexi[i].value);
					break;
				}
			}
		}
		if(i==4)
		{
			count=stringtoint(guangdiansendtaocanmulujiexi[4].value,0,3);
		}
		if(i>4)
		{
			t=guangdiansendtaocanmulu[4].offset+guangdiansendtaocanmulu[4].length;
			for(k=0;k<count;k++)
			{
				memset(guangdiansendtaocanmulujiexitaocanmingxi[k].value[0],'\0',sizeof(guangdiansendtaocanmulujiexitaocanmingxi[k].value[0]));
				memset(guangdiansendtaocanmulujiexitaocanmingxi[k].value[1],'\0',sizeof(guangdiansendtaocanmulujiexitaocanmingxi[k].value[0]));
				memset(guangdiansendtaocanmulujiexitaocanmingxi[k].value[2],'\0',sizeof(guangdiansendtaocanmulujiexitaocanmingxi[k].value[0]));
				strncpy(guangdiansendtaocanmulujiexitaocanmingxi[k].value[0],buf+guangdiansendtaocanmulu[5].offset+83*k,guangdiansendtaocanmulu[5].length);
				strncpy(guangdiansendtaocanmulujiexitaocanmingxi[k].value[1],buf+guangdiansendtaocanmulu[6].offset+83*k,guangdiansendtaocanmulu[6].length);
				strncpy(guangdiansendtaocanmulujiexitaocanmingxi[k].value[2],buf+guangdiansendtaocanmulu[7].offset+83*k,guangdiansendtaocanmulu[7].length);
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%dJULU0--[%d][%s]\n",k,strlen(guangdiansendtaocanmulujiexitaocanmingxi[k].value[0]),guangdiansendtaocanmulujiexitaocanmingxi[k].value[0]);
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%dJULU1--[%d][%s]\n",k,strlen(guangdiansendtaocanmulujiexitaocanmingxi[k].value[1]),guangdiansendtaocanmulujiexitaocanmingxi[k].value[1]);
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%dJULU2--[%d][%s]\n",k,strlen(guangdiansendtaocanmulujiexitaocanmingxi[k].value[2]),guangdiansendtaocanmulujiexitaocanmingxi[k].value[2]);
			}

			if(k==count)
			{
				break;
			}
		}
	}
	chongzufabao_to_yinhang(send,5);
	return 1;
}

//广电订购套餐返回包解析函数
int guangdian_dinggoutaocan_fan_hui(char *buf,char *send)
{
	int i;

	for(i=0;i<GUANGDIAN_SEND_DINGGOUTAOCAN_REAL_QUE_ZI_DUAN ;i++)
	{
		memset(guangdiansenddinggoutaocanjiexi[i].value,'\0',sizeof(guangdiansenddinggoutaocanjiexi[i].value));
		strncpy(guangdiansenddinggoutaocanjiexi[i].value,buf+guangdiansenddinggoutaocan[i].offset,guangdiansenddinggoutaocan[i].length);
		guangdiansenddinggoutaocanjiexi[i].value[guangdiansenddinggoutaocan[i].length] = '\0';
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(guangdiansenddinggoutaocanjiexi[i].value),guangdiansenddinggoutaocanjiexi[i].value);
		if(i==3)
		{
			if(strcmp(guangdiansenddinggoutaocanjiexi[i].value,"000") !=0 )  //返回信息不是交易成功
			{
				i++;
				memset(guangdiansenddinggoutaocanjiexi[i].value,'\0',sizeof(guangdiansenddinggoutaocanjiexi[i].value));
				strncpy(guangdiansenddinggoutaocanjiexi[i].value,buf+guangdiansenddinggoutaocan[i-1].offset+guangdiansenddinggoutaocan[i-1].length,strlen(buf)-guangdiansenddinggoutaocan[i-1].offset-guangdiansenddinggoutaocan[i-1].length);
				guangdiansenddinggoutaocanjiexi[i].value[strlen(buf)-guangdiansenddinggoutaocan[i-1].offset-guangdiansenddinggoutaocan[i-1].length] = '\0';
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(guangdiansenddinggoutaocanjiexi[i].value),guangdiansenddinggoutaocanjiexi[i].value);				
			}
			break;
		}
	}
	chongzufabao_to_yinhang(send,5);
	return 1;

}
//广电用户查询返回包解析函数
int guangdian_yonghuchaxun_fan_hui(char *buf,char *send)
{
	int i;

	for(i=0;i<GUANGDIAN_SEND_YONGHUCHAXUN_REAL_QUE_ZI_DUAN ;i++)
	{
		memset(guangdiansendyonghuchaxunjiexi[i].value,'\0',sizeof(guangdiansendyonghuchaxunjiexi[i].value));
		strncpy(guangdiansendyonghuchaxunjiexi[i].value,buf+guangdiansendyonghuchaxun[i].offset,guangdiansendyonghuchaxun[i].length);
		guangdiansendyonghuchaxunjiexi[i].value[guangdiansendyonghuchaxun[i].length] = '\0';
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(guangdiansendyonghuchaxunjiexi[i].value),guangdiansendyonghuchaxunjiexi[i].value);
		if(i==3)
		{
			if(strcmp(guangdiansendyonghuchaxunjiexi[i].value,"000") !=0 )  //返回信息不是交易成功
			{
				i++;
				memset(guangdiansendyonghuchaxunjiexi[i].value,'\0',sizeof(guangdiansendyonghuchaxunjiexi[i].value));
				strncpy(guangdiansendyonghuchaxunjiexi[i].value,buf+guangdiansendyonghuchaxun[i].offset,guangdiansendyonghuchaxun[i].length);
				guangdiansendyonghuchaxunjiexi[i].value[guangdiansendyonghuchaxun[i].length] = '\0';
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(guangdiansendyonghuchaxunjiexi[i].value),guangdiansendyonghuchaxunjiexi[i].value);
				break;
			}
		}
	}
	chongzufabao_to_yinhang(send,7);
	return 1;
}



/*欠费查询请求包字段解析函数*/
int qian_fei_cha_xun(char *buf,char *send)
{
    int i,fd,len;
	char temp[BUFFER_SIZE];
	char temptaocanbianhao[3];
	int selecttaocan;
	memset(temptaocanbianhao,'\0',sizeof(temptaocanbianhao));
    for(i=0; i<QIAN_FEI_REAL_QUE_ZI_DUAN; i++)
    {
        memset(qianfeijiexi[i].value,'\0',sizeof(qianfeijiexi[i].value));
        strncpy(qianfeijiexi[i].value,buf+qianfeipeizhi[i].offset,qianfeipeizhi[i].length);
        qianfeijiexi[i].value[qianfeipeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(qianfeijiexi[i].value),qianfeijiexi[i].value);
    }
    /*-------交给重组发包的函数-----------------*/
    if(!strcmp(qianfeijiexi[9].value,"95598       "))
	{
		chongzufabao_to_dianwang(send,0);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[函数%s:%d行][该交易发往95598电力]\n",__FUNCTION__,__LINE__);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"该交易发往95598电力\n");
		return 1;
	}
	else if(!strcmp(qianfeijiexi[9].value,"96766       "))
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[函数%s:%d行][该交易发往96766广电]\n",__FUNCTION__,__LINE__);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"该交易发往96766广电\n");
		chongzufabao_to_guangdian(sendguangdianbuf0,2); //用户查询
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%drow][%d][%s]\n",__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);
		fd=send_data_to_guangdian(sendguangdianbuf0);
		//write_record(sendguangdianbuf0,1);
		//	write_mysql(0,sendelecbuf0);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]发送广电-----------------\n[%s]\n",getpid(),strlen(sendguangdianbuf0),sendguangdianbuf0);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][发送广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);
				
		read_data_from_guangdian(fd,readguangdianbuf0);
		//write_record(readguangdianbuf0,2);
		len = strlen(readguangdianbuf0);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]接受广电-----------------\n[%s]\n",getpid(),strlen(readguangdianbuf0),readguangdianbuf0);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][接受广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readguangdianbuf0),readguangdianbuf0);
	
		guangdian_yonghuchaxun_fan_hui(readguangdianbuf0,temp);
		//	write_mysql(5,temp);
		/*------------------------------------------------------------------------*/
		
		
		
		temptaocanbianhao[0]=qianfeijiexi[14].value[2];
        temptaocanbianhao[1]=qianfeijiexi[14].value[3];  //提取银行套餐选项编号字段中的后两位  00-查询全部  01-10查询某一个		
		 if(strcmp(temptaocanbianhao,"00")!=0)   //----------------查询某一个，进行预订购
        {
			memset(sendguangdianbuf0,'\0',sizeof(sendguangdianbuf0));
			chongzufabao_to_guangdian(sendguangdianbuf0,0); //套餐目录查询
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%drow][%d][%s]\n",__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);
			fd=send_data_to_guangdian(sendguangdianbuf0);
			//write_record(sendguangdianbuf0,1);
			//	write_mysql(0,sendelecbuf0);
			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][发送广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);
		
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]发送广电-----------------\n[%s]\n",getpid(),strlen(sendguangdianbuf0),sendguangdianbuf0);
			memset(readguangdianbuf0,'\0',sizeof(readguangdianbuf0));
			read_data_from_guangdian(fd,readguangdianbuf0);
			len = strlen(readguangdianbuf0);
			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][接受广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readguangdianbuf0),readguangdianbuf0);
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]接受广电-----------------\n[%s]\n",getpid(),strlen(readguangdianbuf0),readguangdianbuf0);
			memset(temp,'\0',sizeof(temp));
			guangdian_taocanmulu_fan_hui(readguangdianbuf0,temp);
			chongzufabao_to_guangdian(send,1);  //订购套餐
		}
		else
		{
			chongzufabao_to_guangdian(send,0);  //套餐目录查询
		}
		return 2;
	}
	else
	{
		return 3;
	}

}


/*交费请求包字段解析函数*/
int jiao_fei_qing_qiu(char *buf,char *send)
{
    char temp_readelecbuf0[BUFFER_SIZE];
    int i,fd,len;
    char temp[BUFFER_SIZE];
    for(i=0; i<JIAO_FEI_REAL_QUE_ZI_DUAN; i++)
    {
        memset(jiaofeijiexi[i].value,'\0',sizeof(jiaofeijiexi[i].value));
        strncpy(jiaofeijiexi[i].value,buf+jiaofeipeizhi[i].offset,jiaofeipeizhi[i].length);
        jiaofeijiexi[i].value[jiaofeipeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(jiaofeijiexi[i].value),jiaofeijiexi[i].value);
        if(i <= 14)
        {
            memset(qianfeijiexi[i].value,'\0',sizeof(qianfeijiexi[i].value));
            if(i==0)
            {
                strcpy(qianfeijiexi[i].value,"0222");
            }
            else if(i==1)
            {
                strcpy(qianfeijiexi[i].value,"0000    ");
            }
            else
            {
                strncpy(qianfeijiexi[i].value,buf+qianfeipeizhi[i].offset,qianfeipeizhi[i].length);
            }
            qianfeijiexi[i].value[qianfeipeizhi[i].length] = '\0';
        }
    }

	if(!strcmp(jiaofeijiexi[9].value,"95598       "))
	{
		/*-------交给重组发包的进程-----------------*/
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[函数%s:%d行][该交易发往95598电力]\n",__FUNCTION__,__LINE__);
		chongzufabao_to_dianwang(sendelecbuf0,0);
		fd=send_data_to_elec(sendelecbuf0);
		//write_record(sendelecbuf0,1);
		//	write_mysql(0,sendelecbuf0);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][发送电网][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendelecbuf0),sendelecbuf0);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d发送电网-----------------\n[%s]\n",getpid(),sendelecbuf0);
		read_data_from_elec(fd,readelecbuf0);
		//write_record(readelecbuf0,2);
		len = strlen(readelecbuf0);

		for(i=0; i<len; i++)
		{
			temp_readelecbuf0[i]=readelecbuf0[i];
		}
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][发送电网][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readelecbuf0),readelecbuf0);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d接受电网-----------------\n[%s]\n",getpid(),readelecbuf0);
		qian_fei_fan_hui(temp_readelecbuf0,temp);
		//	write_mysql(5,temp);
		/*------------------------------------------------------------------------*/
		chongzufabao_to_dianwang(send,1);
		return 1;
	}
	else if(!strcmp(qianfeijiexi[9].value,"96766       "))
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[函数%s:%d行][该交易发往96766广电]\n",__FUNCTION__,__LINE__);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"该交易发往96766广电\n");
		chongzufabao_to_guangdian(sendguangdianbuf0,2); //用户查询		
		fd=send_data_to_guangdian(sendguangdianbuf0);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][发送广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%drow][%d][%s]\n",__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);		
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][%d]发送广电-----------------\n[%s]\n",getpid(),strlen(sendguangdianbuf0),sendguangdianbuf0);
		//printf("[%drow][%d][%s]\n",__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);
		read_data_from_guangdian(fd,readguangdianbuf0);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][接收广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readguangdianbuf0),readguangdianbuf0);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][%d行接收广电][%d][%s]\n",getpid(),__LINE__,strlen(readguangdianbuf0),readguangdianbuf0);		
		len = strlen(readguangdianbuf0);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]接受广电-----------------\n[%s]\n",getpid(),strlen(readguangdianbuf0),readguangdianbuf0);
		guangdian_yonghuchaxun_fan_hui(readguangdianbuf0,temp);
		
		//write_record();
		
		
		memset(sendguangdianbuf0,'\0',BUFFER_SIZE);
		memset(readguangdianbuf0,'\0',BUFFER_SIZE);
		chongzufabao_to_guangdian(sendguangdianbuf0,0);  //套餐目录查询
		
		fd=send_data_to_guangdian(sendguangdianbuf0);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][发送广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d行发送广电][%d][%s]\n",__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);
		//printf("[%drow][%d][%s]\n",__LINE__,strlen(sendguangdianbuf0),sendguangdianbuf0);		
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d[%d]发送广电-----------------\n[%s]\n",getpid(),strlen(sendguangdianbuf0),sendguangdianbuf0);
		read_data_from_guangdian(fd,readguangdianbuf0);
		//printf("[%d行接收广电][%d][%s]\n",__LINE__,strlen(readguangdianbuf0),readguangdianbuf0);
		//write_record(readguangdianbuf0,2);
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][接收广电][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(readguangdianbuf0),readguangdianbuf0);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d行接收广电][%d][%s]\n",__LINE__,strlen(readguangdianbuf0),readguangdianbuf0);
		len = strlen(readguangdianbuf0);		
		guangdian_taocanmulu_fan_hui(readguangdianbuf0,temp);
		//write_record();
		
		fd=send_data_to_java_gdpayservice();
		
		read_data_from_java_gdpayservice(fd);
		
		chongzufabao_to_yinhang(send,6);  //订购套餐
		return 3;
	}
	else
	{
		return 4;
	}
}

/*撤销服务请求包字段解析函数*/
int che_xiao_fu_wu(char *buf,char *send)
{
    int i;

    for(i=0; i<CHE_XIAO_REAL_QUE_ZI_DUAN; i++)
    {
        strncpy(chexiaojiexi[i].value,buf+chexiaopeizhi[i].offset,chexiaopeizhi[i].length);
        chexiaojiexi[i].value[chexiaopeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(chexiaojiexi[i].value),chexiaojiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    if(!strcmp(chexiaojiexi[9].value,"95598       "))
	{
		chongzufabao_to_dianwang(send,2);
		return 1;
	}
	else if(!strcmp(chexiaojiexi[9].value,"96766       "))
	{

		chongzufabao_to_guangdian(send,2);
		return 2;
	}
	else
	{
		return 3;
	}
}
/*对账交易请求包字段解析函数*/
int dui_zhang_jiao_yi(char *buf,char *send)
{
    int i;

    for(i=0; i<DUI_ZHANG_REAL_QUE_ZI_DUAN; i++)
    {
        strncpy(duizhangjiexi[i].value,buf+duizhangpeizhi[i].offset,duizhangpeizhi[i].length);
        duizhangjiexi[i].value[duizhangpeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(duizhangjiexi[i].value),duizhangjiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_dianwang(send,3);
    return 1;
}
/*银行方签约解约交易请求包字段解析函数*/
int dai_kou_bank_qian_jie_yue_jiao_yi(char *buf,char *send)
{
    int i;

    for(i=0; i<DAI_KOU_BANK_QIAN_JIE_YUE_REAL_QUE_ZI_DUAN; i++)
    {
        strncpy(daikoubankqianjieyuejiexi[i].value,buf+daikoubankqianjieyuepeizhi[i].offset,daikoubankqianjieyuepeizhi[i].length);
        daikoubankqianjieyuejiexi[i].value[daikoubankqianjieyuepeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,daikoubankqianjieyuejiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_dianwang(send,4);
    return 1;
}
/*读卡信息银行出参字段解析函数*/ //*****************************2013-12-25添加*****************
int du_ka_xin_xi_cha_xun_from_yinhang(char *buf,char *send)
{
    int i;

    for(i=0; i<DU_KA_CHA_XUN_FROM_YINHANG_REAL_QUE_ZI_DUAN; i++)
    {
        memset(dukachaxunfromyinhangjiexi[i].value,'\0',sizeof(dukachaxunfromyinhangjiexi[i].value));
        strncpy(dukachaxunfromyinhangjiexi[i].value,buf+dukachaxunfromyinhangpeizhi[i].offset,dukachaxunfromyinhangpeizhi[i].length);
        dukachaxunfromyinhangjiexi[i].value[dukachaxunfromyinhangpeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(dukachaxunfromyinhangjiexi[i].value),dukachaxunfromyinhangjiexi[i].value);
    }
    /*-------交给重组发包的函数-----------------*/
    chongzufabao_to_dianwang(send,5);
    return 1;
}

/*卡表购电银行出参字段解析函数*/ //*****************************2013-12-25添加*****************
int ka_biao_gou_dian_from_yinhang(char *buf,char *send)
{
    int i;

    for(i=0; i<KA_BIAO_GOU_DIAN_FROM_YINHANG_REAL_QUE_ZI_DUAN; i++)
    {
        memset(kabiaogoudianfromyinhangjiexi[i].value,'\0',sizeof(kabiaogoudianfromyinhangjiexi[i].value));
        strncpy(kabiaogoudianfromyinhangjiexi[i].value,buf+kabiaogoudianfromyinhangpeizhi[i].offset,kabiaogoudianfromyinhangpeizhi[i].length);
        kabiaogoudianfromyinhangjiexi[i].value[kabiaogoudianfromyinhangpeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(kabiaogoudianfromyinhangjiexi[i].value),kabiaogoudianfromyinhangjiexi[i].value);
    }
    /*-------交给重组发包的函数-----------------*/
    chongzufabao_to_dianwang(send,6);
    return 1;
}
/*卡表购电冲正银行出参字段解析函数*/ //*****************************2013-12-25添加*****************
int ka_biao_gou_dian_chong_zheng_from_yinhang(char *buf,char *send)
{
    int i;

    for(i=0; i<KA_BIAO_GOU_DIAN_CHONG_ZHENG_FROM_YINHANG_REAL_QUE_ZI_DUAN; i++)
    {
        memset(kabiaogoudianchongzhengfromyinhangjiexi[i].value,'\0',sizeof(kabiaogoudianchongzhengfromyinhangjiexi[i].value));
        strncpy(kabiaogoudianchongzhengfromyinhangjiexi[i].value,buf+kabiaogoudianchongzhengfromyinhangpeizhi[i].offset,kabiaogoudianchongzhengfromyinhangpeizhi[i].length);
        kabiaogoudianchongzhengfromyinhangjiexi[i].value[kabiaogoudianchongzhengfromyinhangpeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(kabiaogoudianchongzhengfromyinhangjiexi[i].value),kabiaogoudianchongzhengfromyinhangjiexi[i].value);
    }
    /*-------交给重组发包的函数-----------------*/
    chongzufabao_to_dianwang(send,7);
    return 1;
}
/*补写卡银行出参字段解析函数*/ //*****************************2013-12-25添加*****************
int bu_xie_ka_from_yinhang(char *buf,char *send)
{
    int i;

    for(i=0; i<BU_XIE_KA_FROM_YINHANG_REAL_QUE_ZI_DUAN; i++)
    {
        memset(buxiekafromyinhangjiexi[i].value,'\0',sizeof(buxiekafromyinhangjiexi[i].value));
        strncpy(buxiekafromyinhangjiexi[i].value,buf+buxiekafromyinhangpeizhi[i].offset,buxiekafromyinhangpeizhi[i].length);
        buxiekafromyinhangjiexi[i].value[buxiekafromyinhangpeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(buxiekafromyinhangjiexi[i].value),buxiekafromyinhangjiexi[i].value);
    }
    /*-------交给重组发包的函数-----------------*/
    chongzufabao_to_dianwang(send,8);
    return 1;
}

/*缴费票据查询银行出参字段解析函数*/ //*****************************2013-12-25添加*****************
int jiao_fei_piao_ju_cha_xun_from_yinhang(char *buf,char *send)
{
    int i;

    for(i=0; i<JIAO_FEI_PIAO_JU_CHA_XUN_FROM_YINHANG_REAL_QUE_ZI_DUAN; i++)
    {
        memset(jiaofeipiaojuchaxunfromyinhangjiexi[i].value,'\0',sizeof(jiaofeipiaojuchaxunfromyinhangjiexi[i].value));
        strncpy(jiaofeipiaojuchaxunfromyinhangjiexi[i].value,buf+jiaofeipiaojuchaxunfromyinhangpeizhi[i].offset,jiaofeipiaojuchaxunfromyinhangpeizhi[i].length);
        jiaofeipiaojuchaxunfromyinhangjiexi[i].value[jiaofeipiaojuchaxunfromyinhangpeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(jiaofeipiaojuchaxunfromyinhangjiexi[i].value),jiaofeipiaojuchaxunfromyinhangjiexi[i].value);
    }
    /*-------交给重组发包的函数-----------------*/
    chongzufabao_to_dianwang(send,9);
    return 1;
}
/*票据打印信息查询银行出参字段解析函数*/ //*****************************2013-12-25添加********20140710该交易暂时取消不用*********
int piao_ju_da_yin_xin_xi_cha_xun_from_yinhang(char *buf,char *send)
{
    int i;

    for(i=0; i<PIAO_JU_DA_YIN_XIN_XI_CHA_XUN_FROM_YINHANG_REAL_QUE_ZI_DUAN; i++)
    {
        memset(piaojudayinxinxichaxunfromyinhangjiexi[i].value,'\0',sizeof(piaojudayinxinxichaxunfromyinhangjiexi[i].value));
        strncpy(piaojudayinxinxichaxunfromyinhangjiexi[i].value,buf+piaojudayinxinxichaxunfromyinhangpeizhi[i].offset,piaojudayinxinxichaxunfromyinhangpeizhi[i].length);
        piaojudayinxinxichaxunfromyinhangjiexi[i].value[piaojudayinxinxichaxunfromyinhangpeizhi[i].length] = '\0';
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(piaojudayinxinxichaxunfromyinhangjiexi[i].value),piaojudayinxinxichaxunfromyinhangjiexi[i].value);
    }
    /*-------交给重组发包的函数-----------------*/
    chongzufabao_to_dianwang(send,10);
    return 1;
}





/*欠费查询请求返回包字段解析函数*/
int qian_fei_fan_hui(char *buf,char *send)
{
    char *next;
    char *token;
	//	int biaozhi[QIAN_FEI_FAN_HUI_REAL_QUE_ZI_DUAN]={0};
	//	int baolen;
    int i,j,count,k,jilushu;
	//	baolen = strlen(buf);
    /*	for(i=0;i<baolen;i++)
	{
	if(buf[i]=='|')
	{
	count++;
	if(buf[i+1]=='|')
	{
	if(count!=QIAN_FEI_FAN_HUI_REAL_QUE_ZI_DUAN-1)
	{
	memset(qianfeifanhuijiexi[count].value,'\0',qianfeifanhuipeizhi[count].length);
	biaozhi[count]=1;
	}
	else
	{
	biaozhi[count]=1;
	}
	}
	}
	}
    */
    i=0;
    j=0;
    count=0;
    k=0;
    for(i=0; i<3; i++)
    {
        memset(qianfeifanhuijiexi[i].value,'\0',sizeof(qianfeifanhuijiexi[i].value));
        strncpy(qianfeifanhuijiexi[i].value,buf+qianfeifanhuipeizhi[i].offset,qianfeifanhuipeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,qianfeifanhuijiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(i=3;;)
    {
        if(i < 13)
        {
			//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"token------%s\n",token);
            if(i==3)
            {
                for(k=0; k<strlen(token); k++)
                {
                    token[k]=token[k+HEAD_LENGTH];
                }
                token[k]='\0';
                memset(qianfeifanhuijiexi[i].value,'\0',sizeof(qianfeifanhuijiexi[i].value));
                strncpy(qianfeifanhuijiexi[i].value,token,strlen(token));
                sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,qianfeifanhuijiexi[i].value);
                i++;
                token=strsep(&buf,"|");
            }
            else
            {
                memset(qianfeifanhuijiexi[i].value,'\0',sizeof(qianfeifanhuijiexi[i].value));
                strncpy(qianfeifanhuijiexi[i].value,token,strlen(token));
                sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,qianfeifanhuijiexi[i].value);
                i++;
                token=strsep(&buf,"|");
                if(i==13)
                {
                    memset(qianfeifanhuijiexi[i].value,'\0',sizeof(qianfeifanhuijiexi[i].value));
                    strncpy(qianfeifanhuijiexi[i].value,token,strlen(token));
                    sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,qianfeifanhuijiexi[i].value);
                }
				//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"--buf%s\n",buf);
            }
        }
        else
        {
            jilushu=stringtoint(qianfeifanhuijiexi[13].value,0,strlen(qianfeifanhuijiexi[13].value)-1);
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jilushu--%d\n",jilushu);
            if(jilushu>0)
            {
                if(count == jilushu)
                {
                    break;
                }
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"buf%s\n",buf);
                next=strsep(&buf,";");
				//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"next%s\n",next);
				//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"----------\n");
                for(j=0; j<5; j++)
                {

                    token=strsep(&next,"&");
					//					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"token%s\n",token);
                    memset(qianfeifanhuijiexijilu[count].value[j],'\0',sizeof(qianfeifanhuijiexijilu[j].value));
                    strncpy(qianfeifanhuijiexijilu[count].value[j],token,strlen(token));
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"JILU%d.%d--[%s]\n",count,j,qianfeifanhuijiexijilu[count].value[j]);
                }
                count++;
            }
            else
            {
                break;
            }
        }

    }

    /*-------交给重组发包的函数-----------------*/
	//	chongzufabao(send,1);
    chongzufabao_to_yinhang(send,0);
    //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"!!!!!!!!!!!!!!!!!!!!!!!%s\n",send);
    return 1;

}

/*交费请求返回包字段解析函数*/
int jiao_fei_fan_hui(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(jiaofeifanhuijiexi[i].value,'\0',sizeof(jiaofeifanhuijiexi[i].value));
        strncpy(jiaofeifanhuijiexi[i].value,buf+jiaofeifanhuipeizhi[i].offset,jiaofeifanhuipeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,jiaofeifanhuijiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    token[k]='\0';
    for(i=3; i<JIAO_FEI_FAN_HUI_REAL_QUE_ZI_DUAN; i++)
    {
		//		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"---%s\n",token);
        memset(jiaofeifanhuijiexi[i].value,'\0',sizeof(jiaofeifanhuijiexi[i].value));
        strncpy(jiaofeifanhuijiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,jiaofeifanhuijiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,1);
    return 1;
}

/*撤销服务返回包字段解析函数*/
int che_xiao_fan_hui(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(chexiaofanhuijiexi[i].value,'\0',sizeof(chexiaofanhuijiexi[i].value));
        strncpy(chexiaofanhuijiexi[i].value,buf+chexiaofanhuipeizhi[i].offset,chexiaofanhuipeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,chexiaofanhuijiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<CHE_XIAO_FAN_HUI_REAL_QUE_ZI_DUAN; i++)
    {
        memset(chexiaofanhuijiexi[i].value,'\0',sizeof(chexiaofanhuijiexi[i].value));
        strncpy(chexiaofanhuijiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,chexiaofanhuijiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,2);
    return 1;
}

/*对账交易返回包字段解析函数*/
int dui_zhang_fan_hui(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(duizhangfanhuijiexi[i].value,'\0',sizeof(duizhangfanhuijiexi[i].value));
        strncpy(duizhangfanhuijiexi[i].value,buf+duizhangfanhuipeizhi[i].offset,duizhangfanhuipeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,duizhangfanhuijiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<DUI_ZHANG_FAN_HUI_REAL_QUE_ZI_DUAN; i++)
    {
        memset(duizhangfanhuijiexi[i].value,'\0',sizeof(duizhangfanhuijiexi[i].value));
        strncpy(duizhangfanhuijiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,duizhangfanhuijiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,3);
    return 1;
}

/*银行方签约解约交易返回包字段解析函数*/
int dai_kou_bank_qian_jie_yue_fan_hui(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(daikoubankqianjieyuefanhuijiexi[i].value,'\0',sizeof(duizhangfanhuijiexi[i].value));
        strncpy(daikoubankqianjieyuefanhuijiexi[i].value,buf+daikoubankqianjieyuefanhuipeizhi[i].offset,daikoubankqianjieyuefanhuipeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,daikoubankqianjieyuefanhuijiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<DAI_KOU_BANK_QIAN_JIE_YUE_FAN_HUI_REAL_QUE_ZI_DUAN; i++)
    {
        memset(daikoubankqianjieyuefanhuijiexi[i].value,'\0',sizeof(daikoubankqianjieyuefanhuijiexi[i].value));
        strncpy(daikoubankqianjieyuefanhuijiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,daikoubankqianjieyuefanhuijiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,4);
    return 1;
}
/*读卡信息电力出参字段解析函数*///*****************************2013-12-25添加*****************
int du_ka_xin_xi_cha_xun_from_elec(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(dukachaxunfromelecjiexi[i].value,'\0',sizeof(dukachaxunfromelecjiexi[i].value));
        strncpy(dukachaxunfromelecjiexi[i].value,buf+dukachaxunfromelecpeizhi[i].offset,dukachaxunfromelecpeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(dukachaxunfromelecjiexi[i].value),dukachaxunfromelecjiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<DU_KA_CHA_XUN_FROM_ELEC_REAL_QUE_ZI_DUAN; i++)
    {
        memset(dukachaxunfromelecjiexi[i].value,'\0',sizeof(dukachaxunfromelecjiexi[i].value));
        strncpy(dukachaxunfromelecjiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(dukachaxunfromelecjiexi[i].value),dukachaxunfromelecjiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,8);
    return 1;
}

/*卡表购电电力出参字段解析函数*///*****************************2013-12-25添加*****************
int ka_biao_gou_dian_from_elec(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(kabiaogoudianfromelecjiexi[i].value,'\0',sizeof(kabiaogoudianfromelecjiexi[i].value));
        strncpy(kabiaogoudianfromelecjiexi[i].value,buf+kabiaogoudianfromelecpeizhi[i].offset,kabiaogoudianfromelecpeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(kabiaogoudianfromelecjiexi[i].value),kabiaogoudianfromelecjiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<KA_BIAO_GOU_DIAN_FROM_ELEC_REAL_QUE_ZI_DUAN; i++)
    {
        memset(kabiaogoudianfromelecjiexi[i].value,'\0',sizeof(kabiaogoudianfromelecjiexi[i].value));
        strncpy(kabiaogoudianfromelecjiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(kabiaogoudianfromelecjiexi[i].value),kabiaogoudianfromelecjiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,9);
    return 1;
}

/*卡表购电冲正电力出参字段解析函数*///*****************************2013-12-25添加*****************
int ka_biao_gou_dian_chong_zheng_from_elec(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(kabiaogoudianchongzhengfromelecjiexi[i].value,'\0',sizeof(kabiaogoudianchongzhengfromelecjiexi[i].value));
        strncpy(kabiaogoudianchongzhengfromelecjiexi[i].value,buf+kabiaogoudianchongzhengfromelecpeizhi[i].offset,kabiaogoudianchongzhengfromelecpeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(kabiaogoudianchongzhengfromelecjiexi[i].value),kabiaogoudianchongzhengfromelecjiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<KA_BIAO_GOU_DIAN_CHONG_ZHENG_FROM_ELEC_REAL_QUE_ZI_DUAN; i++)
    {
        memset(kabiaogoudianchongzhengfromelecjiexi[i].value,'\0',sizeof(kabiaogoudianchongzhengfromelecjiexi[i].value));
        strncpy(kabiaogoudianchongzhengfromelecjiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(kabiaogoudianchongzhengfromelecjiexi[i].value),kabiaogoudianchongzhengfromelecjiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,10);
    return 1;
}

/*补写卡电力出参字段解析函数*///*****************************2013-12-25添加*****************
int bu_xie_ka_from_elec(char *buf,char *send)
{
    int i,k;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(buxiekafromelecjiexi[i].value,'\0',sizeof(buxiekafromelecjiexi[i].value));
        strncpy(buxiekafromelecjiexi[i].value,buf+buxiekafromelecpeizhi[i].offset,buxiekafromelecpeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(buxiekafromelecjiexi[i].value),buxiekafromelecjiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<BU_XIE_KA_FROM_ELEC_REAL_QUE_ZI_DUAN; i++)
    {
        memset(buxiekafromelecjiexi[i].value,'\0',sizeof(buxiekafromelecjiexi[i].value));
        strncpy(buxiekafromelecjiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(buxiekafromelecjiexi[i].value),buxiekafromelecjiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
    chongzufabao_to_yinhang(send,11);
    return 1;
}
/*缴费票据查询电力出参字段解析函数*/
int jiao_fei_piao_ju_cha_xun_from_elec(char *buf,char *send)
{
    char *next;
    char *token;
    int i,j,count,k,jilushu;
    i=0;
    j=0;
    count=0;
    k=0;
    for(i=0; i<3; i++)
    {
        memset(jiaofeipiaojuchaxunfromelecjiexi[i].value,'\0',sizeof(jiaofeipiaojuchaxunfromelecjiexi[i].value));
        strncpy(jiaofeipiaojuchaxunfromelecjiexi[i].value,buf+jiaofeipiaojuchaxunfromelecpeizhi[i].offset,jiaofeipiaojuchaxunfromelecpeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,jiaofeipiaojuchaxunfromelecjiexi[i].value);
    }
    token=strsep(&buf,"|");


    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<JIAO_FEI_PIAO_JU_CHA_XUN_FROM_ELEC_REAL_QUE_ZI_DUAN; i++)
    {
		if(i==JIAO_FEI_PIAO_JU_CHA_XUN_FROM_ELEC_REAL_QUE_ZI_DUAN-1)
		{
			 jilushu=stringtoint(jiaofeipiaojuchaxunfromelecjiexi[5].value,0,strlen(jiaofeipiaojuchaxunfromelecjiexi[5].value)-1);
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jilushu--%d\n",jilushu);
			for(j=0;j<jilushu;j++)
			{
				next=strsep(&token,";");
				memset(jiaofeipiaojuchaxunfromelecjiexijilu[j].value,'\0',sizeof(jiaofeipiaojuchaxunfromelecjiexijilu[j].value));
                strncpy(jiaofeipiaojuchaxunfromelecjiexijilu[j].value,next,strlen(next));
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"JILU%d--[%s]\n",j,jiaofeipiaojuchaxunfromelecjiexijilu[j].value);
			}
		}
		else
		{
			memset(jiaofeipiaojuchaxunfromelecjiexi[i].value,'\0',sizeof(jiaofeipiaojuchaxunfromelecjiexi[i].value));
			strncpy(jiaofeipiaojuchaxunfromelecjiexi[i].value,token,strlen(token));
			token=strsep(&buf,"|");
			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(jiaofeipiaojuchaxunfromelecjiexi[i].value),jiaofeipiaojuchaxunfromelecjiexi[i].value);
		}
    }

   /* for(i=3;;)
    {
        if(i < 5)
        {
			//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"token------%s\n",token);
            if(i==3)
            {
                for(k=0; k<strlen(token); k++)
                {
                    token[k]=token[k+HEAD_LENGTH];
                }
                token[k]='\0';
                memset(jiaofeipiaojuchaxunfromelecjiexi[i].value,'\0',sizeof(jiaofeipiaojuchaxunfromelecjiexi[i].value));
                strncpy(jiaofeipiaojuchaxunfromelecjiexi[i].value,token,strlen(token));
                sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,jiaofeipiaojuchaxunfromelecjiexi[i].value);
                i++;
                token=strsep(&buf,"|");
            }
            else
            {
                memset(jiaofeipiaojuchaxunfromelecjiexi[i].value,'\0',sizeof(jiaofeipiaojuchaxunfromelecjiexi[i].value));
                strncpy(jiaofeipiaojuchaxunfromelecjiexi[i].value,token,strlen(token));
                sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,jiaofeipiaojuchaxunfromelecjiexi[i].value);
                i++;
                token=strsep(&buf,"|");
                if(i==5)
                {
                    memset(jiaofeipiaojuchaxunfromelecjiexi[i].value,'\0',sizeof(jiaofeipiaojuchaxunfromelecjiexi[i].value));
                    strncpy(jiaofeipiaojuchaxunfromelecjiexi[i].value,token,strlen(token));
                    sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%s]\n",i,jiaofeipiaojuchaxunfromelecjiexi[i].value);
                }
				//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"--buf%s\n",buf);
            }
        }
        else
        {
            jilushu=stringtoint(jiaofeipiaojuchaxunfromelecjiexi[5].value,0,strlen(jiaofeipiaojuchaxunfromelecjiexi[5].value)-1);
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jilushu--%d\n",jilushu);
            if(jilushu>0)
            {
                if(count == jilushu)
                {
                    break;
                }
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"buf%s\n",buf);
                next=strsep(&buf,";");
				//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"next%s\n",next);
				//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"----------\n");
                for(j=0; j<3; j++)
                {

                    token=strsep(&next,"&");
					//					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"token%s\n",token);
                    memset(jiaofeipiaojuchaxunfromelecjiexijilu[count].value[j],'\0',sizeof(jiaofeipiaojuchaxunfromelecjiexijilu[j].value));
                    strncpy(jiaofeipiaojuchaxunfromelecjiexijilu[count].value[j],token,strlen(token));
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"JILU%d.%d--[%s]\n",count,j,jiaofeipiaojuchaxunfromelecjiexijilu[count].value[j]);
                }
                count++;
            }
            else
            {
                break;
            }
        }

    }
*/
    /*-------交给重组发包的函数-----------------*/
	//	chongzufabao(send,1);
    chongzufabao_to_yinhang(send,12);
    //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"!!!!!!!!!!!!!!!!!!!!!!!%s\n",send);
    return 1;

}
/*票据打印信息查询电力出参字段解析函数*///*****************************2013-12-25添加*****************
int piao_ju_da_yin_xin_xi_cha_xun_from_elec(char *buf,char *send)
{
    int i,k,file_fd,j;
    char *token;
    for(i=0; i<3; i++)
    {
        memset(piaojudayinxinxichaxunfromelecjiexi[i].value,'\0',sizeof(piaojudayinxinxichaxunfromelecjiexi[i].value));
        strncpy(piaojudayinxinxichaxunfromelecjiexi[i].value,buf+ piaojudayinxinxichaxunfromelecpeizhi[i].offset, piaojudayinxinxichaxunfromelecpeizhi[i].length);
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(piaojudayinxinxichaxunfromelecjiexi[i].value),piaojudayinxinxichaxunfromelecjiexi[i].value);
    }
    token=strsep(&buf,"|");
    for(k=0; k<strlen(token); k++)
    {
        token[k]=token[k+HEAD_LENGTH];
    }
    for(i=3; i<PIAO_JU_DA_YIN_XIN_XI_CHA_XUN_FROM_ELEC_REAL_QUE_ZI_DUAN; i++)
    {
        memset(piaojudayinxinxichaxunfromelecjiexi[i].value,'\0',sizeof(piaojudayinxinxichaxunfromelecjiexi[i].value));
        strncpy(piaojudayinxinxichaxunfromelecjiexi[i].value,token,strlen(token));
        token=strsep(&buf,"|");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"ID%d--[%d][%s]\n",i,strlen(piaojudayinxinxichaxunfromelecjiexi[i].value),piaojudayinxinxichaxunfromelecjiexi[i].value);
    }
    /*-------交给重组发包的进程-----------------*/
	/*char XML_PATH_temp[256];
	memset(XML_PATH,'\0',256);
	strcpy(XML_PATH,XML_PATH_EXCEPT_FILENAME);
	k=strlen(XML_PATH);
	memset(XML_PATH_temp,'\0',256);
	strcpy(XML_PATH_temp,piaojudayinxinxichaxunfromyinhangjiexi[13].value);
	for(i=strlen(piaojudayinxinxichaxunfromyinhangjiexi[13].value)-1;i>=0;i--)
	{
		if(XML_PATH_temp[i]!=' ')
		{
			break;
		}
	}
	for(j=0;j<=i;j++)
	{
		XML_PATH[k++]=XML_PATH_temp[j];
	}
	XML_PATH[k++]='_';
	memset(XML_PATH_temp,'\0',256);
	strcpy(XML_PATH_temp,piaojudayinxinxichaxunfromyinhangjiexi[14].value);
	for(i=strlen(piaojudayinxinxichaxunfromyinhangjiexi[14].value)-1;i>=0;i--)
	{
		if(XML_PATH_temp[i]!=' ')
		{
			break;
		}
	}
	for(j=0;j<=i;j++)
	{
		XML_PATH[k++]=XML_PATH_temp[j];
	}
	strcat(XML_PATH,".xml");
    chongzufabao_to_yinhang(send,13);
	file_fd = open(XML_PATH,O_CREAT | O_APPEND | O_RDWR | O_TRUNC,0777);
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"fd--%d\n",file_fd);
    lock_set(file_fd,F_WRLCK);
	write(file_fd,piaojudayinxinxichaxunfromelecjiexi[3].value,strlen(piaojudayinxinxichaxunfromelecjiexi[3].value));
    lock_set(file_fd,F_UNLCK);
    close(file_fd);
    ftp_send_file();
	*/
	chongzufabao_to_yinhang(send,13);
    return 1;
}





int send_data(int fd,char *buf)
{
	//char bufl[]="103118BAE4DCDFE5283300107200001|0000000000000010|20131211111249|121100000155|0000000000000010|YH0Z654010203|01|2002038791||20090909|";
    int buflen;
    int baochanglength = strlen(buf);
    int sendbaochanglength=0;
    int tmp=baochanglength;
    int nleft;
	struct timeval mytime;
	char *ptr;
	ptr=buf;
	mytime.tv_sec = TIME_OUT;
	mytime.tv_usec = 0;
	nleft=strlen(buf);
    while(sendbaochanglength!=tmp)
    {
        if((buflen = send(fd,ptr,nleft,0))>0)
        {
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d\n",buflen);
			nleft-=buflen;
            sendbaochanglength+=buflen;
            baochanglength-=buflen;
            ptr+=buflen;
        }
    }
	//    sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d\n",strlen(buf));
    return 1;
}
/*读取数据包的函数*/
int read_data_from_bank(int fd,char *buf)
{
    int i,sum=0;
    int buflen;
    char temp[BUFFER_SIZE];
    int baochanglength = 4;
    int readbaochanglength=0;
    int tmp=baochanglength;
	struct timeval mytime;
	mytime.tv_sec = TIME_OUT;
	mytime.tv_usec = 0;
    memset(temp,'\0',sizeof(temp));
    memset(buf,'\0',sizeof(buf));
	//setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &mytime, sizeof(mytime));
alarm(TIME_OUT);
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"setsockopt ok!!!\n");
    while(readbaochanglength!=tmp)
    {
        if((buflen = recv(fd,temp,baochanglength,0))>0)
        {
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baotou----%s\n",temp);
            readbaochanglength+=buflen;
            baochanglength-=buflen;
            for(i=0; i<buflen; i++)
            {
                buf[sum++] = temp[i];
            }
            memset(temp,'\0',sizeof(temp));
        }
		else //if (buflen <= 0)
		{
			if(errno = EWOULDBLOCK)
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
			}
			else
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
			}
			//perror("READ DATA FROM ELEC TIME OUT!!!\n");
			write_record();
			exit(1);
		}
    }
    alarm(0);
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"buf----%s\n",buf);
    baochanglength = stringtoint(buf,0,3);
    readbaochanglength=0;
    tmp=baochanglength;
    alarm(TIME_OUT);
    while(readbaochanglength!=tmp)
    {
        if((buflen = recv(fd,temp,baochanglength,0))>0)
        {
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baowen----%s\n",temp);
            readbaochanglength+=buflen;
            baochanglength-=buflen;
            for(i=0; i<buflen; i++)
            {
                buf[sum++] = temp[i];
            }
            memset(temp,'\0',sizeof(temp));
        }
		else
		{
			if(errno = EWOULDBLOCK)
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
			}
			else
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
			}
			//perror("READ DATA FROM ELEC TIME OUT!!!\n");
			write_record();
			exit(1);
		}

    }

	alarm(0);
    //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"buf----%s\n",buf);
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"---------------------------------\n");
    return sum;

}
int read_data_from_elec(int fd,char *buf)
{
	int inlen,outlen;
	int baochangint;
	char baochang[6];
    int i,sum=0;
    int buflen;
    char temp[BUFFER_SIZE];
	//	int sendbytes;
    memset(buf,'\0',sizeof(buf));
    memset(temp,'\0',sizeof(temp));
    int baochanglength = 25;
    int readbaochanglength=0;
    int tmp=baochanglength;

	struct timeval mytime;
	mytime.tv_sec = TIME_OUT;
	mytime.tv_usec = 0;

	//setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &mytime, sizeof(mytime));
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"第一个----------%d-----------\n",baochanglength);
	alarm(TIME_OUT);
    while(readbaochanglength!=tmp)
    {
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
	
        if((buflen = recv(fd,temp,baochanglength,0))>0)
        {
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"第一个buflen---%d\n",buflen);
            readbaochanglength+=buflen;
            baochanglength-=buflen;
            for(i=0; i<buflen; i++)
            {
                buf[sum++] = temp[i];
            }
            memset(temp,'\0',sizeof(temp));
        }
		else //if (buflen == 0)
		{
			//printf("%d\n",buflen);
			//printf("%d\n",SOCKET_ERROR);
			if(errno = EWOULDBLOCK)
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
				continue;
			}
			else
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
				continue;
			}
			//perror("READ DATA FROM ELEC TIME OUT!!!\n");
			write_record();
			exit(1);
		}

    }
    alarm(0);
    baochanglength = stringtoint(buf,20,24);
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"----------%d-----------\n",baochanglength);
    readbaochanglength=0;
    tmp=baochanglength;
    alarm(TIME_OUT);
    while(readbaochanglength!=tmp)
    {
        if((buflen = recv(fd,temp,baochanglength,0))>0)
        {
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"buflen---%d\n",buflen);
            readbaochanglength+=buflen;
            baochanglength-=buflen;
            for(i=0; i<buflen; i++)
            {
                buf[sum++] = temp[i];
            }
            memset(temp,'\0',sizeof(temp));
        }
		else //if (buflen == 0)
		{
			if(errno = EWOULDBLOCK)
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][%s%d][%s][系统超时]\n",__FUNCTION__,__LINE__,strerror(errno));
				continue;
			}
			else
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][%s%d][%s]\n",__FUNCTION__,__LINE__,strerror(errno));
				continue;
			}
			//perror("READ DATA FROM ELEC TIME OUT!!!\n");
			write_record();
			exit(1);
		}
    }

alarm(0);
sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][转码之前][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(buf),buf);
 
	//    sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(buf),getpid(),buf);

	memset(temp,'\0',BUFFER_SIZE);
	inlen=strlen(buf);
	outlen=BUFFER_SIZE;
	code_convert(FROM_CODE,TO_CODE,buf,inlen,temp,outlen);
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

	
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(temp),getpid(),temp);
	memset(baochang,'\0',6);
	baochangint=strlen(temp)-HEAD_LENGTH;
	//       sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
	i=4;
	while(i>=0)
	{
		if(baochangint==0)
		{
			baochang[i]='0';
		}
		else
		{
			baochang[i]=baochangint%10+'0';
			baochangint=baochangint/10;
		}
		i--;
	}
	// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
	tmp+=insertelec(buf,20,6,baochang);
    //   sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(buf),getpid(),buf);

	// memset(send,'\0',BUFFER_SIZE);
	inlen=strlen(buf);
	outlen=BUFFER_SIZE;
	code_convert(FROM_CODE,TO_CODE,buf,inlen,temp,outlen);
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(temp),getpid(),temp);
	memset(buf,'\0',BUFFER_SIZE);
	strcpy(buf,temp);
	//		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(buf),getpid(),buf);



	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"---------------------------------\n");
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s\n",buf);
    return sum;
}
int read_data_from_guangdian(int fd,char *buf)
{
	int i,sum=0;
	int buflen;
	char temp[BUFFER_SIZE];
	//	int sendbytes;
	memset(buf,'\0',sizeof(buf));
	memset(temp,'\0',sizeof(temp));
	int baochanglength = 10;
	int readbaochanglength=0;
	int tmp=baochanglength;
	struct timeval mytime;
	mytime.tv_sec = TIME_OUT;
	mytime.tv_usec = 0;

	//setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &mytime, sizeof(mytime));
	alarm(TIME_OUT);
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"第一个----------%d-----------\n",baochanglength);
	while(readbaochanglength!=tmp)
	{
		if((buflen = recv(fd,temp,baochanglength,0))>0)
		{
			//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"第一个buflen---%d\n",buflen);
			readbaochanglength+=buflen;
			baochanglength-=buflen;
			for(i=0;i<buflen;i++)
			{
				buf[sum++] = temp[i];
			}
			memset(temp,'\0',sizeof(temp));
		}
		else //if (buflen == 0)
		{
			if(errno = EWOULDBLOCK)
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
				continue;
			}
			else
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
				continue;
			}
			//perror("READ DATA FROM ELEC TIME OUT!!!\n");
			write_record();
			exit(1);
		}
	}
	alarm(0);
	baochanglength = stringtoint(buf,2,9)-10;
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"----------%d-----------\n",baochanglength);
	readbaochanglength=0;
	tmp=baochanglength;
	alarm(TIME_OUT);
	while(readbaochanglength!=tmp)
	{
		if((buflen = recv(fd,temp,baochanglength,0))>0)
		{
			//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"buflen---%d\n",buflen);
			readbaochanglength+=buflen;
			baochanglength-=buflen;
			for(i=0;i<buflen;i++)
			{
				buf[sum++] = temp[i];
			}
			memset(temp,'\0',sizeof(temp));
		}
		else //if (buflen == 0)
		{
			if(errno = EWOULDBLOCK)
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
				continue;
			}
			else
			{
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
				continue;
			}
			//perror("READ DATA FROM ELEC TIME OUT!!!\n");
			write_record();
			exit(1);
		}
	}
alarm(0);


	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"---------------------------------\n");
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s\n",buf);
	return sum;
}


int read_data_from_java_gdpayservice(int fd)
{
	int i,sum=0;
	int buflen;
	char temp[BUFFER_SIZE];
	char baochangcharlengthtemp[5];
	char baochangcharlength[5];
	int baochanglength;
	int readbaochanglength;
	int tmp;
	//	int sendbytes;
	struct timeval mytime;
	mytime.tv_sec = TIME_OUT;
	mytime.tv_usec = 0;
	/*if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &mytime, sizeof(mytime)) < 0)
	{
		perror("fail to setsockopt");
		write_record();
		exit(-1);
	}*/
	for(i=0;i<JAVA_SEND_DINGGOUTAOCAN_REAL_QUE_ZI_DUAN;i++)
	{
		memset(baochangcharlength,'\0',sizeof(baochangcharlength));
		memset(baochangcharlengthtemp,'\0',sizeof(baochangcharlengthtemp));
		//memset(buf,'\0',sizeof(buf));
		memset(temp,'\0',sizeof(temp));
		baochanglength = 4;
		readbaochanglength=0;
		tmp=baochanglength;
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"第一个----------%d-----------\n",baochanglength);
		alarm(TIME_OUT);
		while(readbaochanglength!=tmp)
		{
			if((buflen = recv(fd,temp,baochanglength,0))>0)
			{
			//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"第一个buflen---%d\n",buflen);
				readbaochanglength+=buflen;
				baochanglength-=buflen;
				strcat(baochangcharlengthtemp,temp);
				memset(temp,'\0',sizeof(temp));
			}
			else //if (buflen == 0)
			{
				if(errno = EWOULDBLOCK)
				{
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
					continue;
				}
				else
				{
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
					continue;
				}
				//perror("READ DATA FROM ELEC TIME OUT!!!\n");
				write_record();
				exit(1);
			}
			/*else //if (buflen == 0)
			{
				//perror("READ DATA FROM JAVA TIME OUT!!!\n");
				write_record();
				exit(1);
			}*/
		}
		alarm(0);
		DeleteTailSpace(baochangcharlengthtemp,baochangcharlength);
		baochanglength = stringtoint(baochangcharlengthtemp,0,strlen(baochangcharlength)-1);
		
		readbaochanglength=0;
		tmp=baochanglength;
		memset(guangdiansenddinggoutaocanjiexi[i].value,'\0',sizeof(guangdiansenddinggoutaocanjiexi[i].value));
		alarm(TIME_OUT);
		while(readbaochanglength!=tmp)
		{
			if((buflen = recv(fd,temp,baochanglength,0))>0)
			{
			//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"buflen---%d\n",buflen);
				readbaochanglength+=buflen;
				baochanglength-=buflen;
				strcat(guangdiansenddinggoutaocanjiexi[i].value,temp);
				memset(temp,'\0',sizeof(temp));
			}
			else //if (buflen == 0)
			{
				if(errno = EWOULDBLOCK)
				{
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s][系统超时]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
					continue;
				}
				else
				{
					sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][错误码：%d，错误信息：%s]\n",__FUNCTION__,__LINE__,errno,strerror(errno));
					continue;
				}
				//perror("READ DATA FROM ELEC TIME OUT!!!\n");
				write_record();
				exit(1);
			}
			
		}
		alarm(0);
	}
	
	for(i=0;i<JAVA_SEND_DINGGOUTAOCAN_REAL_QUE_ZI_DUAN;i++)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][接收本地GDPayService][%d][%s]\n",getpid(),__FUNCTION__,__LINE__,strlen(guangdiansenddinggoutaocanjiexi[i].value),guangdiansenddinggoutaocanjiexi[i].value);
	
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"接收本地java[%d][%s]\n",strlen(guangdiansenddinggoutaocanjiexi[i].value),guangdiansenddinggoutaocanjiexi[i].value);
	}
	return 1;
}
int send_data_to_guangdian(char *sendbao)
{
	struct hostent *host;
	struct sockaddr_in serv_addr;
	int sockfd;
	if((host = gethostbyname(GUANGDIAN_HOST_NAME)) == NULL)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(1);
	}
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(GUANGDIANPORT);
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
	//		inet_pton(AF_INET, HOST_NAME, (void *)&serv_addr.sin_addr);
	bzero(&(serv_addr.sin_zero),8);
	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(1);
	}
	if(send_data(sockfd,sendbao) <= 0)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(1);
	}
	return sockfd;
}


int send_data_to_java_gdpayservice()
{
	struct hostent *host;
	struct sockaddr_in serv_addr;
	int sockfd;
	int i,j;
	int len,lengthlenchar;
	char lenchar[5];
	char temptaocanbianhao[3];
	int selecttaocan;
	if((host = gethostbyname(JAVA_GDPAYSERVICE_HOST_NAME)) == NULL)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(1);
	}
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(LOCALJAVAGDPAYSERVICEPORT);
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
	//		inet_pton(AF_INET, HOST_NAME, (void *)&serv_addr.sin_addr);
	bzero(&(serv_addr.sin_zero),8);
	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(1);
	}
	
	
	
	for(i=0;i<JIAO_FEI_REAL_QUE_ZI_DUAN;i++)
	{
		len=strlen(jiaofeijiexi[i].value);
		inttostring(len,lenchar);
		lengthlenchar=strlen(lenchar);
		//将lenchar后补空格
		if(lengthlenchar==1)
		{
			lenchar[1]=' ';
			lenchar[2]=' ';
			lenchar[3]=' ';
			lenchar[4]='\0';
			
		}
		else if(lengthlenchar==2)
		{
			lenchar[2]=' ';
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		else if(lengthlenchar==3)
		{
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		send_data(sockfd,lenchar);
		send_data(sockfd,jiaofeijiexi[i].value);		
	}
	
	for(i=0;i<GUANGDIAN_SEND_YONGHUCHAXUN_REAL_QUE_ZI_DUAN;i++)
	{
		len=strlen(guangdiansendyonghuchaxunjiexi[i].value);
		inttostring(len,lenchar);
		lengthlenchar=strlen(lenchar);
		//将lenchar后补空格
		if(lengthlenchar==1)
		{
			lenchar[1]=' ';
			lenchar[2]=' ';
			lenchar[3]=' ';
			lenchar[4]='\0';
			
		}
		else if(lengthlenchar==2)
		{
			lenchar[2]=' ';
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		else if(lengthlenchar==3)
		{
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		send_data(sockfd,lenchar);
		send_data(sockfd,guangdiansendyonghuchaxunjiexi[i].value);		
	}

	for(i=0;i<GUANGDIAN_SEND_TAOCANMULU_REAL_QUE_ZI_DUAN;i++)
	{
		if(i<=4)
		{
			len=strlen(guangdiansendtaocanmulujiexi[i].value);
			inttostring(len,lenchar);
			lengthlenchar=strlen(lenchar);
			//将lenchar后补空格
			if(lengthlenchar==1)
			{
				lenchar[1]=' ';
				lenchar[2]=' ';
				lenchar[3]=' ';
			lenchar[4]='\0';
			
			}
		else if(lengthlenchar==2)
		{
			lenchar[2]=' ';
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		else if(lengthlenchar==3)
		{
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		send_data(sockfd,lenchar);
		send_data(sockfd,guangdiansendtaocanmulujiexi[i].value);		
		}
		
		else
		{
			memset(temptaocanbianhao,'\0',sizeof(temptaocanbianhao));
			temptaocanbianhao[0]=jiaofeijiexi[14].value[2];
			temptaocanbianhao[1]=jiaofeijiexi[14].value[3];  //提取银行套餐选项编号字段中的后两位  00-查询全部  01-10查询某一个
			selecttaocan=SelectTaoCanNo(temptaocanbianhao);
			for(j=0;j<3;j++)
			{
				len=strlen(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[j]);
			inttostring(len,lenchar);
			lengthlenchar=strlen(lenchar);
			//将lenchar后补空格
			if(lengthlenchar==1)
			{
				lenchar[1]=' ';
				lenchar[2]=' ';
				lenchar[3]=' ';
			lenchar[4]='\0';
			
			}
		else if(lengthlenchar==2)
		{
			lenchar[2]=' ';
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		else if(lengthlenchar==3)
		{
			lenchar[3]=' ';
			lenchar[4]='\0';
		}
		send_data(sockfd,lenchar);
		send_data(sockfd,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[j]);		
			}
		}
	
	}
	

return sockfd;
}


int send_data_to_elec(char *sendbao)
{
    struct hostent *host;
    struct sockaddr_in serv_addr;
    int sockfd;
    if((host = gethostbyname(ELEC_HOST_NAME)) == NULL)
    {
       sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        exit(1);
    }
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
       sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ELECPORT);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
	//		inet_pton(AF_INET, HOST_NAME, (void *)&serv_addr.sin_addr);
    bzero(&(serv_addr.sin_zero),8);
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
    {
       sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        exit(1);
    }
    if(send_data(sockfd,sendbao)<= 0)
    {
       sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        exit(1);
    }
    return sockfd;
}




/*0----套餐目录查询
 *1----订购套餐
 *2----用户查询
 */

void chongzufabao_to_guangdian(char *send,int n)//////////////////////////////////////
{
	int pos=0;
	int i=0;
	char temp[3];
	memset(temp,'\0',sizeof(temp));
	char temptaocanbianhao[3];
	int selecttaocan;
	memset(temptaocanbianhao,'\0',sizeof(temptaocanbianhao));
	if(n==0)//chaxun
	{
		insertguangdian(send,pos,guangdianreceivetaocanmulu[0].length,"GD");
		pos+=guangdianreceivetaocanmulu[0].length;

		insertguangdian(send,pos,guangdianreceivetaocanmulu[1].length,"41      ");
		pos+=guangdianreceivetaocanmulu[1].length;

		insertguangdian(send,pos,guangdianreceivetaocanmulu[2].length,"500004");
		pos+=guangdianreceivetaocanmulu[2].length;

		insertguangdian(send,pos,guangdianreceivetaocanmulu[3].length,"80000300");
		pos+=guangdianreceivetaocanmulu[3].length;
		temp[0]=qianfeijiexi[14].value[0];
		temp[1]=qianfeijiexi[14].value[1];
		if(!strcmp(temp,"09"))
		{
			insertguangdian(send,pos,guangdianreceivetaocanmulu[4].length,"2");  //宽带账号
		}
		else
		{
			insertguangdian(send,pos,guangdianreceivetaocanmulu[4].length,"1");
		}

		pos+=guangdianreceivetaocanmulu[4].length;
		insertguangdian(send,pos,guangdianreceivetaocanmulu[5].length,qianfeijiexi[13].value);
		pos+=guangdianreceivetaocanmulu[5].length;

	}
	else if(n==1)//jiaofei
	{
		insertguangdian(send,pos,guangdianreceivedinggoutaocan[0].length,"GD");
		pos+=guangdianreceivedinggoutaocan[0].length;
		insertguangdian(send,pos,guangdianreceivedinggoutaocan[1].length,"60      ");
		pos+=guangdianreceivedinggoutaocan[1].length;
		insertguangdian(send,pos,guangdianreceivedinggoutaocan[2].length,"500005");
		pos+=guangdianreceivedinggoutaocan[2].length;
		insertguangdian(send,pos,guangdianreceivedinggoutaocan[3].length,"80000300");
		pos+=guangdianreceivedinggoutaocan[3].length;
		
		temp[0]=qianfeijiexi[14].value[0];
		temp[1]=qianfeijiexi[14].value[1];
		temptaocanbianhao[0]=qianfeijiexi[14].value[2];
        temptaocanbianhao[1]=qianfeijiexi[14].value[3];  //提取银行套餐选项编号字段中的后两位  00-查询全部  01-10查询某一个
        selecttaocan=SelectTaoCanNo(temptaocanbianhao);
		if(!strcmp(temp,"09"))
		{
			insertguangdian(send,pos,guangdianreceivedinggoutaocan[4].length,"2");  //宽带账号
		}
		else
		{
			insertguangdian(send,pos,guangdianreceivedinggoutaocan[4].length,"1");
		}
		pos+=guangdianreceivedinggoutaocan[4].length;
		insertguangdian(send,pos,guangdianreceivedinggoutaocan[5].length,qianfeijiexi[13].value);
		pos+=guangdianreceivedinggoutaocan[5].length;
		if(selecttaocan==-1)
		{
			insertguangdian(send,pos,guangdianreceivedinggoutaocan[6].length,"");
			pos+=guangdianreceivedinggoutaocan[6].length;
			insertguangdian(send,pos,guangdianreceivedinggoutaocan[7].length,"");
			pos+=guangdianreceivedinggoutaocan[7].length;
		}
		else
		{
			insertguangdian(send,pos,guangdianreceivedinggoutaocan[6].length,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[0]);
			pos+=guangdianreceivedinggoutaocan[6].length;
			insertguangdian(send,pos,guangdianreceivedinggoutaocan[7].length,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2]);
			pos+=guangdianreceivedinggoutaocan[7].length;
		}
		

	}
	else if(n==2)//用户查询
	{
		insertguangdian(send,pos,guangdianreceiveyonghuchaxun[0].length,"GD");
		pos+=guangdianreceiveyonghuchaxun[0].length;
		insertguangdian(send,pos,guangdianreceiveyonghuchaxun[1].length,"41      ");
		pos+=guangdianreceiveyonghuchaxun[1].length;
		insertguangdian(send,pos,guangdianreceiveyonghuchaxun[2].length,"500006");
		pos+=guangdianreceiveyonghuchaxun[2].length;
		insertguangdian(send,pos,guangdianreceiveyonghuchaxun[3].length,"80000300");
		pos+=guangdianreceiveyonghuchaxun[3].length;
		temp[0]=qianfeijiexi[14].value[0];
		temp[1]=qianfeijiexi[14].value[1];
		if(!strcmp(temp,"09"))
		{
			insertguangdian(send,pos,guangdianreceiveyonghuchaxun[4].length,"2");  //宽带账号
		}
		else
		{
			insertguangdian(send,pos,guangdianreceiveyonghuchaxun[4].length,"1");
		}

		pos+=guangdianreceiveyonghuchaxun[4].length;
		insertguangdian(send,pos,guangdianreceiveyonghuchaxun[5].length,qianfeijiexi[13].value);
		pos+=guangdianreceiveyonghuchaxun[5].length;

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%drow][%d][%s]\n",__LINE__,strlen(send),send);
	}
	else
	{
	}

	return;
}
/*0----代收欠费查询
 *1----代收缴费
 *2----代收冲正
 *3----代收对账
 *4----签解约
 *5----读卡信息查询
 *6----卡表购电
 *7----卡表购电冲正
 *8----补写卡
 *9----缴费票据查询
 *10---票据打印信息查询
 */

void chongzufabao_to_dianwang(char *send,int n)
{
	char jine[12];
    char temp[BUFFER_SIZE];
    char dest16[17];
    int pos=0,tmp;
    int i,j,k;
    char baochang[6];
    int baochangint;
    char date_and_time[14]; //shijian hecheng  14wei
    int inlen,outlen;
    char sendbuf[BUFFER_SIZE];
	char putongorkabiaoforpiaoju[100];
	char filenametemp[512];
	char *token;
	char *pfilenametemp;
	memset(filenametemp,'\0',512);
	memset(putongorkabiaoforpiaoju,'\0',100);
    memset(date_and_time,'\0',14);
    memset(baochang,'\0',6);
	memset(temp,'\0',BUFFER_SIZE);
	memset(dest16,'\0',17);
    if(n==0)//chaxunbao
    {

		//		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s\n",dest16);
        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[0].length,"1015");//xi tong bianhao-----
        tmp=pos;
        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[1].length,"0000000000000000");

        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[2].length,"00000");//cha xun bao chang

        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[3].length,"200001");//jiaoyi daima

        pos+=insertelec(send,pos,1,"|");
		for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(qianfeijiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(qianfeijiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,qianfeitodianwangpeizhi[4].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,qianfeitodianwangpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,qianfeitodianwangpeizhi[4].length,"");//qudao shang bianhao
		}
      /*  for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(qianfeijiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,qianfeitodianwangpeizhi[4].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,qianfeitodianwangpeizhi[4].length,"");//qudao shang bianhao
		}
		*/
        pos+=insertelec(send,pos,1,"|");

		//   sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d-----%s",strlen(qianfeijiexi[6].value),qianfeijiexi[6].value);


        sprintf(date_and_time,"%s%s",qianfeijiexi[3].value,qianfeijiexi[4].value);
        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[5].length,date_and_time);//jiaoyi shijian

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[6].length,qianfeijiexi[2].value);//jiaoyi liushuihao-----

        pos+=insertelec(send,pos,1,"|");
		
		for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(qianfeijiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(qianfeijiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,qianfeitodianwangpeizhi[7].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,qianfeitodianwangpeizhi[8].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,qianfeitodianwangpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,qianfeitodianwangpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,qianfeitodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,qianfeitodianwangpeizhi[8].length,"");
		}
		
      /*  for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(qianfeijiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,qianfeitodianwangpeizhi[7].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,qianfeitodianwangpeizhi[8].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,qianfeitodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,qianfeitodianwangpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[9].length,"01");//chaxunhao leixing

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[10].length,qianfeijiexi[13].value);//chaxunhao bianhao----

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[11].length,"");//yingshou yuefen

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,qianfeitodianwangpeizhi[12].length,qianfeijiexi[3].value);//yinghang zhangwu riqi----

        pos+=insertelec(send,pos,1,"|");
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//     sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
		
		for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(qianfeijiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(qianfeijiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
		}
		
		
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(qianfeijiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}*/
        tmp+=insertelec(send,tmp,qianfeitodianwangpeizhi[1].length,dest16);//MAC jiaoyanma-----


		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,qianfeitodianwangpeizhi[2].length,baochang);
		//  sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);
    }
    else if(n==1)//jiaofeibao
    {

        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[0].length,"1015");//xi tong bianhao-----


        tmp=pos;
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[1].length,"0000000000000000");



        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[2].length,"00000");//cha xun bao chang




        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[3].length,"200002");//jiaoyi daima

        pos+=insertelec(send,pos,1,"|");
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(jiaofeijiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(jiaofeijiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[4].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[4].length,"");//qudao shang bianhao
		}
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(jiaofeijiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[4].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[4].length,"");//qudao shang bianhao
		}*/
        pos+=insertelec(send,pos,1,"|");
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend-----%s\n",pos,send);


        sprintf(date_and_time,"%s%s",jiaofeijiexi[3].value,jiaofeijiexi[4].value);
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[5].length,date_and_time);//jiaoyi shijian

        pos+=insertelec(send,pos,1,"|");
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend-----%s\n",pos,send);

        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[6].length,jiaofeijiexi[2].value);//jiaoyi liushuihao-----

        pos+=insertelec(send,pos,1,"|");
        
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(jiaofeijiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(jiaofeijiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[7].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[8].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[8].length,"");
		}
        
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(jiaofeijiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[7].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[8].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[9].length,qianfeifanhuijiexi[7].value);//yonghu bianhao ---------yinyong chaxunchucan

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[10].length,qianfeifanhuijiexi[6].value);//dianli zijin jiesuan danwei---------yinyong chaxun chucan
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[11].length,qianfeifanhuijiexi[5].value);//dianli zijin bianhao ----------yinyong chaxun chucan
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[12].length,"");//dingdan bianhao------------?
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[13].length,"0201");//jiaofei fangshi---------
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[14].length,"09");//zhifu fangshi
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[15].length,"01");//yinhang jiaofei fangshi 01 02 03 04
        pos+=insertelec(send,pos,1,"|");
        //pos+=insertelec(send,pos,6,"000000");//jiaoyi jin'e-----
        k=0;
        memset(jine,'\0',12);
		for(j=0;j<12;j++)
		{
			if(jiaofeijiexi[16].value[j]=='0')
			{
				continue;
			}
			else
			{
				break;
			}
		}
		if(jiaofeijiexi[16].value[j]=='.')
		{
			if(jiaofeijiexi[16].value[j+1]=='0')
			{
				jine[0]=jiaofeijiexi[16].value[j+2];
			}
			else
			{
				jine[0]=jiaofeijiexi[16].value[j+1];
				jine[1]=jiaofeijiexi[16].value[j+2];
			}
		}
		else
		{
			for(;j<12;j++)
			{
				if(jiaofeijiexi[16].value[j]=='.')
				{
					continue;
				}
				else
				{
					jine[k++]=jiaofeijiexi[16].value[j];
				}
			}
		}
		//在这存下缴费金额
		jineb=stringtoint(jine,0,strlen(jine)-1);

        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[16].length,jine);
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[17].length,jiaofeijiexi[3].value);//yinhang zhangwu riqi
        pos+=insertelec(send,pos,1,"|");


	//这里写判断逻辑
/*增加判断实缴金额与查询返回结果金额大小的比较逻辑*/
		int newjilushu;
		int yitianjine=0;//已经填进前面月份里的金额
		newjilushu = stringtoint(qianfeifanhuijiexi[13].value,0,strlen(qianfeifanhuijiexi[13].value)-1);




         pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[18].length,qianfeifanhuijiexi[13].value);//jilu shu----------??
		 pos+=insertelec(send,pos,1,"|");
//		 sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d,%d,%d\n",jinea,jineb,jinec);
		if(jineb+jinec>=jinea)
		{
			for(i=0;i<newjilushu;i++)
			{
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[19].length,qianfeifanhuijiexijilu[i].value[0]);//应收电费标识
				pos+=insertelec(send,pos,1,"&");
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[20].length,qianfeifanhuijiexijilu[i].value[1]);//应收年月
				pos+=insertelec(send,pos,1,"&");
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[21].length,qianfeifanhuijiexijilu[i].value[2]);//本次应缴电费
				pos+=insertelec(send,pos,1,"&");
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[22].length,qianfeifanhuijiexijilu[i].value[3]);//本次应缴违约金
				pos+=insertelec(send,pos,1,"&");
				if(i==newjilushu-1)
				{
					char newjine[13];
					inttostring(jineb+jinec-yitianjine,newjine);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"newjine----%s\n",newjine);
					pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[23].length,newjine);
				}
				else
				{
					char newjine[13];
					yitianjine+=stringtoint(qianfeifanhuijiexijilu[i].value[2],0,strlen(qianfeifanhuijiexijilu[i].value[2])-1)+stringtoint(qianfeifanhuijiexijilu[i].value[3],0,strlen(qianfeifanhuijiexijilu[i].value[3])-1);
					inttostring(stringtoint(qianfeifanhuijiexijilu[i].value[2],0,strlen(qianfeifanhuijiexijilu[i].value[2])-1)+stringtoint(qianfeifanhuijiexijilu[i].value[3],0,strlen(qianfeifanhuijiexijilu[i].value[3])-1),newjine);
					pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[23].length,newjine);//本次实收金额,b+c>a
				}
				pos+=insertelec(send,pos,1,"&");
				pos+=insertelec(send,pos,1,";");
			}
			 pos+=insertelec(send,pos,1,"|");
		}
		else
		{
			int somejine=0;
			int flag_null=0;
			for(i=0;i<newjilushu;i++)
			{
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[19].length,qianfeifanhuijiexijilu[i].value[0]);
				pos+=insertelec(send,pos,1,"&");
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[20].length,qianfeifanhuijiexijilu[i].value[1]);
				pos+=insertelec(send,pos,1,"&");
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[21].length,qianfeifanhuijiexijilu[i].value[2]);
				pos+=insertelec(send,pos,1,"&");
				somejine+=stringtoint(qianfeifanhuijiexijilu[i].value[2],0,strlen(qianfeifanhuijiexijilu[i].value[2])-1);
				pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[22].length,qianfeifanhuijiexijilu[i].value[3]);
				somejine+=stringtoint(qianfeifanhuijiexijilu[i].value[3],0,strlen(qianfeifanhuijiexijilu[i].value[3])-1);
				pos+=insertelec(send,pos,1,"&");
				if((somejine>=jineb+jinec)&&(flag_null==0))
				{
					char newjine[13];
					inttostring(jineb+jinec-yitianjine,newjine);
					pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[23].length,newjine);
					flag_null=1;

				}
				else
				{
					if(flag_null==1)
					{
						pos+=insertelec(send,pos,1,"0");
					}
					else
					{
						char newjine[13];
						if(newjilushu==1)
						{
							inttostring(jineb+jinec,newjine);
							pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[23].length,newjine);
						}
						else
						{
							yitianjine+=stringtoint(qianfeifanhuijiexijilu[i].value[2],0,strlen(qianfeifanhuijiexijilu[i].value[2])-1)+stringtoint(qianfeifanhuijiexijilu[i].value[3],0,strlen(qianfeifanhuijiexijilu[i].value[3])-1);
							inttostring(stringtoint(qianfeifanhuijiexijilu[i].value[2],0,strlen(qianfeifanhuijiexijilu[i].value[2])-1)+stringtoint(qianfeifanhuijiexijilu[i].value[3],0,strlen(qianfeifanhuijiexijilu[i].value[3])-1),newjine);
//							sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"newjine--%s\n",newjine);
							pos+=insertelec(send,pos,jiaofeitodianwangpeizhi[23].length,newjine);
						}

					}
				}
			 pos+=insertelec(send,pos,1,"&");
			 pos+=insertelec(send,pos,1,";");
			}
			 pos+=insertelec(send,pos,1,"|");
		}






		/**/
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%dsend-----%s\n",pos,send);
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(jiaofeijiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(jiaofeijiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
		}
        
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(jiaofeijiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}
		*/
        tmp+=insertelec(send,tmp,jiaofeitodianwangpeizhi[1].length,dest16);//MAC jiaoyanma-----




		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,jiaofeitodianwangpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);
    }
    else if(n==2) //chexiao qingqiu bao
    {

        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[0].length,"1015");//xi tong bianhao-----

        tmp=pos;
        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[1].length,"0000000000000000");
        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[2].length,"00000");//cha xun bao chang


        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[3].length,"200003");//jiaoyi daima
        pos+=insertelec(send,pos,1,"|");
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(chexiaojiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(chexiaojiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,chexiaotodianwangpeizhi[4].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,chexiaotodianwangpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,chexiaotodianwangpeizhi[4].length,"");//qudao shang bianhao
		}
        
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(chexiaojiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,chexiaotodianwangpeizhi[4].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,chexiaotodianwangpeizhi[4].length,"");//qudao shang bianhao
		}*/
        pos+=insertelec(send,pos,1,"|");

        sprintf(date_and_time,"%s%s",chexiaojiexi[3].value,chexiaojiexi[4].value);
        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[5].length,date_and_time);//jiaoyi shijian
        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[6].length,chexiaojiexi[2].value);//jiaoyi liushuihao-----
        pos+=insertelec(send,pos,1,"|");
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(chexiaojiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(chexiaojiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,chexiaotodianwangpeizhi[7].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,chexiaotodianwangpeizhi[8].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,chexiaotodianwangpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,chexiaotodianwangpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,chexiaotodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,chexiaotodianwangpeizhi[8].length,"");
		}
        
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(chexiaojiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,chexiaotodianwangpeizhi[7].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,chexiaotodianwangpeizhi[8].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,chexiaotodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,chexiaotodianwangpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[9].length,chexiaojiexi[13].value);//yinhangzhangwuriqi
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[10].length,chexiaojiexi[14].value);//yuan jiaoyi liushuihao--------------------
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,chexiaotodianwangpeizhi[11].length,chexiaojiexi[3].value);//yinhangzhangwuriqi
        pos+=insertelec(send,pos,1,"|");
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(chexiaojiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(chexiaojiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
			
		}
        
		/*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(chexiaojiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}*/
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"dest16---%d--%s\n",strlen(dest16),dest16);
        tmp+=insertelec(send,tmp,chexiaotodianwangpeizhi[1].length,dest16);//MAC jiaoyanma-----
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,chexiaotodianwangpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);

    }
    else if(n==3)
    {
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[0].length,"1015");//xi tong bianhao-----
        tmp=pos;
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[1].length,"0000000000000000");
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[2].length,"00000");//cha xun bao chang
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[3].length,"200004");//jiaoyi daima

        pos+=insertelec(send,pos,1,"|");
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(duizhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(duizhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,duizhangtodianwangpeizhi[4].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,duizhangtodianwangpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,duizhangtodianwangpeizhi[4].length,"");//qudao shang bianhao
		}
        
		/*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(duizhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,duizhangtodianwangpeizhi[4].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,duizhangtodianwangpeizhi[4].length,"");//qudao shang bianhao
		}*/
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",duizhangjiexi[3].value,duizhangjiexi[4].value);
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[5].length,date_and_time);//jiaoyi shijian

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[6].length,duizhangjiexi[2].value);//jiaoyi liushuihao-----

        pos+=insertelec(send,pos,1,"|");
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(duizhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(duizhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,duizhangtodianwangpeizhi[7].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,duizhangtodianwangpeizhi[8].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,duizhangtodianwangpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,duizhangtodianwangpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,duizhangtodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,duizhangtodianwangpeizhi[8].length,"");
		}
		/*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(duizhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,duizhangtodianwangpeizhi[7].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,duizhangtodianwangpeizhi[8].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,duizhangtodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,duizhangtodianwangpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");
	
	
		strcpy(filenametemp,duizhangjiexi[15].value);
		pfilenametemp = filenametemp;
		token=strsep(&pfilenametemp,"_");
		token=strsep(&pfilenametemp,"_");
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[9].length,token);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[10].length,duizhangjiexi[14].value);//yuan jiaoyi liushuihao--------------------

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,duizhangtodianwangpeizhi[11].length,duizhangjiexi[15].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
		 for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(duizhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(duizhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
		}
		
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(duizhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}*/
        tmp+=insertelec(send,tmp,duizhangtodianwangpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,duizhangtodianwangpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);
    }
    else if(n==4)
    {
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[0].length,"1015");//xi tong bianhao-----

        tmp=pos;
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[1].length,"0000000000000000");

        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[2].length,"00000");//cha xun bao chang



        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[3].length,"200021");//jiaoyi daima

        pos+=insertelec(send,pos,1,"|");
        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(daikoubankqianjieyuejiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[4].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[4].length,"");//qudao shang bianhao
		}
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",daikoubankqianjieyuejiexi[3].value,daikoubankqianjieyuejiexi[4].value);
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[5].length,date_and_time);//jiaoyi shijian

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[6].length,daikoubankqianjieyuejiexi[2].value);//jiaoyi liushuihao-----

        pos+=insertelec(send,pos,1,"|");
        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(daikoubankqianjieyuejiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[7].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[8].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[8].length,"");
		}
        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[9].length,daikoubankqianjieyuejiexi[13].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[10].length,daikoubankqianjieyuejiexi[14].value);//yuan jiaoyi liushuihao--------------------

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[11].length,daikoubankqianjieyuejiexi[15].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[12].length,daikoubankqianjieyuejiexi[16].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[13].length,daikoubankqianjieyuejiexi[17].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[14].length,daikoubankqianjieyuejiexi[19].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[15].length,daikoubankqianjieyuejiexi[20].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[16].length,daikoubankqianjieyuejiexi[21].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,daikoubankqianjieyuetodianwangpeizhi[17].length,daikoubankqianjieyuejiexi[22].value);//yinhangzhangwuriqi

        pos+=insertelec(send,pos,1,"|");
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(daikoubankqianjieyuejiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}
        tmp+=insertelec(send,tmp,daikoubankqianjieyuetodianwangpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,daikoubankqianjieyuetodianwangpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);
    }
    else if(n==5) //读卡信息查询
    {
		pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[0].length,"1015");//系统编号
        tmp=pos;
        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[1].length,"0000000000000000"); //MAC
        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[2].length,"00000");//包长
        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[3].length,"200005");//交易码
        pos+=insertelec(send,pos,1,"|");
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(dukachaxunfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(dukachaxunfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[4].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
		}
        
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //渠道商编号
        {
			if(!strcmp(dukachaxunfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[4].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
		}*/
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",dukachaxunfromyinhangjiexi[3].value,dukachaxunfromyinhangjiexi[4].value);
        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[5].length,date_and_time);//交易时间

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[6].length,dukachaxunfromyinhangjiexi[2].value);//交易流水号

        pos+=insertelec(send,pos,1,"|");
        
        
         for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(dukachaxunfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(dukachaxunfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[7].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[8].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[8].length,"");
		}
        
      /*  for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //网点代码and操作员编号
        {
			if(!strcmp(dukachaxunfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[7].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[8].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[9].length,dukachaxunfromyinhangjiexi[13].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[10].length,dukachaxunfromyinhangjiexi[14].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[11].length,dukachaxunfromyinhangjiexi[15].value);

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[12].length,dukachaxunfromyinhangjiexi[16].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,dukachaxuntoelecpeizhi[13].length,dukachaxunfromyinhangjiexi[17].value);

        pos+=insertelec(send,pos,1,"|");
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
		
		 for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(dukachaxunfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(dukachaxunfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
		}
		
		
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(dukachaxunfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}*/
        tmp+=insertelec(send,tmp,dukachaxuntoelecpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,dukachaxuntoelecpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);
//		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d加入包长转码之前----------\n[%d][%s]\n",getpid(),strlen(send),send);
		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);
    }
    else if(n==6) //卡表够电
    {
		pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[0].length,"1015");//系统编号
        tmp=pos;
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[1].length,"0000000000000000"); //MAC
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[2].length,"00000");//包长
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[3].length,"200006");//交易码
        pos+=insertelec(send,pos,1,"|");
        
        for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(kabiaogoudianfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(kabiaogoudianfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[4].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[4].length,"");//qudao shang bianhao
		}
        
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //渠道商编号
        {
			if(!strcmp(kabiaogoudianfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[4].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[4].length,"");//qudao shang bianhao
		}*/
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",kabiaogoudianfromyinhangjiexi[3].value,kabiaogoudianfromyinhangjiexi[4].value);
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[5].length,date_and_time);//交易时间

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[6].length,kabiaogoudianfromyinhangjiexi[2].value);//交易流水号

        pos+=insertelec(send,pos,1,"|");
        
         for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(kabiaogoudianfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(kabiaogoudianfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[7].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[8].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[8].length,"");
		}
        
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //网点代码and操作员编号
        {
			if(!strcmp(kabiaogoudianfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[7].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[8].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[9].length,kabiaogoudianfromyinhangjiexi[13].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[10].length,"0201");

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[11].length,"09");

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[12].length,"01");

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[13].length,kabiaogoudianfromyinhangjiexi[14].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[14].length,kabiaogoudianfromyinhangjiexi[3].value);

        pos+=insertelec(send,pos,1,"|");

		k=0;
        memset(jine,'\0',12);
		for(j=0;j<12;j++)
		{
			if(kabiaogoudianfromyinhangjiexi[15].value[j]=='0')
			{
				continue;
			}
			else
			{
				break;
			}
		}
	/*	if(kabiaogoudianfromyinhangjiexi[15].value[j]=='.')
		{
			if(kabiaogoudianfromyinhangjiexi[15].value[j+1]=='0'&&kabiaogoudianfromyinhangjiexi[15].value[j+2]=='0')
			{
				jine[0]='0';
			}
		}
		*/
		if(kabiaogoudianfromyinhangjiexi[15].value[j]=='.')
		{
			if(kabiaogoudianfromyinhangjiexi[15].value[j+1]=='0')
			{
				jine[0]=jiaofeijiexi[16].value[j+2];
			}
			else
			{
				jine[0]=kabiaogoudianfromyinhangjiexi[15].value[j+1];
				jine[1]=kabiaogoudianfromyinhangjiexi[15].value[j+2];
			}
		}
		else
		{
			for(;j<12;j++)
			{
				if(kabiaogoudianfromyinhangjiexi[15].value[j]=='.')
				{
					continue;
				}
				else
				{
					jine[k++]=kabiaogoudianfromyinhangjiexi[15].value[j];
				}
			}
		}

        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[15].length,jine);//交易金额

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[16].length,kabiaogoudianfromyinhangjiexi[16].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[17].length,kabiaogoudianfromyinhangjiexi[17].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[18].length,kabiaogoudianfromyinhangjiexi[18].value);
        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudiantoelecpeizhi[19].length,kabiaogoudianfromyinhangjiexi[19].value);

        pos+=insertelec(send,pos,1,"|");
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
		
		 for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(kabiaogoudianfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(kabiaogoudianfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
		}
		
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(kabiaogoudianfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}
		*/
        tmp+=insertelec(send,tmp,kabiaogoudiantoelecpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,kabiaogoudiantoelecpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);
//		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d加入包长转码之前----------\n[%d][%s]\n",getpid(),strlen(send),send);
		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);

    }
    else if(n==7) //卡表够电冲正
    {
		pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[0].length,"1015");//系统编号
        tmp=pos;
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[1].length,"0000000000000000"); //MAC
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[2].length,"00000");//包长
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[3].length,"200007");//交易码
        pos+=insertelec(send,pos,1,"|");
        
         for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[4].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[4].length,"");//qudao shang bianhao
		}
        
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //渠道商编号
        {
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[4].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[4].length,"");//qudao shang bianhao
		}*/
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",kabiaogoudianchongzhengfromyinhangjiexi[3].value,kabiaogoudianchongzhengfromyinhangjiexi[4].value);
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[5].length,date_and_time);//交易时间

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[6].length,kabiaogoudianchongzhengfromyinhangjiexi[2].value);//交易流水号

        pos+=insertelec(send,pos,1,"|");
        
         for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[7].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[8].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[8].length,"");
		}
        
        
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //网点代码and操作员编号
        {
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[7].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[8].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");


        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[9].length,kabiaogoudianchongzhengfromyinhangjiexi[13].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[10].length,kabiaogoudianchongzhengfromyinhangjiexi[3].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[11].length,kabiaogoudianchongzhengfromyinhangjiexi[14].value);

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[12].length,kabiaogoudianchongzhengfromyinhangjiexi[15].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[13].length,kabiaogoudianchongzhengfromyinhangjiexi[16].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[14].length,kabiaogoudianchongzhengfromyinhangjiexi[17].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[15].length,kabiaogoudianchongzhengfromyinhangjiexi[18].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[16].length,kabiaogoudianchongzhengfromyinhangjiexi[19].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,kabiaogoudianchongzhengtoelecpeizhi[17].length,kabiaogoudianchongzhengfromyinhangjiexi[20].value);

        pos+=insertelec(send,pos,1,"|");
        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
		
		
		
		 for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
		}
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}*/
        tmp+=insertelec(send,tmp,kabiaogoudianchongzhengtoelecpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,kabiaogoudianchongzhengtoelecpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d加入包长转码之前----------\n[%d][%s]\n",getpid(),strlen(send),send);
		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);




    }
    else if(n==8) //补写卡
    {
		pos+=insertelec(send,pos,buxiekatoelecpeizhi[0].length,"1015");//系统编号
        tmp=pos;
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[1].length,"0000000000000000"); //MAC
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[2].length,"00000");//包长
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[3].length,"200008");//交易码
        pos+=insertelec(send,pos,1,"|");
        
         for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(buxiekafromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(buxiekafromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,buxiekatoelecpeizhi[4].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,buxiekatoelecpeizhi[4].length,"");//qudao shang bianhao
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,buxiekatoelecpeizhi[4].length,"");//qudao shang bianhao
		}
        
        
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //渠道商编号
        {
			if(!strcmp(buxiekafromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,buxiekatoelecpeizhi[4].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,buxiekatoelecpeizhi[4].length,"");//qudao shang bianhao
		}*/
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",buxiekafromyinhangjiexi[3].value,buxiekafromyinhangjiexi[4].value);
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[5].length,date_and_time);//交易时间

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,buxiekatoelecpeizhi[6].length,buxiekafromyinhangjiexi[2].value);//交易流水号

        pos+=insertelec(send,pos,1,"|");
        
         for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(buxiekafromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(buxiekafromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						pos+=insertelec(send,pos,buxiekatoelecpeizhi[7].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,buxiekatoelecpeizhi[8].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
						break;
					}
				}
				if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
				{
					pos+=insertelec(send,pos,buxiekatoelecpeizhi[7].length,"");
					pos+=insertelec(send,pos,1,"|");
					pos+=insertelec(send,pos,buxiekatoelecpeizhi[8].length,"");
				}
				break;
			}
		}
		if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,buxiekatoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,buxiekatoelecpeizhi[8].length,"");
		}
        
        
        /*for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //网点代码and操作员编号
        {
			if(!strcmp(buxiekafromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,buxiekatoelecpeizhi[7].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,buxiekatoelecpeizhi[8].length,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,buxiekatoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,buxiekatoelecpeizhi[8].length,"");
		}*/
        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,buxiekatoelecpeizhi[9].length,buxiekafromyinhangjiexi[13].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[10].length,buxiekafromyinhangjiexi[14].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[11].length,buxiekafromyinhangjiexi[15].value);

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,buxiekatoelecpeizhi[12].length,buxiekafromyinhangjiexi[16].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[13].length,buxiekafromyinhangjiexi[17].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[14].length,buxiekafromyinhangjiexi[18].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[15].length,buxiekafromyinhangjiexi[21].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[16].length,buxiekafromyinhangjiexi[19].value);

        pos+=insertelec(send,pos,1,"|");
        pos+=insertelec(send,pos,buxiekatoelecpeizhi[17].length,buxiekafromyinhangjiexi[20].value);
		pos+=insertelec(send,pos,1,"|");

        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
		
		
		
		for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(!strcmp(buxiekafromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
			{
				 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
				{
					if(!strcmp(buxiekafromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
					{
						Md5_mac(temp,dest16,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
						break;
					}
				}
				break;
			}
		}
       /* for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(buxiekafromyinhangjiexi[5].value,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dkdqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}*/
        tmp+=insertelec(send,tmp,buxiekatoelecpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,buxiekatoelecpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d加入包长转码之前----------\n[%d][%s]\n",getpid(),strlen(send),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);

    }
    else if(n==9) //缴费票据查询
    {
		pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[0].length,"1015");//系统编号
        tmp=pos;
        pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[1].length,"0000000000000000"); //MAC
        pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[2].length,"00000");//包长
        pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[3].length,"200009");//交易码
        pos+=insertelec(send,pos,1,"|");
		DeleteTailSpace(jiaofeipiaojuchaxunfromyinhangjiexi[14].value,putongorkabiaoforpiaoju);
		if(!strcmp(putongorkabiaoforpiaoju,"0")) //普通缴费
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
							break;
						}
					}
					if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
					{
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
					}
					break;
				}
			}
			if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
			{
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
			}
		}
		else if(!strcmp(putongorkabiaoforpiaoju,"1")) //卡表购电
		{
			 for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
							break;
						}
					}
					if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
					{
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
					}
					break;
				}
			}
			if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
			{
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
			}
		}
		else
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].qudaoshangbianhao);
							break;
						}
					}
					if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
					{
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
					}
					break;
				}
			}
			if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
			{
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
			}
		}
		
       
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",jiaofeipiaojuchaxunfromyinhangjiexi[3].value,jiaofeipiaojuchaxunfromyinhangjiexi[4].value);
        pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[5].length,date_and_time);//交易时间

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[6].length,jiaofeipiaojuchaxunfromyinhangjiexi[2].value);//交易流水号

        pos+=insertelec(send,pos,1,"|");
		
		if(!strcmp(putongorkabiaoforpiaoju,"0")) //普通缴费
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
							pos+=insertelec(send,pos,1,"|");
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
							break;
						}
					}
					if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
					{
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,"");
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,"");
					}
					break;
				}
			}
			if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
			{
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,"");
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,"");
			}
		}
		else if(!strcmp(putongorkabiaoforpiaoju,"1")) //卡表购电
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
							pos+=insertelec(send,pos,1,"|");
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
							break;
						}
					}
					if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
					{
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,"");
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,"");
					}
					break;
				}
			}
			if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
			{
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,"");
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,"");
			}
		}
		else
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].wangdiandaima);
							pos+=insertelec(send,pos,1,"|");
							pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].caozuoyuanbianhao);//caozuoyuan bianhao
							break;
						}
					}
					if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
					{
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,"");
						pos+=insertelec(send,pos,1,"|");
						pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,"");
					}
					break;
				}
			}
			if(j==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
			{
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[7].length,"");
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[8].length,"");
			}
		}
		
       
        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,jiaofeipiaojuchaxuntoelecpeizhi[9].length,jiaofeipiaojuchaxunfromyinhangjiexi[13].value);

        pos+=insertelec(send,pos,1,"|");


        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
		if(!strcmp(putongorkabiaoforpiaoju,"0")) //普通缴费
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							Md5_mac(temp,dest16,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
							break;
						}
					}
					break;
				}
			}
		}
		else if(!strcmp(putongorkabiaoforpiaoju,"1")) //卡表购电
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					 for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							Md5_mac(temp,dest16,DkJyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
							break;
						}
					}
					break;
				}
			}
		}
		else
		{
			for(j=0;j<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;j++)
			{
				if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[6].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].jyqdbm))
				{
					for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
					{
						if(!strcmp(jiaofeipiaojuchaxunfromyinhangjiexi[5].value,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].diqudaima))
						{
							Md5_mac(temp,dest16,JyqdbmDqdmQdsbhWddmCzybhMyYingshe[j].dqdmqdsbhwddmczybhmyyingshe[i].miyao);
							break;
						}
					}
					break;
				}
			}
		}
        
        tmp+=insertelec(send,tmp,jiaofeipiaojuchaxuntoelecpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,jiaofeipiaojuchaxuntoelecpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d加入包长转码之前----------\n[%d][%s]\n",getpid(),strlen(send),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);

	}
	else if(n==10) //票据打印信息查询
	{
	/*char jine[12];
    char temp[BUFFER_SIZE];
    char dest16[17];
    int pos=0,tmp;
    int i,j,k;
    char baochang[6];
    int baochangint;
    char date_and_time[14]; //shijian hecheng  14wei
    int inlen,outlen;
    char sendbuf[BUFFER_SIZE];
    memset(date_and_time,'\0',14);
    memset(baochang,'\0',6);
	memset(temp,'\0',BUFFER_SIZE);
	memset(dest16,'\0',17);
   */

		pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[0].length,"1015");//系统编号
        tmp=pos;
        pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[1].length,"0000000000000000"); //MAC
        pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[2].length,"00000");//包长
        pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[3].length,"200010");//交易码
        pos+=insertelec(send,pos,1,"|");
        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //渠道商编号
        {
			if(!strcmp(piaojudayinxinxichaxunfromyinhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[4].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].qudaoshangbianhao);
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[4].length,"");//qudao shang bianhao
		}
        pos+=insertelec(send,pos,1,"|");


        sprintf(date_and_time,"%s%s",piaojudayinxinxichaxunfromyinhangjiexi[3].value,piaojudayinxinxichaxunfromyinhangjiexi[4].value);
        pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[5].length,date_and_time);//交易时间

        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[6].length,piaojudayinxinxichaxunfromyinhangjiexi[2].value);//交易流水号

        pos+=insertelec(send,pos,1,"|");
        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //网点代码and操作员编号
        {
			if(!strcmp(piaojudayinxinxichaxunfromyinhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[7].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].wangdiandaima);
				pos+=insertelec(send,pos,1,"|");
				pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[8].length,dqdmqdsbhwddmczybhmyyingshepeizhi[i].caozuoyuanbianhao);//caozuoyuan bianhao
				break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[7].length,"");
			pos+=insertelec(send,pos,1,"|");
			pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[8].length,"");
		}
        pos+=insertelec(send,pos,1,"|");

        pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[9].length,piaojudayinxinxichaxunfromyinhangjiexi[13].value);

        pos+=insertelec(send,pos,1,"|");
		pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[10].length,piaojudayinxinxichaxunfromyinhangjiexi[14].value);

        pos+=insertelec(send,pos,1,"|");
		pos+=insertelec(send,pos,piaojudayinxinxichaxuntoelecpeizhi[11].length,"2");

        pos+=insertelec(send,pos,1,"|");


        for(i=25; i<strlen(send); i++)
        {
            temp[i-25]=send[i];
        }
        temp[i]='\0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp--%s\n",temp);
        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++)
        {
			if(!strcmp(piaojudayinxinxichaxunfromyinhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
				Md5_mac(temp,dest16,dqdmqdsbhwddmczybhmyyingshepeizhi[i].miyao);
				break;
			}
		}
        tmp+=insertelec(send,tmp,piaojudayinxinxichaxuntoelecpeizhi[1].length,dest16);//MAC jiaoyanma-----

        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi qian----------\n-%s\n",strlen(send),getpid(),send);

        memset(sendbuf,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dchong zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);

		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%dzhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
        memset(baochang,'\0',6);
        baochangint=strlen(sendbuf)-HEAD_LENGTH;
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochangint----------%d\n",baochangint);
        i=4;
        while(i>=0)
        {
            if(baochangint==0)
            {
                baochang[i]='0';
            }
            else
            {
                baochang[i]=baochangint%10+'0';
                baochangint=baochangint/10;
            }
            i--;
        }
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"baochang-----%s\n",baochang);
        tmp+=insertelec(send,tmp,piaojudayinxinxichaxuntoelecpeizhi[2].length,baochang);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d加入包长转码之前----------\n[%d][%s]\n",getpid(),strlen(send),send);

		// memset(send,'\0',BUFFER_SIZE);
        inlen=strlen(send);
        outlen=BUFFER_SIZE;
		code_convert(TO_CODE,FROM_CODE,send,inlen,sendbuf,outlen);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(sendbuf),getpid(),sendbuf);
		memset(send,'\0',BUFFER_SIZE);
		strcpy(send,sendbuf);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%d]%djia ru bao chang zhuan ma zhi hou----------\n-%s\n",strlen(send),getpid(),send);
	}


}


/*0----电力代收欠费查询
 *1----电力代收缴费
 *2----电力代收冲正
 *3----电力代收对账
 *4----电力签解约
 *5----广电套餐目录 and  用户查询   先做用户查询，后做套餐目录查询
 *6----广电订购套餐  
 *7----广电用户查询	暂时不用
 *8----电力读卡信息查询
 *9----电力卡表购电
 *10---电力卡表购电冲正
 *11---电力补写卡
 *12---电力缴费票据查询
 *13---电力票据打印信息查询        --------------取消不用---------
 */
void chongzufabao_to_yinhang(char *send,int n)
{
	int sum=0;
	int sum_weiyuejin=0;
	int sum_yingjiao=0;
    char tmp[12];
    int pos=0,i=0;
    int j;
    int k;
    int kt=7;
    int jilushu ;
	int realjilushu;
    int min=0,max=0;
    char jine[13];
    char shuzitobianma[10][5]= {"0001","0002","0003","0004","0005","0006","0007","0008","0009","0010"};
    char address[256];
    int leng;
	char charjilushu[5];
	char zijinbianhaojiequ[10];
	char filename[50];
	char temptaocanbianhao[3];
	int selecttaocan;
	char taocanshuliang[5];
	char taocannoandname[100];
	char taocannodeletetailspace[10];
	char taocannamedeletetailspace[70];
	char taocanjine[16];
	char elecReturnCodeAndMessage[100];
	int sort[100];
	int indexForCode;
	int IsCutChinese;
	memset(taocanshuliang,'\0',sizeof(taocanshuliang));
	memset(taocanjine,'\0',sizeof(taocanjine));
	memset(taocannamedeletetailspace,'\0',sizeof(taocannamedeletetailspace));
	memset(taocannodeletetailspace,'\0',sizeof(taocannodeletetailspace));
	memset(taocannoandname,'\0',sizeof(taocannoandname));
	memset(temptaocanbianhao,'\0',sizeof(temptaocanbianhao));
	memset(elecReturnCodeAndMessage,'\0',sizeof(elecReturnCodeAndMessage));
	memset(filename,'\0',50);
	memset(charjilushu,'\0',5);
    memset(address,'\0',256);
	memset(zijinbianhaojiequ,'\0',10);
	memset(send,'\0',sizeof(send));
	int returncode;  // 1--成功 0--失败
    if(n==0)
    {
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"1900");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeijiexi[2].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        if(strcmp(qianfeifanhuijiexi[3].value,"0000")==0)
        {
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"99  ");//00 99
		}
        pos+=qianfeitoyinhangpeizhi[i++].length;
		
		strcat(elecReturnCodeAndMessage,qianfeifanhuijiexi[3].value);
		strcat(elecReturnCodeAndMessage,qianfeifanhuijiexi[4].value);		
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeijiexi[5].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[7].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[8].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        strcat(address,qianfeifanhuijiexi[9].value);
        strcat(address,qianfeifanhuijiexi[10].value);
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,address);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"095598");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"电力");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"000");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"电力");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"0000");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"全部");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        jilushu = stringtoint(qianfeifanhuijiexi[13].value,0,strlen(qianfeifanhuijiexi[13].value)-1);
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jilushu   %d\n",jilushu);
        min=0,max=0;
        for(j=0; j<jilushu; j++)
        {
            if(strcmp(qianfeifanhuijiexijilu[min].value[1],qianfeifanhuijiexijilu[j].value[1])>0)
            {
                min=j;
            }
            if(strcmp(qianfeifanhuijiexijilu[max].value[1],qianfeifanhuijiexijilu[j].value[1])<0)
            {
                max=j;
            }
            sum_yingjiao+=stringtoint(qianfeifanhuijiexijilu[j].value[2],0,strlen(qianfeifanhuijiexijilu[j].value[2])-1);
            sum_weiyuejin+=stringtoint(qianfeifanhuijiexijilu[j].value[3],0,strlen(qianfeifanhuijiexijilu[j].value[3])-1);

			// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d----%d---\n",sum_yingjiao,sum_weiyuejin);
		}
        inttostring(sum_yingjiao,jine);
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine---%s\n",jine);
        fentoyuan(jine);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine1--%s\n",jine);
        insertjine(send,pos,qianfeitoyinhangpeizhi[i].length-strlen(jine),"");
        pos+=qianfeitoyinhangpeizhi[i].length-strlen(jine);
        insert(send,pos,strlen(jine),jine);
        pos+=strlen(jine);
        i++;
        inttostring(sum_weiyuejin,jine);
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine---%s\n",jine);
        fentoyuan(jine);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine2--%s\n",jine);
        insertjine(send,pos,qianfeitoyinhangpeizhi[i].length-strlen(jine),"");
        pos+=qianfeitoyinhangpeizhi[i].length-strlen(jine);
        insert(send,pos,strlen(jine),jine);
        pos+=strlen(jine);
        i++;
        leng=strlen(qianfeifanhuijiexi[11].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=qianfeifanhuijiexi[11].value[k];
		}
		//这里记录下应缴总金额
		jinea=stringtoint(tmp,0,leng-1);
		fentoyuan(tmp);





		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine3--%s\n",tmp);
		insertjine(send,pos,qianfeitoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=qianfeitoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"send====%s\n",send);
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexijilu[min].value[1]);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexijilu[max].value[1]);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[13].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;





        for(j=0; j<jilushu; j++)
        {
            insert(send,pos,4,shuzitobianma[j]);
            pos+=4;
            insert(send,pos,30,qianfeifanhuijiexijilu[j].value[0]);
            pos+=30;
            //int k;

			sum=stringtoint(qianfeifanhuijiexijilu[j].value[2],0,strlen(qianfeifanhuijiexijilu[j].value[2])-1)+stringtoint(qianfeifanhuijiexijilu[j].value[3],0,strlen(qianfeifanhuijiexijilu[j].value[3])-1);
			inttostring(sum,jine);
			fentoyuan(jine);
			insertjine(send,pos,12-strlen(jine),"");
			pos+=12-strlen(jine);
            insert(send,pos,strlen(jine),jine);
            pos+=strlen(jine);
        }
        if(j<10)
        {
            insert(send,pos,(9-j+1)*46," ");
            pos+=(9-j+1)*46;
        }
        i++;


        //insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        //pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;

    }



    else if(n==1)
    {
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"2424");
        pos+=jiaofeitoyinhangpeizhi[i++].length;

        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,jiaofeijiexi[2].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        if(strcmp(jiaofeifanhuijiexi[3].value,"0000")==0)
        {
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"99  ");//00 99
		}
        pos+=jiaofeitoyinhangpeizhi[i++].length;
		strcat(elecReturnCodeAndMessage,jiaofeifanhuijiexi[3].value);
		strcat(elecReturnCodeAndMessage,jiaofeifanhuijiexi[4].value);		
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
        //insert(send,pos,jiaofeitoyinhangpeizhi[i].length,jiaofeifanhuijiexi[4].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,jiaofeijiexi[5].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;

        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,jiaofeijiexi[13].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[8].value);//00 99
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        strcat(address,qianfeifanhuijiexi[9].value);
        strcat(address,qianfeifanhuijiexi[10].value);
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,address);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"095598");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"电力");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"000");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"电力");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"0000");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"全部");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"1   ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;


		jilushu = stringtoint(qianfeifanhuijiexi[13].value,0,strlen(qianfeifanhuijiexi[13].value)-1);
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jilushu   %d\n",jilushu);
        min=0,max=0;
        for(j=0; j<jilushu; j++)
        {
            if(strcmp(qianfeifanhuijiexijilu[min].value[1],qianfeifanhuijiexijilu[j].value[1])>0)
            {
                min=j;
            }
            if(strcmp(qianfeifanhuijiexijilu[max].value[1],qianfeifanhuijiexijilu[j].value[1])<0)
            {
                max=j;
            }
            sum_yingjiao+=stringtoint(qianfeifanhuijiexijilu[j].value[2],0,strlen(qianfeifanhuijiexijilu[j].value[2])-1);
            sum_weiyuejin+=stringtoint(qianfeifanhuijiexijilu[j].value[3],0,strlen(qianfeifanhuijiexijilu[j].value[3])-1);
        }
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d----%d---\n",sum_yingjiao,sum_weiyuejin);
        inttostring(sum_yingjiao,jine);
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine---%s\n",jine);
        fentoyuan(jine);
        insertjine(send,pos,jiaofeitoyinhangpeizhi[i].length-strlen(jine),"");
        pos+=jiaofeitoyinhangpeizhi[i].length-strlen(jine);
        insert(send,pos,strlen(jine),jine);
        pos+=strlen(jine);
        i++;

        inttostring(sum_weiyuejin,jine);
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine---%s\n",jine);
        fentoyuan(jine);
        insertjine(send,pos,jiaofeitoyinhangpeizhi[i].length-strlen(jine),"");
        pos+=jiaofeitoyinhangpeizhi[i].length-strlen(jine);
        insert(send,pos,strlen(jine),jine);
        pos+=strlen(jine);
        i++;


		leng=strlen(qianfeifanhuijiexi[11].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=qianfeifanhuijiexi[11].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,jiaofeitoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=jiaofeitoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;





        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,qianfeifanhuijiexijilu[min].value[1]);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,qianfeifanhuijiexijilu[max].value[1]);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[13].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;



        for(j=0; j<jilushu; j++)
        {
            insert(send,pos,4,shuzitobianma[j]);
            pos+=4;
            insert(send,pos,40,qianfeifanhuijiexijilu[j].value[0]);
            pos+=40;
            //int k;


            insert(send,pos,20," ");
            pos+=20;
            sum=stringtoint(qianfeifanhuijiexijilu[j].value[2],0,strlen(qianfeifanhuijiexijilu[j].value[2])-1)+stringtoint(qianfeifanhuijiexijilu[j].value[3],0,strlen(qianfeifanhuijiexijilu[j].value[3])-1);
			inttostring(sum,jine);
			fentoyuan(jine);
			insertjine(send,pos,12-strlen(jine),"");
			pos+=12-strlen(jine);
            insert(send,pos,strlen(jine),jine);
            pos+=strlen(jine);

        }
        if(j<10)
        {
            insert(send,pos,(9-j+1)*76," ");
            pos+=(9-j+1)*76;
        }
        i++;
        //insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        //pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[6].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;

		strncpy(zijinbianhaojiequ,qianfeifanhuijiexi[6].value,5);
		for(j=0;j<ZIJIN_ZIDUAN_REAL_SHULIANG;j++)
		{
			if(strcmp(zijinbianhaojiequ,zijinyingshepeizhi[j].org_no)==0)
			{
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,zijinyingshepeizhi[j].zhanghao);
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,zijinyingshepeizhi[j].kaihuhang);
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				break;
			}
		}
		if(j==ZIJIN_ZIDUAN_REAL_SHULIANG)
		{
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");
			pos+=jiaofeitoyinhangpeizhi[i++].length;
		}

        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[5].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
		memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=qianfeifanhuijiexi[12].value[k];
		}
		//这里记录余额
		jinec=0;//stringtoint(tmp,0,leng-1);
		fentoyuan(tmp);


		insertjine(send,pos,jiaofeitoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=jiaofeitoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;

        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
    }
    else if(n==2)
    {
        insert(send,pos,chexiaotoyinhangpeizhi[i].length,"0224");
        pos+=chexiaotoyinhangpeizhi[i++].length;
        insert(send,pos,chexiaotoyinhangpeizhi[i].length,chexiaojiexi[2].value);
        pos+=chexiaotoyinhangpeizhi[i++].length;
		if(strcmp(chexiaofanhuijiexi[3].value,"0000")==0)
        {
			insert(send,pos,chexiaotoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,chexiaotoyinhangpeizhi[i].length,"99  ");//00 99
		}
        pos+=chexiaotoyinhangpeizhi[i++].length;
		
		strcat(elecReturnCodeAndMessage,chexiaofanhuijiexi[3].value);
		strcat(elecReturnCodeAndMessage,chexiaofanhuijiexi[4].value);		
        insert(send,pos,chexiaotoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
		
        //insert(send,pos,chexiaotoyinhangpeizhi[i].length,chexiaofanhuijiexi[4].value);
        pos+=chexiaotoyinhangpeizhi[i++].length;
        insert(send,pos,chexiaotoyinhangpeizhi[i].length,chexiaojiexi[5].value);
        pos+=chexiaotoyinhangpeizhi[i++].length;
        insert(send,pos,chexiaotoyinhangpeizhi[i].length," ");
        pos+=chexiaotoyinhangpeizhi[i++].length;
        insert(send,pos,chexiaotoyinhangpeizhi[i].length," ");
        pos+=chexiaotoyinhangpeizhi[i++].length;
        insert(send,pos,chexiaotoyinhangpeizhi[i].length," ");
        pos+=chexiaotoyinhangpeizhi[i++].length;
    }
    else if(n==3)
    {
        insert(send,pos,duizhangtoyinhangpeizhi[i].length,"0224");
        pos+=duizhangtoyinhangpeizhi[i++].length;
        insert(send,pos,duizhangtoyinhangpeizhi[i].length,duizhangjiexi[2].value);
        pos+=duizhangtoyinhangpeizhi[i++].length;
        if(strcmp(duizhangfanhuijiexi[3].value,"0000")==0)
        {
			insert(send,pos,duizhangtoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,duizhangtoyinhangpeizhi[i].length,"99  ");//00 99
		}
        pos+=duizhangtoyinhangpeizhi[i++].length;
		strcat(elecReturnCodeAndMessage,duizhangfanhuijiexi[3].value);
		strcat(elecReturnCodeAndMessage,duizhangfanhuijiexi[4].value);		
        insert(send,pos,duizhangtoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
       // insert(send,pos,duizhangtoyinhangpeizhi[i].length,duizhangfanhuijiexi[4].value);
        pos+=duizhangtoyinhangpeizhi[i++].length;
        insert(send,pos,duizhangtoyinhangpeizhi[i].length,duizhangjiexi[5].value);
        pos+=duizhangtoyinhangpeizhi[i++].length;
        insert(send,pos,duizhangtoyinhangpeizhi[i].length," ");
        pos+=duizhangtoyinhangpeizhi[i++].length;
        insert(send,pos,duizhangtoyinhangpeizhi[i].length," ");
        pos+=duizhangtoyinhangpeizhi[i++].length;
        insert(send,pos,duizhangtoyinhangpeizhi[i].length," ");
        pos+=duizhangtoyinhangpeizhi[i++].length;
    }
    else if(n==4)
    {
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,"0620");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,daikoubankqianjieyuejiexi[2].value);
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        if(strcmp(daikoubankqianjieyuefanhuijiexi[3].value,"0000")==0)
        {
			insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,"99  ");//00 99
		}
        //insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[3].value);//00 99
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[4].value);
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,daikoubankqianjieyuejiexi[5].value);
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length," ");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length," ");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length," ");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;

        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[5].value);
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length," ");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length," ");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length," ");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
        insert(send,pos,daikoubankqianjieyuetoyinhangpeizhi[i].length," ");
        pos+=daikoubankqianjieyuetoyinhangpeizhi[i++].length;
    }
	//重组广电的包
	//重组广电的包
	else if(n==5)  //套餐目录
	{
		insert(send,pos,qianfeitoyinhangpeizhi[i].length,"1900");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeijiexi[2].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        
        temptaocanbianhao[0]=qianfeijiexi[14].value[2];
        temptaocanbianhao[1]=qianfeijiexi[14].value[3];  //提取银行套餐选项编号字段中的后两位  00-查询全部  01-10查询某一个
       
			//printf("%s\n%s\n",guangdiansendtaocanmulujiexi[3].value,guangdiansendyonghuchaxunjiexi[3].value);
			if((strcmp(guangdiansendtaocanmulujiexi[3].value,"000")==0)&&(strcmp(guangdiansendyonghuchaxunjiexi[3].value,"000")==0))
			{
				 if(strcmp(temptaocanbianhao,"00")!=0)   //----------------查询某一个
				{					
					if((strcmp(guangdiansenddinggoutaocanjiexi[3].value,"000")==0))
					{
						insert(send,pos,qianfeitoyinhangpeizhi[i].length,"00  ");//00 99
						pos+=qianfeitoyinhangpeizhi[i++].length;
						insert(send,pos,qianfeitoyinhangpeizhi[i].length,"交易成功");
						pos+=qianfeitoyinhangpeizhi[i++].length;
						returncode=1;
					}
					else
					{
						insert(send,pos,qianfeitoyinhangpeizhi[i].length,"99  ");//00 99
						pos+=qianfeitoyinhangpeizhi[i++].length;
						for(j=0;j<GUANGDIAN_ERROR_CODE_MESSAGE_SHULIANG;j++)
						{
							if(strcmp(guangdiansenddinggoutaocanjiexi[3].value,GDErrorCodeMessage[j].code)==0)
							{
								insert(send,pos,qianfeitoyinhangpeizhi[i].length,GDErrorCodeMessage[j].message);
								pos+=qianfeitoyinhangpeizhi[i++].length;
								returncode=0;
								break;
							}
						}
						if(j==GUANGDIAN_ERROR_CODE_MESSAGE_SHULIANG)
						{
							insert(send,pos,qianfeitoyinhangpeizhi[i].length,"用户不允许购买该套餐！请选择其他套餐！");
							pos+=qianfeitoyinhangpeizhi[i++].length;
							returncode=0;
						}
						
					}
				}
				else
				{
					insert(send,pos,qianfeitoyinhangpeizhi[i].length,"00  ");//00 99
					pos+=qianfeitoyinhangpeizhi[i++].length;
					insert(send,pos,qianfeitoyinhangpeizhi[i].length,"交易成功");
					pos+=qianfeitoyinhangpeizhi[i++].length;
					returncode=1;
				}
			}
			else if(strcmp(guangdiansendyonghuchaxunjiexi[3].value,"000")!=0)
			{
				insert(send,pos,qianfeitoyinhangpeizhi[i].length,"99  ");//00 99
				pos+=qianfeitoyinhangpeizhi[i++].length;
				for(j=0;j<GUANGDIAN_ERROR_CODE_MESSAGE_SHULIANG;j++)
				{
					if(strcmp(guangdiansendyonghuchaxunjiexi[3].value,GDErrorCodeMessage[j].code)==0)
					{
						insert(send,pos,qianfeitoyinhangpeizhi[i].length,GDErrorCodeMessage[j].message);
						pos+=qianfeitoyinhangpeizhi[i++].length;
						returncode=0;
						break;
					}
				}
				if(j==GUANGDIAN_ERROR_CODE_MESSAGE_SHULIANG)
				{
					insert(send,pos,qianfeitoyinhangpeizhi[i].length,guangdiansendyonghuchaxunjiexi[4].value);
					pos+=qianfeitoyinhangpeizhi[i++].length;
					returncode=0;
				}
			}
			else if (strcmp(guangdiansendtaocanmulujiexi[3].value,"000")!=0)
			{
				insert(send,pos,qianfeitoyinhangpeizhi[i].length,"99  ");//00 99
				pos+=qianfeitoyinhangpeizhi[i++].length;
				for(j=0;j<GUANGDIAN_ERROR_CODE_MESSAGE_SHULIANG;j++)
				{
					if(strcmp(guangdiansendtaocanmulujiexi[3].value,GDErrorCodeMessage[j].code)==0)
					{
						insert(send,pos,qianfeitoyinhangpeizhi[i].length,GDErrorCodeMessage[j].message);
						pos+=qianfeitoyinhangpeizhi[i++].length;
						returncode=0;
						break;
					}
				}
				if(j==GUANGDIAN_ERROR_CODE_MESSAGE_SHULIANG)
				{
					insert(send,pos,qianfeitoyinhangpeizhi[i].length,guangdiansendtaocanmulujiexi[4].value);
					pos+=qianfeitoyinhangpeizhi[i++].length;
					returncode=0;
				}
			}
		if(returncode == 1)
		{
			 insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeijiexi[5].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeijiexi[13].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,guangdiansendyonghuchaxunjiexi[6].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;

        insert(send,pos,qianfeitoyinhangpeizhi[i].length,guangdiansendyonghuchaxunjiexi[4].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;

        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"096766");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"陕西广电网络");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        if(!strcmp(guangdiansendyonghuchaxunjiexi[5].value,"A"))
        {
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"00");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"陕西省公司（除宝鸡）用户");     //二级机构名称
			pos+=qianfeitoyinhangpeizhi[i++].length;
		}
		else if(!strcmp(guangdiansendyonghuchaxunjiexi[5].value,"B"))
        {
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"01");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"宝鸡子公司用户");     //二级机构名称
			pos+=qianfeitoyinhangpeizhi[i++].length;
		}
		else
		{
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");     //二级机构名称
			pos+=qianfeitoyinhangpeizhi[i++].length;
		}
		insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");    //缴费项目编号
		pos+=qianfeitoyinhangpeizhi[i++].length;


        temptaocanbianhao[0]=qianfeijiexi[14].value[2];
        temptaocanbianhao[1]=qianfeijiexi[14].value[3];  //提取银行套餐选项编号字段中的后两位  00-查询全部  01-10查询某一个
        if(strcmp(temptaocanbianhao,"00")==0)   //----------------查询全部
        {
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"请按明细选择套餐");  //缴费项目名称
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");  //费用总额
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");  //优惠
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");  //应缴金额
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");  //费用起始日期
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");  //费用终止日期
			pos+=qianfeitoyinhangpeizhi[i++].length;

			jilushu=stringtoint(guangdiansendtaocanmulujiexi[4].value,0,3);
			inttostring(jilushu,taocanshuliang);

			insert(send,pos,qianfeitoyinhangpeizhi[i].length,taocanshuliang);   //账单数量
			pos+=qianfeitoyinhangpeizhi[i++].length;

			
			SortTaoCanNo(sort);
			jilushu = jilushu>=10?10:jilushu;
			for(j=0; j<jilushu; j++)
			{
				insert(send,pos,6,guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[0]);
				pos+=6;
				memset(taocannamedeletetailspace,'\0',sizeof(taocannamedeletetailspace));
				DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[1],taocannamedeletetailspace);//去除套餐名称尾部空格
				taocannamedeletetailspace[32]='\0';
	            IsCutChinese = 0;
	            for(indexForCode = 0; indexForCode <= 31;)
	            {
					if(taocannamedeletetailspace[indexForCode]&0x80)//防止从一个汉字的中间截断，具体参考汉字编码规则
					{
						if(indexForCode + 1 == 32)
						{
							IsCutChinese = 1;
							break;
						}
						if(taocannamedeletetailspace[indexForCode + 1]&0x80)
						{
							indexForCode += 2;
						}
						else
						{
							indexForCode++;
						}											
					}
					else
					{
						indexForCode++;
					}					
				}
				if(IsCutChinese == 1)//防止从一个汉字的中间截断，具体参考汉字编码规则
				{
					taocannamedeletetailspace[31] = ' ';					
				}
				insert(send,pos,32,taocannamedeletetailspace);
				pos+=32;
				//printf("[No]%s\n",guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[0]);
				//printf("[name1]%s\n",guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[1]);
				//printf("[name2]%s\n",taocannamedeletetailspace);
				
				

				//截取套餐金额的后8位，并将前导空格转换为前导0
				memset(taocanjine,'\0',sizeof(taocanjine));
				strncpy(taocanjine,guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[2]+strlen(guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[2])-8,8);
				for(k=0;k<8;k++)
				{
					if(taocanjine[k]==' ')
					{
						taocanjine[k]='0';
					}
				}

				insert(send,pos,8,taocanjine);
				pos+=8;
				//printf("[money1]%s\n",taocanjine);
				//printf("[money2]%s\n",taocanjine);
				//printf("[send]%s\n",send);
			}
			if(j<10)
			{
				insert(send,pos,(9-j+1)*46," ");
				pos+=(9-j+1)*46;
			}
			i++;
		}
		else 									//----------------查询某一个
		{
			selecttaocan=SelectTaoCanNo(temptaocanbianhao);
			if(selecttaocan!=-1)
			{
				DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[0],taocannodeletetailspace);//去除套餐编号尾部空格
				DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[1],taocannamedeletetailspace);//去除套餐名称尾部空格
				strcat(taocannoandname,taocannodeletetailspace);
				strcat(taocannoandname,taocannamedeletetailspace);
				taocannoandname[qianfeitoyinhangpeizhi[i].length]='\0';        //套餐编号+套餐名称 ，多余的截取
				insert(send,pos,qianfeitoyinhangpeizhi[i].length,taocannoandname);   //缴费项目名称
				pos+=qianfeitoyinhangpeizhi[i++].length;

				//截取套餐金额的后12位，并将前导空格转换为前导0
				strncpy(taocanjine,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2]+strlen(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2])-12,12);
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"套餐金额--[%s]",taocanjine);
				for(j=0;j<12;j++)
				{
					if(taocanjine[j]==' ')
					{
						taocanjine[j]='0';
					}
				}
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"套餐金额--[%s]",taocanjine);
				insert(send,pos,qianfeitoyinhangpeizhi[i].length,taocanjine);   //费用总额
				pos+=qianfeitoyinhangpeizhi[i++].length;

				insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");   //优惠
				pos+=qianfeitoyinhangpeizhi[i++].length;

				insert(send,pos,qianfeitoyinhangpeizhi[i].length,taocanjine);   //应缴总额
				pos+=qianfeitoyinhangpeizhi[i++].length;

				insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");  //费用起始日期
				pos+=qianfeitoyinhangpeizhi[i++].length;
				insert(send,pos,qianfeitoyinhangpeizhi[i].length,"");  //费用终止日期
				pos+=qianfeitoyinhangpeizhi[i++].length;


				jilushu=1;
				inttostring(jilushu,taocanshuliang);

				insert(send,pos,qianfeitoyinhangpeizhi[i].length,taocanshuliang);   //账单数量
				pos+=qianfeitoyinhangpeizhi[i++].length;

				for(j=0; j<jilushu; j++)
				{
					insert(send,pos,6,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[0]);
					pos+=6;
					memset(taocannamedeletetailspace,'\0',sizeof(taocannamedeletetailspace));
					DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[1],taocannamedeletetailspace);//去除套餐名称尾部空格
					taocannamedeletetailspace[32]='\0';
					 IsCutChinese = 0;
	            for(indexForCode = 0; indexForCode <= 31;)
	            {
					if(taocannamedeletetailspace[indexForCode]&0x80)//防止从一个汉字的中间截断，具体参考汉字编码规则
					{
						if(indexForCode + 1 == 32)
						{
							IsCutChinese = 1;
							break;
						}
						if(taocannamedeletetailspace[indexForCode + 1]&0x80)
						{
							indexForCode += 2;
						}
						else
						{
							indexForCode++;
						}											
					}
					else
					{
						indexForCode++;
					}					
				}
				if(IsCutChinese == 1)//防止从一个汉字的中间截断，具体参考汉字编码规则
				{
					taocannamedeletetailspace[31] = ' ';					
				}
					insert(send,pos,32,taocannamedeletetailspace);
					pos+=32;


					//截取套餐金额的后8位，并将前导空格转换为前导0
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s\n",guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2]);
					memset(taocanjine,'\0',sizeof(taocanjine));
					strncpy(taocanjine,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2]+5,8);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%drow]套餐金额[%s]\n",__LINE__,taocanjine);
					for(k=0;k<8;k++)
					{
						if(taocanjine[k]==' ')
						{
							taocanjine[k]='0';
						}
					}

					insert(send,pos,8,taocanjine);
					pos+=8;
				}
				if(j<10)
				{
					insert(send,pos,(9-j+1)*46," ");
					pos+=(9-j+1)*46;
				}
				i++;
			}
			else
			{
				insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
				pos+=qianfeitoyinhangpeizhi[i++].length;
				insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
				pos+=qianfeitoyinhangpeizhi[i++].length;
				insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
				pos+=qianfeitoyinhangpeizhi[i++].length;
				insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
				pos+=qianfeitoyinhangpeizhi[i++].length;
				insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
				pos+=qianfeitoyinhangpeizhi[i++].length;
				insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
				pos+=qianfeitoyinhangpeizhi[i++].length;

				jilushu=0;
				inttostring(jilushu,taocanshuliang);

				insert(send,pos,qianfeitoyinhangpeizhi[i].length,taocanshuliang);   //账单数量
				pos+=qianfeitoyinhangpeizhi[i++].length;
				insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
				pos+=qianfeitoyinhangpeizhi[i++].length;
			}

		}

        insert(send,pos,qianfeitoyinhangpeizhi[i].length,guangdiansendyonghuchaxunjiexi[7].value);   //保留1
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
		}
		else
		{
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
			insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
			pos+=qianfeitoyinhangpeizhi[i++].length;
		}
			
       
	}
	
	
	
	
	
	else if(n==6)  //订购套餐
	{
		insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"2424");
        pos+=jiaofeitoyinhangpeizhi[i++].length;

        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,jiaofeijiexi[2].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        if(strcmp(guangdiansenddinggoutaocanjiexi[5].value,"0")==0)
        {
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"00  ");//00 99
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"成功");
		}
		else
		{
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"99  ");//00 99
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"失败");
		}
        
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,jiaofeijiexi[5].value);
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;


        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,jiaofeijiexi[13].value); //缴费号
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,guangdiansendyonghuchaxunjiexi[6].value); //业主姓名
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        
        
        
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,guangdiansendyonghuchaxunjiexi[4].value); //缴费单元名称
        pos+=jiaofeitoyinhangpeizhi[i++].length;

        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"096766"); //一级机构编码
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"陕西广电网络"); //一级机构名称
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        if(!strcmp(guangdiansendyonghuchaxunjiexi[5].value,"A"))
        {
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"00");
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"陕西省公司（除宝鸡）用户");     //二级机构名称
			pos+=jiaofeitoyinhangpeizhi[i++].length;
		}
		else if(!strcmp(guangdiansendyonghuchaxunjiexi[5].value,"B"))
        {
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"01");
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"宝鸡子公司用户");     //二级机构名称
			pos+=jiaofeitoyinhangpeizhi[i++].length;
		}
		else
		{
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");     //二级机构名称
			pos+=jiaofeitoyinhangpeizhi[i++].length;
		}
		insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");    //缴费项目编号
		pos+=jiaofeitoyinhangpeizhi[i++].length;


        temptaocanbianhao[0]=qianfeijiexi[14].value[2];
        temptaocanbianhao[1]=qianfeijiexi[14].value[3];  //提取银行套餐选项编号字段中的后两位  00-查询全部  01-10查询某一个
        if(strcmp(temptaocanbianhao,"00")==0)   //----------------查询全部
        {
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"请按明细选择套餐");  //缴费项目名称
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"1   ");
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");  //费用总额
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");  //优惠
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");  //应缴金额
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");  //费用起始日期
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");  //费用终止日期
			pos+=jiaofeitoyinhangpeizhi[i++].length;

			jilushu=stringtoint(guangdiansendtaocanmulujiexi[4].value,0,3);
			inttostring(jilushu,taocanshuliang);

			insert(send,pos,jiaofeitoyinhangpeizhi[i].length,taocanshuliang);   //账单数量
			pos+=jiaofeitoyinhangpeizhi[i++].length;
			SortTaoCanNo(sort);
			jilushu = jilushu>=10?10:jilushu;
			for(j=0; j<jilushu; j++)
			{
				insert(send,pos,6,guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[0]);
				pos+=6;
				memset(taocannamedeletetailspace,'\0',sizeof(taocannamedeletetailspace));
				DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[1],taocannamedeletetailspace);//去除套餐名称尾部空格
				taocannamedeletetailspace[62]='\0';
				insert(send,pos,62,taocannamedeletetailspace);
				pos+=62;


				//截取套餐金额的后8位，并将前导空格转换为前导0
				memset(taocanjine,'\0',sizeof(taocanjine));
				strncpy(taocanjine,guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[2]+strlen(guangdiansendtaocanmulujiexitaocanmingxi[sort[j]].value[2])-8,8);
				for(k=0;k<8;k++)
				{
					if(taocanjine[k]==' ')
					{
						taocanjine[k]='0';
					}
				}

				insert(send,pos,8,taocanjine);
				pos+=8;
			}
			if(j<10)
			{
				insert(send,pos,(9-j+1)*76," ");
				pos+=(9-j+1)*76;
			}
			i++;
			
			insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");   //保留1
        pos+=jiaofeitoyinhangpeizhi[i++].length;
		}
		else 									//----------------查询某一个
		{
			selecttaocan=SelectTaoCanNo(temptaocanbianhao);
			if(selecttaocan!=-1)
			{
				DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[0],taocannodeletetailspace);//去除套餐编号尾部空格
				DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[1],taocannamedeletetailspace);//去除套餐名称尾部空格
				strcat(taocannoandname,taocannodeletetailspace);
				strcat(taocannoandname,taocannamedeletetailspace);
				taocannoandname[jiaofeitoyinhangpeizhi[i].length]='\0';        //套餐编号+套餐名称 ，多余的截取
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,taocannoandname);   //缴费项目名称
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"1   ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;

				//截取套餐金额的后12位，并将前导空格转换为前导0
				strncpy(taocanjine,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2]+strlen(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2])-12,12);
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"套餐金额--[%s]",taocanjine);
				for(j=0;j<12;j++)
				{
					if(taocanjine[j]==' ')
					{
						taocanjine[j]='0';
					}
				}
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"套餐金额--[%s]",taocanjine);
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,taocanjine);   //费用总额
				pos+=jiaofeitoyinhangpeizhi[i++].length;

				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");   //优惠
				pos+=jiaofeitoyinhangpeizhi[i++].length;

				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,taocanjine);   //应缴总额
				pos+=jiaofeitoyinhangpeizhi[i++].length;

				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");  //费用起始日期
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");  //费用终止日期
				pos+=jiaofeitoyinhangpeizhi[i++].length;


				jilushu=1;
				inttostring(jilushu,taocanshuliang);

				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,taocanshuliang);   //账单数量
				pos+=jiaofeitoyinhangpeizhi[i++].length;

				for(j=0; j<jilushu; j++)
				{
					insert(send,pos,6,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[0]);
					pos+=6;
					memset(taocannamedeletetailspace,'\0',sizeof(taocannamedeletetailspace));
					DeleteTailSpace(guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[1],taocannamedeletetailspace);//去除套餐名称尾部空格
					taocannamedeletetailspace[62]='\0';
					insert(send,pos,62,taocannamedeletetailspace);
					pos+=62;


					//截取套餐金额的后8位，并将前导空格转换为前导0
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s\n",guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2]);
					memset(taocanjine,'\0',sizeof(taocanjine));
					strncpy(taocanjine,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[2]+5,8);
					//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[%drow]套餐金额[%s]\n",__LINE__,taocanjine);
					for(k=0;k<8;k++)
					{
						if(taocanjine[k]==' ')
						{
							taocanjine[k]='0';
						}
					}

					insert(send,pos,8,taocanjine);
					pos+=8;
				}
				if(j<10)
				{
					insert(send,pos,(9-j+1)*76," ");
					pos+=(9-j+1)*76;
				}
				i++;
				
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,guangdiansendtaocanmulujiexitaocanmingxi[selecttaocan].value[0]);   //保留1
				pos+=jiaofeitoyinhangpeizhi[i++].length;
			}
			else
			{
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,"1   ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;

				jilushu=0;
				inttostring(jilushu,taocanshuliang);

				insert(send,pos,jiaofeitoyinhangpeizhi[i].length,taocanshuliang);   //账单数量
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
				pos+=jiaofeitoyinhangpeizhi[i++].length;
				insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");   //保留1
        pos+=jiaofeitoyinhangpeizhi[i++].length;
			}

		}


        
        
        
        
        
        
        
        


        
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeitoyinhangpeizhi[i].length," ");
        pos+=jiaofeitoyinhangpeizhi[i++].length;
	}
	
	
	
	else if(n==7)  //用户查询
	{
		insert(send,pos,qianfeitoyinhangpeizhi[i].length,"1900");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeijiexi[2].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        if(strcmp(qianfeifanhuijiexi[3].value,"0000")==0)
        {
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,qianfeitoyinhangpeizhi[i].length,"99  ");//00 99
		}
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[4].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeijiexi[5].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[7].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[8].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        strcat(address,qianfeifanhuijiexi[9].value);
        strcat(address,qianfeifanhuijiexi[10].value);
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,address);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"095598");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"电力");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"000");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"电力");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"0000");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,"全部");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        jilushu = stringtoint(qianfeifanhuijiexi[13].value,0,strlen(qianfeifanhuijiexi[13].value)-1);
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jilushu   %d\n",jilushu);
        min=0,max=0;
        for(j=0; j<jilushu; j++)
        {
            if(strcmp(qianfeifanhuijiexijilu[min].value[1],qianfeifanhuijiexijilu[j].value[1])>0)
            {
                min=j;
            }
            if(strcmp(qianfeifanhuijiexijilu[max].value[1],qianfeifanhuijiexijilu[j].value[1])<0)
            {
                max=j;
            }
            sum_yingjiao+=stringtoint(qianfeifanhuijiexijilu[j].value[2],0,strlen(qianfeifanhuijiexijilu[j].value[2])-1);
            sum_weiyuejin+=stringtoint(qianfeifanhuijiexijilu[j].value[3],0,strlen(qianfeifanhuijiexijilu[j].value[3])-1);

			// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d----%d---\n",sum_yingjiao,sum_weiyuejin);
		}
        inttostring(sum_yingjiao,jine);
		// sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine---%s\n",jine);
        fentoyuan(jine);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine1--%s\n",jine);
        insertjine(send,pos,qianfeitoyinhangpeizhi[i].length-strlen(jine),"");
        pos+=qianfeitoyinhangpeizhi[i].length-strlen(jine);
        insert(send,pos,strlen(jine),jine);
        pos+=strlen(jine);
        i++;
        inttostring(sum_weiyuejin,jine);
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine---%s\n",jine);
        fentoyuan(jine);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine2--%s\n",jine);
        insertjine(send,pos,qianfeitoyinhangpeizhi[i].length-strlen(jine),"");
        pos+=qianfeitoyinhangpeizhi[i].length-strlen(jine);
        insert(send,pos,strlen(jine),jine);
        pos+=strlen(jine);
        i++;
        leng=strlen(qianfeifanhuijiexi[11].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=qianfeifanhuijiexi[11].value[k];
		}
		fentoyuan(tmp);
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"jine3--%s\n",tmp);
		insertjine(send,pos,qianfeitoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=qianfeitoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"send====%s\n",send);
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexijilu[min].value[1]);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexijilu[max].value[1]);
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length,qianfeifanhuijiexi[13].value);
        pos+=qianfeitoyinhangpeizhi[i++].length;





        for(j=0; j<jilushu; j++)
        {
            insert(send,pos,4,shuzitobianma[j]);
            pos+=4;
            insert(send,pos,30,qianfeifanhuijiexijilu[j].value[0]);
            pos+=30;
            //int k;

			sum=stringtoint(qianfeifanhuijiexijilu[j].value[2],0,strlen(qianfeifanhuijiexijilu[j].value[2])-1)+stringtoint(qianfeifanhuijiexijilu[j].value[3],0,strlen(qianfeifanhuijiexijilu[j].value[3])-1);
			inttostring(sum,jine);
			fentoyuan(jine);
			insertjine(send,pos,12-strlen(jine),"");
			pos+=12-strlen(jine);
            insert(send,pos,strlen(jine),jine);
            pos+=strlen(jine);
        }
        if(j<10)
        {
            insert(send,pos,(9-j+1)*46," ");
            pos+=(9-j+1)*46;
        }
        i++;


        //insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        //pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
        insert(send,pos,qianfeitoyinhangpeizhi[i].length," ");
        pos+=qianfeitoyinhangpeizhi[i++].length;
	}

	//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"chongzufabao_to_yinhang------------%s\n",send);
	//电力
    else if(n==8) //读卡信息查询
    {
		insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,"0936");
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromyinhangjiexi[2].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        if(strcmp(dukachaxunfromelecjiexi[3].value,"0000")==0)
        {
			insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,"99  ");//00 99
		}
        //insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[3].value);//00 99
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
		strcat(elecReturnCodeAndMessage,dukachaxunfromelecjiexi[3].value);
		strcat(elecReturnCodeAndMessage,dukachaxunfromelecjiexi[4].value);		
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
        //insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[4].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromyinhangjiexi[5].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length," ");
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length," ");
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length," ");
        pos+=dukachaxuntoyinhangpeizhi[i++].length;

        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[5].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[6].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[7].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[8].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[9].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;

        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[10].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[15].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[22].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;

        leng=strlen(dukachaxunfromelecjiexi[16].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=dukachaxunfromelecjiexi[16].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,dukachaxuntoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=dukachaxuntoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;

		leng=strlen(dukachaxunfromelecjiexi[17].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=dukachaxunfromelecjiexi[17].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,dukachaxuntoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=dukachaxuntoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;

		leng=strlen(dukachaxunfromelecjiexi[18].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=dukachaxunfromelecjiexi[18].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,dukachaxuntoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=dukachaxuntoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;

		leng=strlen(dukachaxunfromelecjiexi[23].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=dukachaxunfromelecjiexi[23].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,dukachaxuntoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=dukachaxuntoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;

        //insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[16].value);
        //pos+=dukachaxuntoyinhangpeizhi[i++].length;
        //insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[17].value);
        //pos+=dukachaxuntoyinhangpeizhi[i++].length;
       // insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[18].value);
        //pos+=dukachaxuntoyinhangpeizhi[i++].length;
        //insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[23].value);
        //pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[19].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[24].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;

		insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,dukachaxunfromelecjiexi[20].value);
        pos+=dukachaxuntoyinhangpeizhi[i++].length;

		leng=strlen(dukachaxunfromelecjiexi[21].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=dukachaxunfromelecjiexi[21].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,dukachaxuntoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=dukachaxuntoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;
		//insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,tmp);
        //pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,"");
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,"");
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,dukachaxuntoyinhangpeizhi[i].length,"");
        pos+=dukachaxuntoyinhangpeizhi[i++].length;
    }
    else if(n==9)//卡表购电
    {
		insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"4044");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromyinhangjiexi[2].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        if(strcmp(kabiaogoudianfromelecjiexi[3].value,"0000")==0)
        {
			insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"99  ");//00 99
		}
        //insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[3].value);//00 99
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
		strcat(elecReturnCodeAndMessage,kabiaogoudianfromelecjiexi[3].value);
		strcat(elecReturnCodeAndMessage,kabiaogoudianfromelecjiexi[4].value);		
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
        //insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[4].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromyinhangjiexi[5].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;

        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[5].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;

       /* leng=strlen(kabiaogoudianfromelecjiexi[6].value);
        memset(tmp,'\0',13);
		for(k=0,j=0; k<leng; k++,j++)
		{
			if((kabiaogoudianfromelecjiexi[6].value[j]=='.')&&(j==leng-3))
			{
				j++;
			}
			else if((kabiaogoudianfromelecjiexi[6].value[j]=='.')&&(j==leng-2))
			{
				j++;
				tmp[k+2]='0';
				tmp[k]=kabiaogoudianfromelecjiexi[6].value[j];
			}
			else
			{
				tmp[k]=kabiaogoudianfromelecjiexi[6].value[j];
			}
		}
		fentoyuan(tmp);
		insertjine(send,pos,kabiaogoudiantoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=kabiaogoudiantoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;
		*/

        //insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[6].value);
        //pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
		leng=strlen(kabiaogoudianfromelecjiexi[6].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=kabiaogoudianfromelecjiexi[6].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,kabiaogoudiantoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=kabiaogoudiantoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;


        leng=strlen(kabiaogoudianfromelecjiexi[7].value);
        memset(tmp,'\0',13);
		for(k=0; k<leng; k++)
		{
			tmp[k]=kabiaogoudianfromelecjiexi[7].value[k];
		}
		fentoyuan(tmp);
		insertjine(send,pos,kabiaogoudiantoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=kabiaogoudiantoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;

        //insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[7].value);
        //pos+=kabiaogoudiantoyinhangpeizhi[i++].length;

        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[8].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[9].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[10].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[11].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[12].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[13].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[14].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[15].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[16].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,kabiaogoudianfromelecjiexi[17].value);
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudiantoyinhangpeizhi[i].length,"");
        pos+=kabiaogoudiantoyinhangpeizhi[i++].length;

    }
    else if(n==10)//卡表购电冲正
    {
		insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,"0564");
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,kabiaogoudianchongzhengfromyinhangjiexi[2].value);
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        if(strcmp(kabiaogoudianchongzhengfromelecjiexi[3].value,"0000")==0)
        {
			insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,"99  ");//00 99
		}
        //insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[3].value);//00 99
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
		strcat(elecReturnCodeAndMessage,kabiaogoudianchongzhengfromelecjiexi[3].value);
		strcat(elecReturnCodeAndMessage,kabiaogoudianchongzhengfromelecjiexi[4].value);		
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
        //insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,kabiaogoudianchongzhengfromelecjiexi[4].value);
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,kabiaogoudianchongzhengfromyinhangjiexi[5].value);
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;

        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length,kabiaogoudianchongzhengfromelecjiexi[5].value);
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
        insert(send,pos,kabiaogoudianchongzhengtoyinhangpeizhi[i].length," ");
        pos+=kabiaogoudianchongzhengtoyinhangpeizhi[i++].length;
    }
    else if(n==11)//补写卡
    {
		insert(send,pos,buxiekatoyinhangpeizhi[i].length,"4032");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromyinhangjiexi[2].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        if(strcmp(buxiekafromelecjiexi[3].value,"0000")==0)
        {
			insert(send,pos,buxiekatoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,buxiekatoyinhangpeizhi[i].length,"99  ");//00 99
		}
        //insert(send,pos,buxiekatoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[3].value);//00 99
        pos+=buxiekatoyinhangpeizhi[i++].length;
		strcat(elecReturnCodeAndMessage,buxiekafromelecjiexi[3].value);
		strcat(elecReturnCodeAndMessage,buxiekafromelecjiexi[4].value);		
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
		
        //insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[4].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromyinhangjiexi[5].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length," ");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length," ");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length," ");
        pos+=buxiekatoyinhangpeizhi[i++].length;


        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[5].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;


		 leng=strlen(buxiekafromelecjiexi[6].value);
        memset(tmp,'\0',13);
		for(k=0,j=0; k<leng; k++,j++)
		{
			if((buxiekafromelecjiexi[6].value[j]=='.')&&(j==leng-3))
			{
				j++;
			}
			else if((buxiekafromelecjiexi[6].value[j]=='.')&&(j==leng-2))
			{
				j++;
				tmp[k+2]='0';
				tmp[k]=buxiekafromelecjiexi[6].value[j];
			}
			else
			{
				tmp[k]=buxiekafromelecjiexi[6].value[j];
			}
		}
		fentoyuan(tmp);
		insertjine(send,pos,buxiekatoyinhangpeizhi[i].length-strlen(tmp),"");
        pos+=buxiekatoyinhangpeizhi[i].length-strlen(tmp);
        insert(send,pos,strlen(tmp),tmp);
        pos+=strlen(tmp);
        i++;

        //insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[6].value);
        //pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[7].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[8].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[9].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[10].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[11].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[12].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[13].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[14].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[15].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,buxiekafromelecjiexi[16].value);
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,"");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,"");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,"");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,"");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,"");
        pos+=buxiekatoyinhangpeizhi[i++].length;
        insert(send,pos,buxiekatoyinhangpeizhi[i].length,"");
        pos+=buxiekatoyinhangpeizhi[i++].length;
    }
	else if(n==12)//缴费票据查询
    {
		insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,"9620");
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,jiaofeipiaojuchaxunfromyinhangjiexi[2].value);
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
        if(strcmp(jiaofeipiaojuchaxunfromelecjiexi[3].value,"0000")==0)
        {
			insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,"99  ");//00 99
		}
        //insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[3].value);//00 99
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
		
		strcat(elecReturnCodeAndMessage,jiaofeipiaojuchaxunfromelecjiexi[3].value);
		strcat(elecReturnCodeAndMessage,jiaofeipiaojuchaxunfromelecjiexi[4].value);		
        insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,elecReturnCodeAndMessage);
        //insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,jiaofeipiaojuchaxunfromelecjiexi[4].value);
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,jiaofeipiaojuchaxunfromyinhangjiexi[5].value);
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length," ");
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length," ");
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length," ");
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;

		jilushu=stringtoint(jiaofeipiaojuchaxunfromelecjiexi[5].value,0,strlen(jiaofeipiaojuchaxunfromelecjiexi[5].value)-1);
	/*	realjilushu=0;
		for(j=0;j<jilushu; j++)
		{
			if(!strcmp(jiaofeipiaojuchaxunfromelecjiexijilu[j].value[1],"2"))
			{
				realjilushu++;
			}
		}
		if(realjilushu>10)
		{
			realjilushu=10;
		}
		*/
		realjilushu=jilushu;
		inttostring(realjilushu,charjilushu);
        insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,charjilushu);
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;

		for(j=0; j<jilushu; j++)
        {
			insert(send,pos,740,jiaofeipiaojuchaxunfromelecjiexijilu[j].value);
			pos+=740;
        }
        if(j<12)
        {
            insert(send,pos,(11-realjilushu+1)*740," ");
            pos+=(11-realjilushu+1)*740;
          //  insert(send,pos,80," ");
           // pos+=80;
        }
        i++;
        

     //   insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,jiaofeipiaojuchaxunfromelecjiexi[6].value);
     //   pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;

		insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,"");
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
		insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,"");
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
		insert(send,pos,jiaofeipiaojuchaxuntoyinhangpeizhi[i].length,"");
        pos+=jiaofeipiaojuchaxuntoyinhangpeizhi[i++].length;
	}
	else if(n==13)//票据打印信息查询
	{
		insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,"9184");
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,piaojudayinxinxichaxunfromyinhangjiexi[2].value);
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
      /*  if(strcmp(piaojudayinxinxichaxunfromelecjiexi[3].value,"0000")==0)
        {
			insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,"00  ");//00 99
		}
		else
		{
			insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,"99  ");//00 99
		}
*/
insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,"00  ");
        //insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,daikoubankqianjieyuefanhuijiexi[3].value);//00 99
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,piaojudayinxinxichaxunfromelecjiexi[4].value);
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,piaojudayinxinxichaxunfromyinhangjiexi[5].value);
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length," ");
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length," ");
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
        insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length," ");
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
        /*
		for(j=strlen(XML_PATH)-1;j>=0;j--)
		{
			if(XML_PATH[j]=='/')
			{
				break;
			}
		}
		k=0;
		j++;
		for(;j<strlen(XML_PATH);j++)
		{
			filename[k++]=XML_PATH[j];
		}
		insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,filename);
		*/
		insert(send,pos,piaojudayinxinxichaxuntoyinhangpeizhi[i].length,piaojudayinxinxichaxunfromelecjiexi[3].value);
        pos+=piaojudayinxinxichaxuntoyinhangpeizhi[i++].length;
	}
	else
	{
	}
	//			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"chongzufabao_to_yinhang------------%s\n",send);
    return;
}

int gettype(char * buf,char *temp)
{
    //char temp[8];
    int i,j=0;
    for(i = JIAO_YI_MA_OFFSET; i < JIAO_YI_MA_OFFSET + JIAO_YI_MA_LENGTH; i++)
    {
        temp[j]=buf[i];
        j++;
    }
    temp[j]='\0';
    return 1;
}
int gettypeguangdian(char * buf,char *temp)
{
	//char temp[8];
	int i,j=0;
	for(i=10;i<=15;i++)
	{
		temp[j]=buf[i];
		j++;
	}
	temp[j]='\0';
	return 1;
}


void insertjine(char *send,int start ,int length,char *node)
{
    int i,j=0;
	if(strlen(node)>length)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(0);
	}
    for(i=start; j<strlen(node); i++)
    {

        send[i]=node[j];
        j++;

    }
    for(i=start+j; i<start+length; i++) //bu zu  bu 'p'
    {
        send[i]='0';
    }


}

void insert(char *send,int start ,int length,char *node)
{
    int i,j=0;

	if(strlen(node)>length)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		for(i=start; j<length; i++)
		{
			send[i]=node[j];
			j++;
		}
		return;
		//write_record();
		//exit(0);
	}
    for(i=start; j<strlen(node); i++)
    {

        send[i]=node[j];
        j++;

    }
    for(i=start+j; i<start+length; i++) //bu zu  bu 'p'
    {
        send[i]=' ';
    }


}
int insertelec(char *send,int start ,int length,char *node)
{
    int i,j=0,k=0,count=0;
	int leng=strlen(node);
	for(i=leng-1;i>=0;i--)
	{
		if(node[i]!=' ')
		{
			break;
		}
	}
	leng=i+1;
	if(leng>length)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(0);
	}
    for(i=start; j<strlen(node); i++)
    {
        if(node[j]==' ')
        {
            for(k=j; k<strlen(node); k++)
            {
                if(node[k]!=' ')
                {
                    break;
                }
            }
        }
        if(k==strlen(node))
        {
            return j;
        }

        send[i]=node[j];
        j++;

    }
    return j;

}

int insertguangdian(char *send,int start ,int length,char *node)
{
   int i,j=0,k=0,count=0;
	int leng=strlen(node);
	int min;
	//判断银行的数据去掉空格之后是否比广电的该字段长度长
	for(i=leng-1;i>=0;i--)
	{
		if(node[i]!=' ')
		{
			break;
		}
	}
	leng=i+1;
	if(leng>length)
	{
		sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
		exit(0);
	}
	min=length>strlen(node)?strlen(node):length;

    for(i=start; j<min; i++)
    {

        send[i]=node[j];
        j++;

    }
    for(i=start+j; i<start+length; i++) //bu zu  bu 'p'
    {
        send[i]=' ';
    }
    return j;

}



int gettypeelec(char * buf,char *temp)
{
    //char temp[8];
    int i,j=0;
    for(i=HEAD_LENGTH; i<HEAD_LENGTH + JIAO_YI_LEI_XING_LENGTH; i++)
    {
        temp[j]=buf[i];
        j++;
    }
    temp[j]='\0';
    return 1;
}


int stringtoint(char *buf,int start,int end)
{

    int sum=0,i,j=0;
    for(i=end,j=0; i>=start; i--,j++)
    {
        sum += (buf[i] - '0') * (int)pow(10,j);
    }
	//	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"stringtoint------%d\n",sum);
    return sum;
}
void inttostring(int n,char *buf)
{
	int i=0;
	char temp;
	memset(buf,'\0',sizeof(buf));
	if(n==0)
	{
		buf[0]='0';
		return;
	}
	while(n!=0)
	{
		buf[i]=n%10+'0';
		//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%c\n",buf[i]);
		n=n/10;
		i++;
	}
	buf[i]='\0';
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%d,buf====%s\n",i,buf);
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"strken%d\n",strlen(buf));
	for(i=0;i<strlen(buf)/2;i++)
	{
		temp=buf[i];
		buf[i]=buf[strlen(buf)-i-1];
		buf[strlen(buf)-i-1]=temp;
	}
}
void fentoyuan(char *buf)
{
	int leng=strlen(buf);
	int i;
	if(leng==1)
	{
		buf[4]='\0';
		buf[3]=buf[0];
		buf[2]='0';
		buf[1]='.';
		buf[0]='0';
	}
	else if(leng==2)
	{
		if(buf[0]=='-')
		{
			buf[5]='\0';
			buf[4]='0';
			buf[3]=buf[1];
			buf[2]='.';
			buf[1]='0';
		}
		else
		{
			buf[4]='\0';
			buf[3]=buf[1];
			buf[2]=buf[0];
			buf[1]='.';
			buf[0]='0';
		}
	}
	else if(leng==3)
	{
		if(buf[0]=='-')
		{
			buf[5]='\0';
			buf[4]=buf[1];
			buf[3]=buf[0];
			buf[2]='.';
			buf[1]='0';
		}
		else
		{
			buf[leng+1]='\0';
			buf[leng]=buf[leng-1];
			buf[leng-1]=buf[leng-2];
			buf[leng-2]='.';
		}
	}
	else
	{
		buf[leng+1]='\0';
		buf[leng]=buf[leng-1];
		buf[leng-1]=buf[leng-2];
		buf[leng-2]='.';
	}
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"yiwan%s\n",buf);
}


int lock_set(int fd, int type)
{
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_type = type;
    lock.l_pid = -1;
    fcntl(fd,F_SETLKW,&lock);
    return 1;
}
void sigchld_handler(int sig)
{
    while (waitpid(-1, 0, WNOHANG) > 0);
    return;
}

void sigalrm_handler(int sig)
{
	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][系统超时]\n",__FUNCTION__,__LINE__);
	write_record();
	exit(0);
}



void write_record()
{
	
	int file_fd;
	time_t rawtime;
	struct tm* timeinfo;
	char timE[80];
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(timE,80,"%Y%m%drecord.txt",timeinfo);
	char recordpath[100];
	memset(recordpath,'\0',100);
	strcpy(recordpath,RECORD_PATH);
	strcat(recordpath,timE);
	
	//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"日志文件路径：%s\n",recordpath,strlen(STRING_FOR_RECORD));
    file_fd = open(recordpath,O_CREAT | O_APPEND | O_RDWR,0644);

    lock_set(file_fd,F_WRLCK);
	
	time(&rawtime);
	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s[进程%d END][函数%s:%d行]\n",ctime(&rawtime),getpid(),__FUNCTION__,__LINE__);
    write(file_fd,STRING_FOR_RECORD,strlen(STRING_FOR_RECORD));
    
    
    
    lock_set(file_fd,F_UNLCK);
    close(file_fd);
	
	printf("%s\n",STRING_FOR_RECORD);
	memset(STRING_FOR_RECORD,'\0',sizeof(STRING_FOR_RECORD));
    return;
    
    
    
    /*int i;
    int len=strlen(s);
    int file_fd;
    time_t timep;
    time (&timep);
    char baolength[5];
	//				char temp[]="-----------------------------------------------------------\n";
    char temp1[]="-----------BANK SEND------------------------------------------\n";
    char temp2[]="-----------ELEC RECEIVE0--------------------------------------\n";
    char temp3[]="-----------ELEC SEND0----------------------------------------\n";

    char temp4[]="----------BANK RECEIVE-----------------------------------------\n";
	time_t rawtime;
	struct tm* timeinfo;
	char timE[80];
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(timE,80,"%Y%m%drecord",timeinfo);
	char recordpath[100];
	memset(recordpath,'\0',100);
	strcpy(recordpath,RECORD_PATH);
	strcat(recordpath,timE);
	sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"%s\n",recordpath);
    file_fd = open(recordpath,O_CREAT | O_APPEND | O_RDWR,0644);
	//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"-----%d\n",file_fd);
    lock_set(file_fd,F_WRLCK);

    //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),“%s”,ctime(&timep));

	//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

    write(file_fd,"*************",strlen("*************"));
    write(file_fd,ctime(&timep),strlen(ctime(&timep)));
	//				write(file_fd,"*************",strlen("*************"));

	//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"temp\n------%d",x);
    if(n == 0)
        write(file_fd,temp1,strlen(temp1));
    else if(n==1)
        write(file_fd,temp2,strlen(temp2));
    else if(n==2)
        write(file_fd,temp3,strlen(temp3));
    else if(n==3)
        write(file_fd,temp4,strlen(temp4));
    else
    {
    }
    for(i=0; i<4; i++)
    {
        baolength[3-i]=len%10+'0';
        len/=10;
    }
    baolength[4]='\0';
	//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"---%s\n",baolength);

    write(file_fd,"[",strlen("["));
    write(file_fd,baolength,4);
    write(file_fd,"]",strlen("]"));

    write(file_fd,"[",strlen("["));
    len =strlen(s);
    char t[1];
    for(i=0; i<len; i++)
    {
        t[0]=s[i];
        write(file_fd,t,1);
        if((i%50==0)&&(i!=0))
            write(file_fd,"\n",strlen("\n"));
    }
    write(file_fd,"]",strlen("]"));

    write(file_fd,"\n",strlen("\n"));
    write(file_fd,"\n",strlen("\n"));



    lock_set(file_fd,F_UNLCK);
    close(file_fd);
    return;
	//				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"close-----");
*/
}

int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	size_t in =(size_t)inlen;
	size_t out =(size_t)outlen;


	cd = iconv_open(to_charset,from_charset);
	if (cd==0)
		return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&in,pout,&out) == -1)
		return -1;
	iconv_close(cd);
	return 0;
}

//void ftp_send_file()
//{
	//FILE	*fpFtp;
	//char	sBuf[1000];
	//int i,j,k;
	//char filename[50];
	//memset(filename,'\0',50);
	//for(j=strlen(XML_PATH)-1;j>=0;j--)
		//{
			//if(XML_PATH[j]=='/')
			//{
				//break;
			//}
		//}
		//k=0;
		//j++;
		//for(;j<strlen(XML_PATH);j++)
		//{
			//filename[k++]=XML_PATH[j];
		//}
	//fpFtp = fopen(FTP_PATH, "w");


///*
	//fprintf(fpFtp, "open 10.27.0.188\n");
	//fprintf(fpFtp, "user chargftp chargftp\n");
	//fprintf(fpFtp, "cd usr\n");
	//fprintf(fpFtp, "cd chgtest\n");
	//fprintf(fpFtp, "cd xml\n");
	//fprintf(fpFtp, "lcd /home/server/projecttest/file/xml\n");
	//*/
///*
	//fprintf(fpFtp, "open 127.0.0.1\n");
	//fprintf(fpFtp, "user haifeng 199213\n");
	//fprintf(fpFtp, "cd home\n");
	//fprintf(fpFtp, "cd haifeng\n");
	//fprintf(fpFtp, "cd project\n");
	//fprintf(fpFtp, "lcd /home/haifeng/project/server/file/xml\n");
//*/

	//fprintf(fpFtp, "open 127.0.0.1\n");
	//fprintf(fpFtp, "user gd_bank 88203643\n");
	//fprintf(fpFtp, "lcd /home/server/projecttest/server/file/xml\n");



	//fprintf(fpFtp, "put %s\n", filename);
	//fprintf(fpFtp, "bye\n");
	//fclose(fpFtp);

	//sprintf(sBuf, "ftp -n <%s 1>> %s 2>> %s",FTP_PATH,FTP_PATH,FTP_PATH);

	//system(sBuf);

	//return;
//}

/*
 *
 *0 insert
 *1 update
 *
 */
void save_query(char* sql,int n) 					//---------------------------------将一次查询操作保存至本地数据库
{
    MYSQL *conn_ptr;
    int res;

    conn_ptr = mysql_init(NULL);
    if (!conn_ptr)
    {
       sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行]\n",__FUNCTION__,__LINE__);
		write_record();
        return;
    }
#ifdef FORMAL_ENVIRONMENT
	conn_ptr = mysql_real_connect(conn_ptr, MYSQL_IP, MYSQL_USER,MYSQL_PASSWORD, "formal_gdbank_data", 0, NULL, 0);
#endif
#ifdef TESTING_ENVIRONMENT
	conn_ptr = mysql_real_connect(conn_ptr, MYSQL_IP, MYSQL_USER,MYSQL_PASSWORD, "test_gdbank_data", 0, NULL, 0);
#endif
#ifdef DEBUG_ENVIRONMENT
	conn_ptr = mysql_real_connect(conn_ptr, MYSQL_IP, MYSQL_USER,MYSQL_PASSWORD, "test_gdbank_data", 0, NULL, 0);
#endif
   
    mysql_set_character_set(conn_ptr,"gb2312");

	if (conn_ptr)
	{
		if(n==0)
		{
			res = mysql_query(conn_ptr, sql);   //可以把insert语句替换成delete或者update语句，都一样的
			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][%s]\n",getpid(),__FUNCTION__,__LINE__,sql);
//			 sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"Inserted sql--------------%s\n",sql);
			
			if (!res)
			{//输出受影响的行数
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][Inserted %lu rows]\n",getpid(),__FUNCTION__,__LINE__,(unsigned long)mysql_affected_rows(conn_ptr));
 
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"\nInserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
			}
			else
			{//打印出错误代码及详细信息
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][%d:%s]\n",__FUNCTION__,__LINE__,mysql_errno(conn_ptr),mysql_error(conn_ptr));
			}
			
		}
		else if(n==1)
		{
			res = mysql_query(conn_ptr, sql);   //可以把insert语句替换成delete或者update语句，都一样的
			//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"Update sql--------------%s\n",sql);
			sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][%s]\n",getpid(),__FUNCTION__,__LINE__,sql);
			if (!res)
			{//输出受影响的行数
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[进程%d][函数%s:%d行][Update %lu rows]\n",getpid(),__FUNCTION__,__LINE__,(unsigned long)mysql_affected_rows(conn_ptr));
 
				//sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"\nUpdate %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
			}
			else
			{//打印出错误代码及详细信息
				sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][%d:%s]\n",__FUNCTION__,__LINE__,mysql_errno(conn_ptr),mysql_error(conn_ptr));
			}
			
		}

    }
	else
	{
        //sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"Connection failed\n");
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][Connection failed]\n",__FUNCTION__,__LINE__);
	
        return ;
    }
    mysql_close(conn_ptr);
    return;
}

void write_mysql(char *buf)
{
	 int i,j;
	 char tempchar[2];
	 char sql[BUFFER_SIZE];
	 char deletetailspacechar[BUFFER_SIZE];
	 struct tm *ptr;
	 time_t lt;
	 char str[80];
	 int leng;
	 leng = strlen(buf);
	 lt=time(NULL);
	 ptr=localtime(&lt);
	 strftime(str,sizeof(str),"%F %T",ptr);
	 memset(sql,'\0',BUFFER_SIZE);
	 char type[JIAO_YI_MA_LENGTH + 1];
	 memset(type,'\0',sizeof(type));
     if(!gettype(buf,type))
     {
        sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"[error][函数%s:%d行][can't get type!]\n",__FUNCTION__,__LINE__);
     }
     if(!strcmp(type,"0001    "))//0001：交费
     {
        strcat(sql,"insert into  gdbank_log values('");

        DeleteTailSpace(jiaofeijiexi[2].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易标识：流水号
        strcat(sql,"','");

        DeleteTailSpace(jiaofeijiexi[1].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易码：缴费0001、撤销0002、卡表购电1001、卡表购电撤销1002
        strcat(sql,"','");

        strcat(sql,"缴费");   //交易名称：缴费、撤销、卡表购电、卡表购电撤销
        strcat(sql,"','");

        DeleteTailSpace(jiaofeijiexi[13].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //客户标识：缴费号、客户编号（设保留字段，填写关联信息）缴费交易，用于标识客户身份的信息
        strcat(sql,"','");

        strcat(sql,"");   //客户辅助信息：（客户标识的关联信息等）(暂时为空)
        strcat(sql,"','");

        DeleteTailSpace(qianfeifanhuijiexi[8].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //客户名称：
        strcat(sql,"','");

        DeleteTailSpace(jiaofeijiexi[3].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易日期：
        strcat(sql,"','");

        DeleteTailSpace(jiaofeijiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易时间：
        strcat(sql,"',");

        DeleteTailSpace(jiaofeijiexi[16].value,deletetailspacechar);
        yuantofen(deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易金额：

        strcat(sql,",");

        if(strcmp(jiaofeifanhuijiexi[3].value,"0000")==0)   //交易成功标识：0、1、2    0成功，1失败，2是撤销
        {
			strcat(sql,"0");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
            strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
            strcat(sql,"失败");
            strcat(sql,"','");
		}

        strcat(sql,"");   //原交易流水号：缴费交易时为空
        strcat(sql,"',");



         if(strcmp(jiaofeifanhuijiexi[3].value,"0000")==0)   //交易状态：0、1   0成功，1失败
        {
			strcat(sql,"0");   //交易状态：0、1    0成功，1失败
            strcat(sql,",'");
            strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易状态：0、1    0成功，1失败，
            strcat(sql,",'");
            strcat(sql,"失败");
            strcat(sql,"','");
		}
        DeleteTailSpace(jiaofeifanhuijiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //返回信息：？
        strcat(sql,"','");

        DeleteTailSpace(jiaofeijiexi[5].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //区域编码：
        strcat(sql,"','");

        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //区域名称：
        {
			if(!strcmp(jiaofeijiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{
			    DeleteTailSpace(dqdmqdsbhwddmczybhmyyingshepeizhi[i].diquname,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //区域名称：
                strcat(sql,"','");
                break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //区域名称：
            strcat(sql,"','");
		}

        DeleteTailSpace(jiaofeijiexi[6].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易渠道编码：
        strcat(sql,"','");


        for(i=0;i<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //交易渠道名称:
        {
			if(!strcmp(jiaofeijiexi[6].value,jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbm))
			{
			    DeleteTailSpace(jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbmmc,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //交易渠道名称:
                strcat(sql,"','");
                break;
			}
		}
		if(i==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //交易渠道名称:
            strcat(sql,"','");
		}



        DeleteTailSpace(jiaofeijiexi[7].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易机构：
        strcat(sql,"','");

        strcat(sql,"");   //交易机构名称：?????
        strcat(sql,"','");

        DeleteTailSpace(jiaofeijiexi[8].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //操作员号
        strcat(sql,"','");

        DeleteTailSpace(jiaofeijiexi[9].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //收费单位类别编码：
        strcat(sql,"','");

        strcat(sql,"电费缴费");   //收费单位类别名称：公共事业类缴费、物业类、学费类、其它类
        strcat(sql,"','");

        strcat(sql,"095598");   //收费单位一级机构编码：一级机构编码
        strcat(sql,"','");

        strcat(sql,"电力");   //收费单位一级机构名称：
        strcat(sql,"','");

        strcat(sql,"000");   //收费单位二级机构编码：
        strcat(sql,"','");

        strcat(sql,"电力");   //收费单位二级机构名称：
        strcat(sql,"',");

        strcat(sql,"0");   //删除标记   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,",'");

        strcat(sql,"未删除");   //删除标记位详情   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,"',");

        strcat(sql,"0");   //修改标记   数字0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,",'");

        strcat(sql,"未修改");   //修改标记位详情   0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,"',");

        strcat(sql,"0");   //对账结果   0--未对账1---对账成功2---记录有冲突3-对账文件不存在本记录
        strcat(sql,",'");

        strcat(sql,"未对账");   //对账结果详情  0--未对账   1-对账成功  2---记录有冲突  3-对账文件不存在本记录
        strcat(sql,"','");

        strcat(sql,"");   //备注
        strcat(sql,"','");

        strcat(sql,"");   //保留1
        strcat(sql,"','");

        strcat(sql,"");   //保留2
        strcat(sql,"','");

        strcat(sql,"");   //保留3
        strcat(sql,"');");

       // sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"sql--------------%s\n",sql);
        save_query(sql,0);

     }
     else if(!strcmp(type,"0002    "))     //0002：收费撤销
     {
        //记录撤销这笔交易
		strcat(sql,"insert into  gdbank_log values('");

        DeleteTailSpace(chexiaojiexi[2].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易标识：流水号
        strcat(sql,"','");

        DeleteTailSpace(chexiaojiexi[1].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易码：缴费0001、撤销0002、卡表购电1001、卡表购电撤销1002
        strcat(sql,"','");
        strcat(sql,"撤销");   //交易名称：缴费、撤销、卡表购电、卡表购电撤销
        strcat(sql,"','");

        DeleteTailSpace(chexiaojiexi[13].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //客户标识：缴费号、客户编号（设保留字段，填写关联信息）缴费交易，用于标识客户身份的信息
        strcat(sql,"','");
        strcat(sql,"");   //客户辅助信息：（客户标识的关联信息等）(暂时为空)
        strcat(sql,"','");
        strcat(sql,"");   //客户名称：
        strcat(sql,"','");

        DeleteTailSpace(chexiaojiexi[3].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易日期：
        strcat(sql,"','");

        DeleteTailSpace(chexiaojiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易时间：
        strcat(sql,"',");

        DeleteTailSpace(chexiaojiexi[15].value,deletetailspacechar);
        yuantofen(deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易金额：
        strcat(sql,",");

        if(strcmp(chexiaofanhuijiexi[3].value,"0000")==0)   //交易成功标识：0、1、2    0成功，1失败，2是撤销
        {
			strcat(sql,"0");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
              strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
              strcat(sql,"失败");
            strcat(sql,"','");
		}

        DeleteTailSpace(chexiaojiexi[14].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //原交易流水号：缴费交易时为空
        strcat(sql,"',");



        if(strcmp(chexiaofanhuijiexi[3].value,"0000")==0)   //交易状态：0、1    0成功，1失败，
        {
			strcat(sql,"0");   //交易状态：0、1   0成功，1失败
            strcat(sql,",'");
              strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易状态：0、1    0成功，1失败，
            strcat(sql,",'");
              strcat(sql,"失败");
            strcat(sql,"','");
		}

        DeleteTailSpace(chexiaofanhuijiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //返回信息：？
        strcat(sql,"','");

        DeleteTailSpace(chexiaojiexi[5].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //区域编码：
        strcat(sql,"','");

        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //区域名称：
        {
			if(!strcmp(chexiaojiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{

                DeleteTailSpace(dqdmqdsbhwddmczybhmyyingshepeizhi[i].diquname,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //区域名称：
                strcat(sql,"','");
                break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //区域名称：
            strcat(sql,"','");
		}


        DeleteTailSpace(chexiaojiexi[6].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易渠道编码：
        strcat(sql,"','");


        for(i=0;i<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //交易渠道名称:
        {
			if(!strcmp(chexiaojiexi[6].value,jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbm))
			{

                DeleteTailSpace(jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbmmc,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //交易渠道名称:
                strcat(sql,"','");
                break;
			}
		}
		if(i==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //交易渠道名称:
            strcat(sql,"','");
		}


        DeleteTailSpace(chexiaojiexi[7].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易机构：
        strcat(sql,"','");
        strcat(sql,"");   //交易机构名称：?????
        strcat(sql,"','");

        DeleteTailSpace(chexiaojiexi[8].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //操作员号
        strcat(sql,"','");

        DeleteTailSpace(chexiaojiexi[9].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //收费单位类别编码：
        strcat(sql,"','");
        strcat(sql,"电费缴费");   //收费单位类别名称：公共事业类缴费、物业类、学费类、其它类
        strcat(sql,"','");
        strcat(sql,"095598");   //收费单位一级机构编码：一级机构编码
        strcat(sql,"','");
        strcat(sql,"电力");   //收费单位一级机构名称：
        strcat(sql,"','");
        strcat(sql,"000");   //收费单位二级机构编码：
        strcat(sql,"','");
        strcat(sql,"电力");   //收费单位二级机构名称：
        strcat(sql,"',");
        strcat(sql,"0");   //删除标记   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,",'");

        strcat(sql,"未删除");   //删除标记位详情   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,"',");

        strcat(sql,"0");   //修改标记   数字0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,",'");

        strcat(sql,"未修改");   //修改标记位详情   0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,"',");

        strcat(sql,"0");   //对账结果   0--未对账1---对账成功2---记录有冲突3-对账文件不存在本记录
        strcat(sql,",'");

        strcat(sql,"未对账");   //对账结果详情  0--未对账   1-对账成功  2---记录有冲突  3-对账文件不存在本记录
        strcat(sql,"','");

        strcat(sql,"");   //备注
        strcat(sql,"','");

        strcat(sql,"");   //保留1
        strcat(sql,"','");

        strcat(sql,"");   //保留2
        strcat(sql,"','");

        strcat(sql,"");   //保留3
        strcat(sql,"');");
       // sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"sql--------------%s\n",sql);
        save_query(sql,0);
		//修改被撤销的缴费交易
		memset(sql,'\0',BUFFER_SIZE);
		if(strcmp(chexiaofanhuijiexi[3].value,"0000")==0)   //撤销成功，可以修改缴费交易
        {
			strcat(sql,"UPDATE gdbank_log SET jycgbs=2 WHERE jybs='");
            DeleteTailSpace(chexiaojiexi[14].value,deletetailspacechar);
			strcat(sql,deletetailspacechar);
            strcat(sql,"';");
            save_query(sql,1);
		}
     }
     else if(!strcmp(type,"1001    "))     //1001：卡表充值
     {
		strcat(sql,"insert into  gdbank_log values('");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[2].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易标识：流水号
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[1].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易码：缴费0001、撤销0002、卡表购电1001、卡表购电撤销1002
        strcat(sql,"','");
        strcat(sql,"卡表购电");   //交易名称：缴费、撤销、卡表购电、卡表购电撤销
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[13].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //客户标识：缴费号、客户编号（设保留字段，填写关联信息）缴费交易，用于标识客户身份的信息
        strcat(sql,"','");
        strcat(sql,"");   //客户辅助信息：（客户标识的关联信息等）(暂时为空)
        strcat(sql,"','");
        strcat(sql,"");   //客户名称：
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[3].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易日期：
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易时间：
        strcat(sql,"',");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[15].value,deletetailspacechar);
		yuantofen(deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易金额：
        strcat(sql,",");

        if(strcmp(kabiaogoudianfromelecjiexi[3].value,"0000")==0)   //交易成功标识：0、1、2    0成功，1失败，2是撤销
        {
			strcat(sql,"0");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
            strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
            strcat(sql,"失败");
            strcat(sql,"','");
		}
        strcat(sql,"");   //原交易流水号：缴费交易时为空
        strcat(sql,"',");

        if(strcmp(kabiaogoudianfromelecjiexi[3].value,"0000")==0)   //交易状态：0、1、2    0成功，1失败
        {
			strcat(sql,"0");   //交易状态：0、1    0成功，1失败
            strcat(sql,",'");
            strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易状态：0、1    0成功，1失败，
            strcat(sql,",'");
            strcat(sql,"失败");
            strcat(sql,"','");
		}



		DeleteTailSpace(kabiaogoudianfromelecjiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //返回信息：？
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[5].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //区域编码：
        strcat(sql,"','");

        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //区域名称：
        {
			if(!strcmp(kabiaogoudianfromyinhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{

                DeleteTailSpace(dqdmqdsbhwddmczybhmyyingshepeizhi[i].diquname,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //区域名称：
                strcat(sql,"','");
                break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //区域名称：
            strcat(sql,"','");
		}

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[6].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易渠道编码：
        strcat(sql,"','");


        for(i=0;i<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //交易渠道名称:
        {
			if(!strcmp(kabiaogoudianfromyinhangjiexi[6].value,jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbm))
			{
                DeleteTailSpace(jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbmmc,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //交易渠道名称:
                strcat(sql,"','");
                break;
			}
		}
		if(i==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //交易渠道名称:
            strcat(sql,"','");
		}

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[7].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易机构：
        strcat(sql,"','");
        strcat(sql,"");   //交易机构名称：?????
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[8].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //操作员号
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianfromyinhangjiexi[9].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //收费单位类别编码：
        strcat(sql,"','");
        strcat(sql,"电费缴费");   //收费单位类别名称：公共事业类缴费、物业类、学费类、其它类
        strcat(sql,"','");
        strcat(sql,"095598");   //收费单位一级机构编码：一级机构编码
        strcat(sql,"','");
        strcat(sql,"电力");   //收费单位一级机构名称：
        strcat(sql,"','");
        strcat(sql,"000");   //收费单位二级机构编码：
        strcat(sql,"','");
        strcat(sql,"电力");   //收费单位二级机构名称：
        strcat(sql,"',");
        strcat(sql,"0");   //删除标记   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,",'");

        strcat(sql,"未删除");   //删除标记位详情   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,"',");

        strcat(sql,"0");   //修改标记   数字0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,",'");

        strcat(sql,"未修改");   //修改标记位详情   0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,"',");

        strcat(sql,"0");   //对账结果   0--未对账1---对账成功2---记录有冲突3-对账文件不存在本记录
        strcat(sql,",'");

        strcat(sql,"未对账");   //对账结果详情  0--未对账   1-对账成功  2---记录有冲突  3-对账文件不存在本记录
        strcat(sql,"','");

        strcat(sql,"");   //备注
        strcat(sql,"','");

        strcat(sql,"");   //保留1
        strcat(sql,"','");

        strcat(sql,"");   //保留2
        strcat(sql,"','");

        strcat(sql,"");   //保留3
        strcat(sql,"');");
      // sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"sql--------------%s\n",sql);
        save_query(sql,0);
     }
     else if(!strcmp(type,"1002    "))    //1002：卡表充值撤销
     {
		//记录卡表购电冲正这笔交易
		strcat(sql,"insert into  gdbank_log values('");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[2].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易标识：流水号
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[1].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易码：缴费0001、撤销0002、卡表购电1001、卡表购电撤销1002
        strcat(sql,"','");
        strcat(sql,"卡表购电撤销");   //交易名称：缴费、撤销、卡表购电、卡表购电撤销
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[13].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //客户标识：缴费号、客户编号（设保留字段，填写关联信息）缴费交易，用于标识客户身份的信息
        strcat(sql,"','");
        strcat(sql,"");   //客户辅助信息：（客户标识的关联信息等）(暂时为空)
        strcat(sql,"','");
        strcat(sql,"");   //客户名称：
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[3].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易日期：
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易时间：
        strcat(sql,"',");
        strcat(sql,"null");   //交易金额：
        strcat(sql,",");
        if(strcmp(kabiaogoudianchongzhengfromelecjiexi[3].value,"0000")==0)   //交易成功标识：0、1、2    0成功，1失败，2是撤销
        {
			strcat(sql,"0");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
            strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易成功标识：0、1、2    0成功，1失败，2是撤销
            strcat(sql,",'");
            strcat(sql,"失败");
            strcat(sql,"','");
		}

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[14].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //原交易流水号：缴费交易时为空
        strcat(sql,"',");


        if(strcmp(kabiaogoudianchongzhengfromelecjiexi[3].value,"0000")==0)   //交易状态：0、1    0成功，1失败
        {
			strcat(sql,"0");   //交易状态：0、1    0成功，1失败
            strcat(sql,",'");
            strcat(sql,"成功");
            strcat(sql,"','");
		}
		else
		{
			strcat(sql,"1");   //交易状态：0、1    0成功，1失败
            strcat(sql,",'");
            strcat(sql,"失败");
            strcat(sql,"','");
		}


		DeleteTailSpace(kabiaogoudianchongzhengfromelecjiexi[4].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //返回信息：？
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[5].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //区域编码：
        strcat(sql,"','");
        for(i=0;i<DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //区域名称：
        {
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[5].value,dqdmqdsbhwddmczybhmyyingshepeizhi[i].diqudaima))
			{

                DeleteTailSpace(dqdmqdsbhwddmczybhmyyingshepeizhi[i].diquname,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //区域名称：
                strcat(sql,"','");
                break;
			}
		}
		if(i==DQDM_QDSBH_WDDM_CZYBH_MY_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //区域名称：
            strcat(sql,"','");
		}

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[6].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易渠道编码：
        strcat(sql,"','");


        for(i=0;i<JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG;i++) //交易渠道名称:
        {
			if(!strcmp(kabiaogoudianchongzhengfromyinhangjiexi[6].value,jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbm))
			{

                DeleteTailSpace(jyqdbmjyqdbmmcyingshepeizhi[i].jyqdbmmc,deletetailspacechar);
				strcat(sql,deletetailspacechar);   //交易渠道名称:
                strcat(sql,"','");
                break;
			}
		}
		if(i==JYQDBM_JYQDBMMC_YINGSHENG_ZIDUAN_REAL_SHULIANG)
		{
			strcat(sql,"");   //交易渠道名称:
            strcat(sql,"','");
		}

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[7].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //交易机构：
        strcat(sql,"','");
        strcat(sql,"");   //交易机构名称：?????
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[8].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //操作员号
        strcat(sql,"','");

		DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[9].value,deletetailspacechar);
        strcat(sql,deletetailspacechar);   //收费单位类别编码：
        strcat(sql,"','");
        strcat(sql,"电费缴费");   //收费单位类别名称：公共事业类缴费、物业类、学费类、其它类
        strcat(sql,"','");
        strcat(sql,"095598");   //收费单位一级机构编码：一级机构编码
        strcat(sql,"','");
        strcat(sql,"电力");   //收费单位一级机构名称：
        strcat(sql,"','");
        strcat(sql,"000");   //收费单位二级机构编码：
        strcat(sql,"','");
        strcat(sql,"电力");   //收费单位二级机构名称：
        strcat(sql,"',");
         strcat(sql,"0");   //删除标记   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,",'");

        strcat(sql,"未删除");   //删除标记位详情   0--未删除   1--已删除     标识此记录是否删除
        strcat(sql,"',");

        strcat(sql,"0");   //修改标记   数字0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,",'");

        strcat(sql,"未修改");   //修改标记位详情   0--未修改   1---修改一次  2--修改两次  3--修改三次
        strcat(sql,"',");

        strcat(sql,"0");   //对账结果   0--未对账1---对账成功2---记录有冲突3-对账文件不存在本记录
        strcat(sql,",'");

        strcat(sql,"未对账");   //对账结果详情  0--未对账   1-对账成功  2---记录有冲突  3-对账文件不存在本记录
        strcat(sql,"','");

        strcat(sql,"");   //备注
        strcat(sql,"','");

        strcat(sql,"");   //保留1
        strcat(sql,"','");

        strcat(sql,"");   //保留2
        strcat(sql,"','");

        strcat(sql,"");   //保留3
        strcat(sql,"');");
       // sprintf(STRING_FOR_RECORD+strlen(STRING_FOR_RECORD),"sql--------------%s\n",sql);
        save_query(sql,0);
		//修改被撤销的缴费交易
		memset(sql,'\0',BUFFER_SIZE);
		if(strcmp(kabiaogoudianchongzhengfromelecjiexi[3].value,"0000")==0)   //撤销成功，可以修改缴费交易
        {
			strcat(sql,"UPDATE gdbank_log SET jycgbs=2 WHERE jybs='" );

		    DeleteTailSpace(kabiaogoudianchongzhengfromyinhangjiexi[14].value,deletetailspacechar);
			strcat(sql,deletetailspacechar);
            strcat(sql,"';");
            save_query(sql,1);
		}
     }
     else
     {

     }
 }

//去除尾部空格的函数 yuan---需要去除尾部空格的字符串    result---去除尾部空格之后的字符串
 void DeleteTailSpace(char *yuan,char *result)
 {
	memset(result,'\0',sizeof(result));
	int i,j;
	int k=strlen(yuan);
	for(i=k-1;i>=0;i--)
    {
        if(yuan[i]!=' ')
        {
            break;
        }
    }
    for(j=0;j<=i;j++)
    {
        result[j]=yuan[j];
    }
    result[j]='\0';
    return;
 }
 //将去除尾部空格，含有前导0的以元为单位的金额变为去除前导0的以分为单位的金额
 void yuantofen(char *result)
 {
     int n=strlen(result);
     char jinetemp[20];
     int i,j;
     int k=0;
     memset(jinetemp,'\0',20);
		for(j=0;j<n;j++)
		{
			if(result[j]=='0')
			{
				continue;
			}
			else
			{
				break;
			}
		}
		if(result[j]=='.')
		{
			if(result[j+1]=='0')
			{
				jinetemp[0]=result[j+2];
			}
			else
			{
				jinetemp[0]=result[j+1];
				jinetemp[1]=result[j+2];
			}
		}
		else
		{
			for(;j<n;j++)
			{
				if(result[j]=='.')
				{
					continue;
				}
				else
				{
					jinetemp[k++]=result[j];
				}
			}
		}
		n=strlen(jinetemp);
		memset(result,'\0',strlen(result));
		for(i=0;i<n;i++)
        {
            result[i]=jinetemp[i];
        }

 }
int zerototenchange(char *temp)
{
	if(!strcmp(temp,"01"))
	{
		return 0;
	}
	else if(!strcmp(temp,"02"))
	{
		return 1;
	}
	else if(!strcmp(temp,"03"))
	{
		return 2;
	}
	else if(!strcmp(temp,"04"))
	{
		return 3;
	}
	else if(!strcmp(temp,"05"))
	{
		return 4;
	}
	else if(!strcmp(temp,"06"))
	{
		return 5;
	}
	else if(!strcmp(temp,"07"))
	{
		return 6;
	}
	else if(!strcmp(temp,"08"))
	{
		return 7;
	}
	else if(!strcmp(temp,"09"))
	{
		return 8;
	}
	else if(!strcmp(temp,"10"))
	{
		return 9;
	}
	else
	{
		return -1;
	}

}

/*char *utf8converttogb2312(char *s)
{
	int inlen,outlen;
	char temp[100];  //汉字应该不会太多。。有待改善
	memset(temp,'\0',BUFFER_SIZE);
	inlen=strlen(s);
	outlen=100;
	code_convert("utf-8","gb2312",s,inlen,temp,outlen);
	return temp;
}*/
/*void OutputRedirect()
{
	time_t rawtime;
	struct tm* timeinfo;
	char timE[80];
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(timE,80,"%Y%m%drecord.txt",timeinfo);
	char recordpath[100];
	memset(recordpath,'\0',100);
	strcpy(recordpath,RECORD_PATH);
	strcat(recordpath,timE);
	freopen(recordpath, "at", stdout);
	return;
}
*/
/*
 *
 * 1.将传来的01-10转换为对应的数字
 * 2.将套餐明细进行排序，计算每一个明细时，统计套餐编号小于它的数量，记录在数组相应的位置，
 * 3.最后将数组里的值跟步骤1的结果比对，返回数组对应下标
 *
 *
 *
 *
 *
 *
 */
int SelectTaoCanNo(char *s)
{
	int count,i,j;
	int jilushu;
	int sort[100];
	int countxiaoyu;//统计套餐编号小于它的数量
	jilushu=stringtoint(guangdiansendtaocanmulujiexi[4].value,0,3);
	count=zerototenchange(s);
	//printf("%d,%d\n",jilushu,count);
	i=0;
	j=0;
	for(i=0;i<jilushu;i++)
	{
		countxiaoyu=0;
		for(j=0;j<jilushu;j++)
		{
			if(strcmp(guangdiansendtaocanmulujiexitaocanmingxi[j].value[0],guangdiansendtaocanmulujiexitaocanmingxi[i].value[0])<0)
			{
				countxiaoyu++;
			}
		}
		sort[i]=countxiaoyu;
	}
	
	for(i=0;i<jilushu;i++)
	{
		//printf("%d\n",sort[i]);
	}
	if(count>jilushu-1)
	{
		return -1;
	}
	else
	{
		for(i=0;i<jilushu;i++)
		{
			if(count==sort[i])
			{
				//printf("selectnumber:%d\n",i);
				return i;
			}
		}
		return -1;
	}
	

}

void SortTaoCanNo(int sort[])
{
	int i,j;
	int jilushu;
	int countxiaoyu;//统计套餐编号小于它的数量
	int sorttemp[100];
	jilushu=stringtoint(guangdiansendtaocanmulujiexi[4].value,0,3);
	//printf("%d,%d\n",jilushu,count);
	i=0;
	j=0;
	for(i=0;i<jilushu;i++)
	{
		countxiaoyu=0;
		for(j=0;j<jilushu;j++)
		{
			if(strcmp(guangdiansendtaocanmulujiexitaocanmingxi[j].value[0],guangdiansendtaocanmulujiexitaocanmingxi[i].value[0])<0)
			{
				countxiaoyu++;
			}
		}
		sorttemp[i]=countxiaoyu;
	}
	
	for(i=0;i<jilushu;i++)
	{
		for(j=0;j<jilushu;j++)
		{
			if(i==sorttemp[j])
			{
				sort[i] = j;
				break;
			}
		}
	}
/*	for(i=0;i<jilushu;i++)
	{
		printf("%d\n",sort[i]);
	}*/
	return;
}
