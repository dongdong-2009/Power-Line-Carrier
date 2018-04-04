#include "rx.h"
#include "usart.h"
#include "led.h"
#include "driver.h"
#include "timer.h"

extern u16 t;     //���ڳ��ȵĵ���
extern u16 len;	  //���շ������ݵĳ���

void RX_1()       //�������ݺ���
{	
	if(USART_RX_STA&0x8000)    //��������ʹ��
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)      //��0-len����
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				
				if(USART_RX_BUF[t]==48)
				{
					LED0=0;            //LEDģ�ⷿ���ڵƵ�����
					LED1=0;
					LED2=0;
					LED3=0;
					LED4=0;
					LED5=0;
					LED6=0;
					LED7=0;
					LED8=0;
					LED9=0;
					LED10=0;
					Locate_Rle_Y(25600,50000,CCW);  //��ת25600�����壨����ϸ��ȥ����Ȧ����
				}
				else if(USART_RX_BUF[t]==49)
				{
					LED0=1;
					LED1=1;
					LED2=1;
					LED3=1;
					LED4=1;
					LED5=1;
					LED6=1;
					LED7=1;
					LED8=1;
					LED9=1;
					LED10=1;
					Locate_Rle_X(25600,50000,CCW);  //��ת25600�����壨����ϸ��ȥ����Ȧ����
				}
			  if(USART_RX_BUF[t]==50)
				{
					LED0=!LED0;
				}
				else if(USART_RX_BUF[t]==51)
				{
					LED1=!LED1;
				}
				else if(USART_RX_BUF[t]==52)
				{
					LED2=!LED2;
				}
				else if(USART_RX_BUF[t]==53)
				{
					LED3=!LED3;
				}
				else if(USART_RX_BUF[t]==54)
				{
					LED4=!LED4;
				}
				else if(USART_RX_BUF[t]==55)
				{
					LED5=!LED5;
				}
				else if(USART_RX_BUF[t]==56)
				{
					LED6=!LED6;
				}
				else if(USART_RX_BUF[t]==57)
				{
					LED7=!LED7;
				}
				else if(USART_RX_BUF[t]==97)
				{
					LED8=!LED8;  				
				}
				else if(USART_RX_BUF[t]==99)
				{
					LED9=!LED9;
				}
				else if(USART_RX_BUF[t]==100)
				{
					LED10=!LED10;
				} 
				if(USART_RX_BUF[t]==98)
				{
					front_door = 0;   //ģ����Ŵ�
				}
				if(front_door == 0)
				{
					TIM_Cmd(TIM6,ENABLE);//������ʱ��6
				}
				
				
				
				printf("  %d\r\n",USART_RX_BUF[t]);//���ڵ���ʱ�鿴�����Ƿ���������
			}
			USART_RX_STA=0;                 //�����������
    }
}	 