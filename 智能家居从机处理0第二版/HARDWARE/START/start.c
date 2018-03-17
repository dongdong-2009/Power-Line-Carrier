#include "start.h"
#include "dht11.h"
#include "delay.h"
#include "oled.h"

void Humiture_Initialize()    //����Ƿ�����ʪ��ģ�����
{
	while(DHT11_Init_Indoor())	//DHT11��ʼ��	
	{
		OLED_ShowString(30,12,"DHT11 Error",12);
		OLED_Refresh_Gram();
		delay_ms(200);
    OLED_ShowString(30,12,"DHT11 Correct",12);
 		delay_ms(200);
		OLED_Refresh_Gram();    //����OLED��ʾ
		OLED_Clear();           //���OLED��ʾ
		
		while(DHT11_Init_Outdoor())	//DHT11��ʼ��	
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