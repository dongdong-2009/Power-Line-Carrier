/*********************************************************************
		 ���ߣ����زر���
		 ���̣�ILoveMCU.taobao.com

	�����̽��������ڱ��깺��ģ����û�ʹ�ã������������̵ĵ�����Ϊ
	��Ȩ���У�����ؾ�������
	MC20 GSM/GPRS/GPS�����壺
	https://item.taobao.com/item.htm?id=562661881042
	SIM800 GSM/GPRSģ��
	https://item.taobao.com/item.htm?id=521599424969
	STC89C52/STC12C5A60S2��Сϵͳ��
	https://item.taobao.com/item.htm?id=26410708738
	USB-TTLģ�飺
	https://item.taobao.com/item.htm?id=39481188174
	�ƶ����Ϲ�����﮵����װ��
	https://item.taobao.com/item.htm?id=530904849115
*********************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

#define false 0
#define true 1

//�������鳤��
#define GPS_Buffer_Length 120
//#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
//	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} xdata _SaveData;


//�������߱�������
extern void delay_ms(unsigned int n);

extern void printGpsBuffer();
extern void parseGpsBuffer();
extern void errorLog();
extern unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
extern void clrStruct();
extern void soft_reset(void);

#endif