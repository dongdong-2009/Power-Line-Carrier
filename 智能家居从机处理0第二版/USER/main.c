#include "delay.h"  //��ʱ
#include "sys.h"    //�����ļ�������Ҫ��
#include "usart.h"  //����1
#include "usart3.h" //����3
#include "oled.h"   //OLED��ʾ
#include "timer.h"  //��ʱ���������ж�
#include "led.h"    //LED��ʾ
#include "rx.h"     //���������յ������ݽ��д������
#include "adc.h"    //ADC��·ͨ���ɼ�
#include "manage.h" //ADC�ɼ���Ϣ����
#include "tsensor.h"//�ڲ�����оƬ�¶���ʾ
#include "dht11.h"  //������ʪ����ʾ
#include "start.h"  //��Ҫ���ĸ���ģ�����ʼ��
#include "driver.h" //�����������
#include "stepmotor.h"  //�����Լ���������
#include "raindrop.h"  //����ʹ����Ƿ���ˮ

/*************************���Ŷ���******************************
PG8��X�᷽���������
PG9��X��ʹ�ܽ�
PA11��X��PWM���ƽ�
PG6��Y�᷽����ƽ�
PG7��Y��ʹ�ܽ�
PC9��Y��PWM������ƽ�
PG11��������ʪ�ȴ����������
PG12��������ʪ�ȴ����������
PA1���ɼ��ⲿ��ѹ��3.3V �����Ҫ�����ߵ�ѹ����Ҫ����תѹ��0-3.3V���к�������ʹ��
PA2���ɼ��ⲿ��ѹ��3.3V �����Ҫ�����ߵ�ѹ����Ҫ����תѹ��0-3.3V��
PB1��LED0
PB2��	LED1
PB5��LED2
PE5��LED3
PF9��LED4
PF10��LED5
PF11��LED6
PG1��LED7
PG2��LED8
PG3��LED9
PG4��LED10
PG5��ʹ�ü̵���ģ�⿪�Ź���
*****************************************************************/
 
 

u16 t;        //����1������������յ��ĳ���һ��
u16 len;      //����1���յ����ݵĳ���
//u8 Humiture_t=0;
u8 Humiture_Temperature_Indoor;  //�����¶�ֵ
u8 Humiture_Humidity_Indoor;     //����ʪ��ֵ
u8 Humiture_Temperature_Outdoor;  //�����¶�ֵ
u8 Humiture_Humidity_Outdoor;     //����ʪ��ֵ

u16 adcx_Voltage;         //���Ų���С��ѹ��3.3V���ڣ�
float temp_Voltage;       
short temp_Control_Chip;  //����оƬ�¶�ֵ����������λС����



int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(4800);	   //����1��ʼ��Ϊ9600
	usart3_init(115200); //����3��ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	OLED_Init();         //OLED��ʼ��
	Adc_Init();          //ADCͨ����ʼ��
	TIM7_Int_Init(899,799);  //����3���շ���ʱ�䶨ʱ��ʼ��
	Driver_Init_X();		     //X����������ʼ��
	Driver_Init_Y();		     //Y����������ʼ��
	TIM1_OPM_RCR_Init(999,72-1);   //1MHz����Ƶ��  ������+�ظ�����ģʽ��X�ᣩ
	TIM8_OPM_RCR_Init(999,72-1);   //1MHz����Ƶ��  ������+�ظ�����ģʽ��Y�ᣩ
	TIM6_Int_Init(9999,7199);   //10KHZ��������ʱ1s
	T_Adc_Init();           //����оƬ�ɼ��¶ȳ�ʼ��
	raindrop_Init();        //��δ�������ʼ��
	Humiture_Initialize();  //����ģ�����ʼ��
	OLED_ShowString(30,0, "a: 00.00" ,12);	 //�趨����оƬ�¶ȳ�ʼֵ
	OLED_ShowString(30,12,"b:  C",12);       //�����¶�
	OLED_ShowString(30,24,"c:  %",12);       //����ʪ��
	OLED_ShowString(30,36,"d:  C",12);       //�����¶�
	OLED_ShowString(30,48,"e:  %",12);       //����ʪ��
	OLED_Refresh_Gram();                     //OLED������ʾ
	
 while(1)
	{
			RX_1();                          //������������������
		  Adc_Voltage_Transition();        //���Ųɼ����ĵ�ѹ
		  Adc_Control_Chip_Temperature();	 //����оƬ�ɼ������¶�
			Adc_Humiture_Measure_Indoor();   //���ڲɼ�������ʪ��
		  Adc_Humiture_Measure_Outdoor();  //���ڲɼ�������ʪ��
      Window_Control();                //��������
			Curtain_Control();               //��������
		  Adc_Raindrop_Indoor();           //������δ��������ж��Ƿ�����ˮ�����ӻ�����оƬ���棩
		  Adc_Raindrop_outdoor();          //������δ��������ж������Ƿ����꣩		
 } 
}
