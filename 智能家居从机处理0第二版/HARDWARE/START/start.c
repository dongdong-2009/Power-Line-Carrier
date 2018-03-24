#include "start.h"
#include "dht11.h"
#include "delay.h"
#include "oled.h"

void Humiture_Initialize()    //��ʪ�ȴ��������
{
		while(DHT11_Init_Indoor())	//���������ʪ��ģ���Ƿ����
	{
		OLED_ShowString(30,12,"DHT11 Error",12);
		OLED_Refresh_Gram();
		delay_ms(200);
    OLED_ShowString(30,12,"DHT11 Correct",12);
 		delay_ms(200);
		OLED_Refresh_Gram();    //����OLED��ʾ
		OLED_Clear();           //���OLED��ʾ
		
		while(DHT11_Init_Outdoor())	//���������ʪ�ȴ������Ƿ����	
	{
		OLED_ShowString(30,24,"DHT11 Error",12);
		OLED_Refresh_Gram();
		delay_ms(200);
    OLED_ShowString(30,24,"DHT11 Correct",12);
 		delay_ms(200);
		OLED_Refresh_Gram();    //����OLED��ʾ
		OLED_Clear();           //���OLED��ʾ
	}
	}
}