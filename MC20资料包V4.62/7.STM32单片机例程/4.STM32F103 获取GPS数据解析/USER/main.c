/**********************************************************
		���ߣ����زر���
		���̣�ILoveMCU.taobao.com

	�����̽��������ڱ��깺��ģ����û�ʹ�ã������������̵ĵ�����Ϊ
	��Ȩ���У�����ؾ�������
	MC20 GSM/GPRS/GPS�����壺
	https://item.taobao.com/item.htm?id=562661881042
	����ʹ��STM32��Сϵͳ��
	https://item.taobao.com/item.htm?id=523336310868
	USB-TTLģ�飺
	https://item.taobao.com/item.htm?id=39481188174
	�ƶ����Ϲ�����﮵����װ��
	https://item.taobao.com/item.htm?id=530904849115
	
	����˵����
	STM32					MC20
	3.3V	------>	V_IO
	GND		<----->	GND
	PA3		<------	GSM_TX
	PA2		------>	GSM_RX
	

	//���ڹۿ�GPS�Ľ������
	STM32					USB-TTLģ��
	GND		------>	GND
	TX1/PA9		------>	RXD
***********************************************************/



#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usart2.h"


//����
#define Success 1U
#define Failure 0U

//�������
unsigned long  Time_Cont = 0;       //��ʱ��������
unsigned int count = 0;

char phoneNumber[] = "173xxxxxxx1";		//�滻����Ҫ������绰�ĺ���
char msg[] = "ILoveMCU.taobao.com";		//��������	

char TCPServer[] = "103.44.145.208";		//TCP��������ַ
char Port[] = "10380";						//�˿�	


void errorLog(int num);
void phone(char *number);
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
void Sys_Soft_Reset(void);
void sendMessage(char *number,char *msg);
void parseGpsBuffer(void);
void printGpsBuffer(void);

int main(void)
{	
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	USART2_Init(9600);	//����2������9600
	u2_printf("AT\r\n");
	Init_LEDpin();
	
	
	
	printf("Welcome to use!\r\n");
	printf("ILoveMcu.taobao.com!\r\n");
	
	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(1);
	delay_ms(10);
	
////Ϊ�˲��忨����ʹ��GPS��������������ָ������SIM��ʱ������ͷ�
// 	if (sendCommand("AT+CPIN?\r\n", "READY", 3000, 10) == Success);
// 	else errorLog(3);
// 	delay_ms(10);

// 	if (sendCommand("AT+CREG?\r\n", ",1", 3000, 10) == Success);	//����SIM��
// 	else if(sendCommand("AT+CREG?\r\n", ",5", 3000, 10) == Success	);//����SIM��
// 	else	errorLog(3);
// 	delay_ms(10);

	if (sendCommand("AT+QGNSSC?\r\n", "+QGNSSC: 1", 5000, 1) == Success);
	else if (sendCommand("AT+QGNSSC=1\r\n", "OK\r\n", 5000, 10) == Success);
	else errorLog(5);

	clrStruct();
	while(1)
	{
		if (sendCommand("AT+QGNSSRD=\"NMEA/RMC\"\r\n", "OK\r\n", 2000, 10) == Success);
		else errorLog(6);

		Save_Data.isGetData = true;
		memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
		memcpy(Save_Data.GPS_Buffer, USART2_RX_BUF, point2);
		
		printf("\r\n==========Save_Data.GPS_Buffer==========\r\n");
		printf(Save_Data.GPS_Buffer); //������յ�����Ϣ
		printf("\r\n");

		
		parseGpsBuffer();
		printGpsBuffer();
			
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡUTCʱ��
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
					errorLog(2);	//��������
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
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

void sendMessage(char *number,char *msg)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog(6);

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else errorLog(7);

	memset(send_buf, 0, 20);    //���
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog(8);
}

void errorLog(int num)
{
	printf("ERROR%d\r\n",num);
	while (1)
	{
		if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
		{
			Sys_Soft_Reset();
		}
		delay_ms(200);
	}
}

void Sys_Soft_Reset(void)
{  
    SCB->AIRCR =0X05FA0000|(u32)0x04;      
}

void phone(char *number)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
	else errorLog(4);
}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	USART2_CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		u2_printf(Command); 		//����GPRSָ��
		
		printf("\r\n***************send****************\r\n");
		printf(Command);
		
		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(USART2_RX_BUF, Response) != NULL)
			{				
				printf("\r\n***************receive****************\r\n");
				printf(USART2_RX_BUF);
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	printf("\r\n***************receive****************\r\n");
	printf(USART2_RX_BUF);
	USART2_CLR_Buf();
	return Failure;
}





