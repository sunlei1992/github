#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
#include <errno.h>
void vCat_MACChar_bank(char *pMacChar, char *pYData, int length);
void str_xor(char *instr,char *outstr);
void str_to_hex(unsigned char	*str, unsigned char	*hex,int len);
int asc2hex(unsigned char dat, unsigned char *new);
int esc_str2hex(unsigned char *read, unsigned char *write, int len);
int Md5_mac(char *send,char *sMac_hex,char *miyao)
{	
	int iRc=0;
	char sOutput[2048];
	char sTmp[2048];
	char sKey[16+1];
	char sMab[16+1];
	char sMac[2048+1];
	char sMacHex[16+1];
	
	memset(sOutput,0x00,sizeof(sOutput));
	memset(sTmp,0x00,sizeof(sTmp));
	memset(sKey,0x00,sizeof(sKey));
	memset(sMab,0x00,sizeof(sMab));
	memset(sMac,0x00,sizeof(sMac));
	memset(sMacHex,0x00,sizeof(sMacHex));
	memset(sMac_hex,0x00,sizeof(sMac_hex));
	
	strcpy(sOutput,send);
//printf("send---%s\n",sOutput);
	vCat_MACChar_bank(sMac,sOutput,strlen(sOutput));/*����mac��*/
	if(strlen(sMac)<=0)
		{
		printf("����mac��������");
		return -1;	
		}
		
	memset(sOutput,0,sizeof(sOutput));
	//printf("chuan=[%s]\n", sMac );
	MD5ENC(sMac,sOutput,strlen(sMac));/*MD5������������ժҪ*/
	//printf("md5=[%s]\n", sOutput );
	str_to_hex(sOutput,sTmp,16);
	//printf("��ӡmd5 16����=[%s]\n", sTmp );
	
	memcpy(sKey,miyao,16);
	//printf("��Կ��[%s]", sKey );
	esc_str2hex(sKey,sKey,16);/*ȡ������Կ*/
	//printf("ת��[%s]\n", sKey);
	/*writelog2(LOG_DEBUG,sKey,strlen(sKey),"[%s],[%d], sKey:",__FILE__,__LINE__);*/
	
	memset(sMac, 0x00, sizeof(sMac) );
	str_xor(sOutput,sMac);/*�����16λ�ַ����ĸ�8�ֽں͵�8�ֽ�*/
	encry(sKey,sMac);
	
	str_to_hex(sMac,sMac_hex,8);
	memcpy(sMacHex,sMac_hex,16);/*sMac_hexΪ������ɵ�mac*/
	
	//printf("���ɵ�macΪ[%s]=[%s]\n",sMac_hex, sMacHex);
	
}
/****MAC�ַ�����****************************************
char * pMacChar  ====���ɵ�MAC��
char *pYData     ====������
int length       ====�����ݳ���
********************************************************/
void vCat_MACChar_bank(char *pMacChar, char *pYData, int length)
{
  int nowlen,i;
  nowlen=strlen(pMacChar);
  
  if(nowlen >0)
  {
    pMacChar[nowlen]=' '; /* �������֮�����һ���ո� */
    nowlen++; 
  } 
  
  /****ȥ���ֶο�ʼ�Ŀո�****/
  for(i=0; i<length; i++)
  {
      if(pYData[i]!=' ')  break;
  }
    
  /****MAC�ַ��Ĵ���****/
  for(;i<length;i++)
  {
    if((pYData[i]>='0' && pYData[i]<='9') ||
       (pYData[i]>='a' && pYData[i]<='z') ||
       (pYData[i]>='A' && pYData[i]<='Z') ||
        pYData[i]==',' || pYData[i]==' '  || pYData[i]=='.')
    {
      /****�Ϸ��ַ�****/
      char ch=pYData[i];      
      if(ch>='a' && ch<='z')/****Сд���д****/
      {
        ch=ch-'a'+'A';
      }
      /****���������Ŀո�****/
      if(ch==' ' &&  nowlen>0 && pMacChar[nowlen-1]==' ')
      {
        continue;
      }     
      pMacChar[nowlen]=ch;  /****����һ���Ϸ��ַ�****/
      nowlen++;
    }
  }
  
  /****ɾ��β���ո�****/
  while(nowlen>0)
  {
    if(pMacChar[nowlen-1]==' ')
    {
      pMacChar[nowlen-1]='\0';
      nowlen--;
    }
    else
    {
      break;
    }
  }
  
}

/*******************************************************************************
*** �������� :  str_xor
*** �������� :  �����16λ�ַ����ĸ�8�ֽں͵�8�ֽ�
*** ��    �� :  instr   16λ�ַ���
*** ��    �� :  outstr  ������8λ�ַ���
*** �� �� ֵ :  ��
*******************************************************************************/
void str_xor(char *instr,char *outstr)
{
	unsigned char *pin=NULL;
	unsigned char *pout=NULL;
	
	if(instr==NULL ||outstr==NULL )
	{
		/*writelog(LOG_ERR,"[%s],[%d]�����������!",__FILE__,__LINE__);*/
		return;
	}
	
	pin=instr;
	pout=outstr;
	
	pout[0]=pin[0]^pin[8+0];
	pout[1]=pin[1]^pin[8+1];
	pout[2]=pin[2]^pin[8+2];
	pout[3]=pin[3]^pin[8+3];
	pout[4]=pin[4]^pin[8+4];
	pout[5]=pin[5]^pin[8+5];
	pout[6]=pin[6]^pin[8+6];
	pout[7]=pin[7]^pin[8+7];
}

int asc2hex(unsigned char dat, unsigned char *new)
{
    if ((dat >= '0') && (dat <= '9'))
        *new = dat - '0';
    else if ((dat >= 'A') && (dat <= 'F'))
        *new = dat - 'A' + 10;
    else if ((dat >= 'a') && (dat <= 'f'))
        *new = dat - 'a' + 10;
    else
        return -1;
    return 0;
}

int esc_str2hex(unsigned char *read, unsigned char *write, int len)
{
    unsigned char dat;

    while (len--)
    {
        if (asc2hex(*read++, &dat))
            return -1;   
        *write = *write << 4 | dat;
        if (!(len & 1))
        {
            ++write;
        }
    }
    return 0;
}


/*
* ���ܣ���ʽ�ַ�����16������ʽ
*/
void str_to_hex(unsigned char	*str, unsigned char	*hex,int len)
{
	int i=0;
	char tmp[3];
	
	sprintf((char*)hex,"%02X",str[0]);
	for(i=1;i<len;i++)
	{
		memset(tmp,0,sizeof(tmp));
		sprintf((char*)tmp,"%02X",str[i]);
		strncat((char*)hex,tmp,2);
	}
	hex[2*len]=0;
}
