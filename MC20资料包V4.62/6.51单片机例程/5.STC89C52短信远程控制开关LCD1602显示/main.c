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

/**********************************************************************
		 ����������
		 ���Ͷ��� OFF���ر�
		 ���Ͷ��� ON����
***********************************************************************/

#include "main.h"
#include "uart.h"
#include "LCD1602.h"

//����
#define Success 1U
#define Failure 0U

//�������
unsigned long  Time_Cont = 0;       //��ʱ��������


char xdata messageBuffer[100];
bit isTimeOut = 0;

//****************************************************
//������
//****************************************************
void main()
{

    Uart_Init();
	Init_LCD1602();
	LCD1602_write_com(0x80);
	LCD1602_write_word("ILoveMCU.       ");
	LCD1602_write_com(0x80+0x40);
	LCD1602_write_word("      taobao.com");

	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CPIN?\r\n", "READY", 1000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CREG?\r\n", ",1", 1000, 10) == Success);
	else
	{
		delay_ms(10);
		if (sendCommand("AT+CREG?\r\n", ",5", 1000, 10) == Success);
		else errorLog();
	}

	if (sendCommand("AT+CMGF=1\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CSCS=\"GSM\"\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CNMI=2,2\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog();
	delay_ms(10);



	while(1)
	{	
		if(isStartGetMessage)
		{
			isStartGetMessage = 0;
			Time_Cont = 0;
			
			while (Time_Cont < 1000)	//�ȴ�1s���ն�������
			{
				delay_ms(100);
				Time_Cont += 100;
			}
			isGetMessage = 	Success;
		}
		if( isGetMessage == Success)
		{
			parseMessage();
				
	
			if(strstr(messageBuffer, "ON") != NULL)	//��
			{
			  	relay1 = RELAY_ON;

				LCD1602_write_com(0x01); //����

				LCD1602_write_com(0x80);
				LCD1602_write_word(messageBuffer);
			}
	
	
			if(strstr(messageBuffer, "OFF") != NULL)//�ر�
			{
				relay1 = RELAY_OFF;

				LCD1602_write_com(0x01); //����

				LCD1602_write_com(0x80);
				LCD1602_write_word(messageBuffer);
			}
			isGetMessage = Failure	;
		}
	}
}

void parseMessage()
{
	char *messageHead=NULL;
	char *messageEnd1=NULL;
	char *messageEnd2=NULL;

	memset(messageBuffer, 0, 100); 	//��ն������ݻ�����
	messageHead = strstr(Rec_Buf, "CMT");
	if (messageHead != NULL)
	{
		messageEnd1 = strstr(messageHead, "\n");
		if (messageEnd1 != NULL)
		{
			messageEnd1 ++;
			messageEnd2 = strstr(messageEnd1, "\r");
			if (messageEnd2 != NULL)
			{	
				memcpy(messageBuffer, messageEnd1, messageEnd2 - messageEnd1);
	
			}
		}

	}

	CLR_Buf();
}






void errorLog()
{
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK", 100, 10) == Success)
		{
			soft_reset();
		}
		delay_ms(200);
	}
}

void soft_reset(void)	 //������������
{
   ((void (code *) (void)) 0x0000) ();
}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		SendString(Command); 		//����GPRSָ��

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(Rec_Buf, Response) != NULL)
			{
				
				CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	
	CLR_Buf();
	return Failure;
}

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void delay_ms(unsigned int n)
{
	unsigned int  i,j;
		for(i=0;i<n;i++)
			for(j=0;j<123;j++);
}