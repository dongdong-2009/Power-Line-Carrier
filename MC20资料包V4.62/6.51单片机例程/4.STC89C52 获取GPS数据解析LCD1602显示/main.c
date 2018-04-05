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
#include "main.h"
#include "uart.h"
#include "LCD1602.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//����
#define Success 1U
#define Failure 0U
//�������
unsigned long  Time_Cont = 0;       //��ʱ��������
_SaveData Save_Data;

//****************************************************
//������
//****************************************************
void main()
{
	unsigned char i = 0;
	Uart_Init();
	delay_ms(10);
	Init_LCD1602();
	LCD1602_write_com(0x80);
	LCD1602_write_word("ILoveMCU.       ");
	LCD1602_write_com(0x80+0x40);
	LCD1602_write_word("      taobao.com");
	clrStruct();	//��ջ�������

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

	if (sendCommand("AT+QGNSSC?\r\n", "+QGNSSC: 1", 1000, 1) == Success);
	else if (sendCommand("AT+QGNSSC=1\r\n", "OK\r\n", 100, 10) == Success);
	else errorLog();

	while(1)
	{
		if (sendCommand("AT+QGNSSRD=\"NMEA/RMC\"\r\n", "OK\r\n", 2000, 10) == Success);
		else errorLog();
		
		Save_Data.isGetData = true;
		memset(Save_Data.GPS_Buffer, 0, 120);      //���
		memcpy(Save_Data.GPS_Buffer, Rec_Buf, point1);



		parseGpsBuffer();
		printGpsBuffer();
	}
}


void errorLog()
{
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
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
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	
	CLR_Buf();
	return Failure;
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
//		SendString("**************\r\n");
//		SendString(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog();	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
//						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog();	//��������
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
//		SendString("Save_Data.UTCTime = ");
//		SendString(Save_Data.UTCTime);
//		SendString("\r\n");

		if(Save_Data.isUsefull)
		{
//			Save_Data.isUsefull = false;
//			SendString("Save_Data.latitude = ");
//			SendString(Save_Data.latitude);
//			SendString("\r\n");
//
//
//			SendString("Save_Data.N_S = ");
//			SendString(Save_Data.N_S);
//			SendString("\r\n");
//
//			SendString("Save_Data.longitude = ");
//			SendString(Save_Data.longitude);
//			SendString("\r\n");
//
//			SendString("Save_Data.E_W = ");
//			SendString(Save_Data.E_W);
//			SendString("\r\n");

			LCD1602_write_com(0x80);
			LCD1602_write_word(Save_Data.latitude);
			LCD1602_write_data(' ');
			LCD1602_write_word(Save_Data.N_S);
			LCD1602_write_word("                ");
			
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_word(Save_Data.longitude);
			LCD1602_write_data(' ');
			LCD1602_write_word(Save_Data.E_W);
			LCD1602_write_word("                ");
		}
		else
		{
//			SendString("GPS DATA is not usefull!\r\n");
			LCD1602_write_com(0x80);
			LCD1602_write_word("GPS DATA is     ");
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_word("  not usefull  ");
		}
		
	}
}

void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
//	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}

//****************************************************
//MS��ʱ����
//****************************************************
void delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}