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

//����
#define Success 1U
#define Failure 0U

//�������
unsigned long  Time_Cont = 0;       //��ʱ��������

unsigned int count = 0;

char phoneNumber[] = "173xxxxxxx1";		//�滻����Ҫ������绰�ĺ���
char msg[] = "ILoveMCU.taobao.com";		//��������

code char TCPServer[] = "103.44.145.245";		//TCP��������ַ
code char Port[] = "10380";						//�˿�	

//****************************************************
//������
//****************************************************
void main()
{

    Uart_Init();						  

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



	while(1)
	{	
		xdata char send_buf[100] = {0};
		memset(send_buf, 0, 100);    //���
		strcpy(send_buf, "AT+QIOPEN=\"TCP\",\"");
		strcat(send_buf, TCPServer);
		strcat(send_buf, "\",\"");
		strcat(send_buf, Port);
		strcat(send_buf, "\"\r\n");
		if (sendCommand(send_buf, "CONNECT OK", 10000, 5) == Success);
		else errorLog();
		delay_ms(100);
	
		while(1)
		{
			//��������
			if (sendCommand("AT+QISEND\r\n", ">", 3000, 5) == Success);
			else errorLog();
			delay_ms(10);
	
			memset(send_buf, 0, 100);    //���
			sprintf(send_buf,"ILoveMCU.taobao.com %d\r\n",count);
			count++;
			
	
			if (sendCommand(send_buf, send_buf, 3000, 1) == Success);
			else errorLog();
			delay_ms(10);
	
			SendData(0x1a);//��������
			delay_ms(500);
	
		}
	
	
		if (sendCommand("AT+QICLOSE\r\n", "OK\r\n", 5000, 10) == Success);
		else errorLog();
		delay_ms(100);
	
		if (sendCommand("AT+QIDEACT\r\n", "OK\r\n", 5000, 10) == Success);
		else errorLog();
	
	
		delay_ms(1000);	
	}
}

void sendMessage(char *number,char *msg)
{
	xdata char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog();

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else errorLog();

	memset(send_buf, 0, 40);    //���
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog();
}

void phone(char *number)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
	else errorLog();
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