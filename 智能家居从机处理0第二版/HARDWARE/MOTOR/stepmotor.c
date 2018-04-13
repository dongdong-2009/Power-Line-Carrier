#include "stepmotor.h"
#include "delay.h"
#include "manage.h"
#include "driver.h"
#include "led.h"
#include "window.h"
#include "SFE_TSL2561.h" 
#include "raindrop.h"  
#include "tsensor.h"
#include "adc.h"
#include "dht11.h"

extern u8 Humiture_Temperature_Indoor;    //�����¶�
extern u8 Humiture_Humidity_Indoor;       //����ʪ��
extern u8 Humiture_Temperature_Outdoor;   //�����¶�
extern u8 Humiture_Humidity_Outdoor;      //����ʪ��

void Window_Control()  //ģ�ⴰ������
{
	if(Humiture_Temperature_Outdoor-5>Humiture_Temperature_Indoor)
	{
		Locate_Abs_X(0,50000);
		Locate_Abs_Y(0,50000);
		Locate_Rle_Y(25600,50000,CCW);
		Locate_Rle_X(25600,50000,CW);
	}
//	if(windows_Right==0)
//	{
//		LED7=!LED7;
//	}
}


void Curtain_Control() //ģ�ⴰ������
{
	
}



