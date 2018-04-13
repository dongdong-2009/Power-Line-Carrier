#include "manage.h"
#include "adc.h"
#include "oled.h"
#include "delay.h"
#include "tsensor.h"
#include "dht11.h"
#include "led.h"
#include "raindrop.h"
#include "SFE_TSL2561.h" 

extern u16 adcx_Voltage;           //ADC1ͨ��1��ѹת��
extern float temp_Voltage;         
extern short temp_Control_Chip;               
extern u8 Humiture_Temperature_Indoor;    //�����¶�
extern u8 Humiture_Humidity_Indoor;       //����ʪ��
extern u8 Humiture_Temperature_Outdoor;   //�����¶�
extern u8 Humiture_Humidity_Outdoor;      //����ʪ��
extern unsigned int data[4];             //���LUX��ʽ����ֵ

void Adc_Voltage_Transition(void)             //��������С��3.3V��ѹʹ��    
{
		adcx_Voltage=Get_Adc_Average(ADC_Channel_1,10);
//		OLED_ShowNum(0,0,adcx_Voltage,4,12);
		temp_Voltage=(float)adcx_Voltage*(3.3/4096);  
		adcx_Voltage=temp_Voltage;
//		OLED_ShowNum(0,12,adcx_Voltage,1,12);
		temp_Voltage-=adcx_Voltage;
		temp_Voltage*=20;
		OLED_ShowNum(0,24,temp_Voltage,3,12);
	  OLED_Refresh_Gram();
	if(adcx_Voltage>1000)
		{
//			LED0=!LED0;
			delay_ms(1000);
		}  
}

void Adc_Control_Chip_Temperature(void)    //���ڲ�������оƬ�¶�
{
	temp_Control_Chip=Get_Temprate();	  //�õ�����оƬ�¶�
		if(temp_Control_Chip<0)            
		{
			temp_Control_Chip=-temp_Control_Chip; 
			OLED_ShowString(42,0,"-",12);	   //�����⵽����ֵС���㣬����ʾ����
		}else OLED_ShowString(42,0," ",12);	    //�����⵽����ֵ�����㣬����ʾ����Ϊ���ţ�
		OLED_ShowNum(48,0,temp_Control_Chip/100,2,12);		 //��ʾ��������
		OLED_ShowNum(66,0,temp_Control_Chip%100,2,12);	  //��ʾС������
		OLED_Refresh_Gram();     //OLED������ʾ
}

void Adc_Humiture_Measure_Indoor(void)     //����������ʪ��
{
	DHT11_Read_Data_Indoor(&Humiture_Temperature_Indoor,&Humiture_Humidity_Indoor);	
	OLED_ShowNum(42,12,Humiture_Temperature_Indoor,2,12);
	OLED_ShowNum(42,24,Humiture_Humidity_Indoor,2,12);
	OLED_Refresh_Gram();
}

void Adc_Humiture_Measure_Outdoor(void)     //����������ʪ��
{
	DHT11_Read_Data_Outdoor(&Humiture_Temperature_Outdoor,&Humiture_Humidity_Outdoor);	
	OLED_ShowNum(42,36,Humiture_Temperature_Outdoor,2,12);
	OLED_ShowNum(42,48,Humiture_Humidity_Outdoor,2,12);
	OLED_Refresh_Gram();
}


//void Adc_Co_Gas(void)                  //�к����庬�� ��ֱ��ʹ����ADC��ѹֱ��ת����  
//{
//	
//}  


void Adc_Raindrop_Indoor(void)             //������δ��������ж��Ƿ�����ˮ�����ӻ�����оƬ���棩
{
	if(raindrop_Indoor==0)
	{
		  OLED_ShowString(78,24,"splash",12);
		  OLED_Refresh_Gram();
	}
	else	
	  OLED_ShowString(78,24,"      ",12);
	  OLED_Refresh_Gram();
}

void Adc_Raindrop_outdoor(void)            //������δ��������ж������Ƿ����꣩
{
	if(raindrop_Outdoor==0)
	{
			OLED_ShowString(78,12,"raining",12);
		  OLED_Refresh_Gram();
	}
	else
		OLED_ShowString(78,12,"       ",12);
		OLED_Refresh_Gram();
}

void Light_Intensity(void)                //��ǿǿ�ȴ�С��W/cm^2��
{
	SFE_TSL2561_read_data0_and_data1(data);
  SFE_TSL2561_read_gain_and_intge(data+2);
  data[0]=CalculateLux(data[2],data[3],data[0],data[1],1);
  OLED_ShowNum(102,0,data[0],4,12);
}	
