/*
�ֵ�����
	
*/

#include "bt.h"
#include "STimer.h"

#define AD_CHANNEL	2

#define LED_MIN		5

u16 LED_t=100;
u16 LED_tick=0;
u8 isBeep=0;

float ad_value=0;//adԭʼֵ

float AD_K=70.523416f;//����
float votage_lim=12;		//��������


static STimer timer;

void bt_ini(void)
{
//AD����
	PD_DDR &= ~BIT3;//����
	PD_CR1 &= ~BIT3;//float
	PD_CR2 &= ~BIT3;//���ж�
	
	ADC_CR1 = 0x72; //Ԥ��ƵfADC = fMASTER/18������ת��ģʽ
	ADC_CR2 = 0x00; //���������
	
	ADC_CSR = AD_CHANNEL; //ѡ��AINx��Ϊ����ͨ��
	ADC_CR1 |= 0x01; //����ADC
	//����ǵ�һ������ADC������Ҫ�ȴ�ʱ��>7us��ȷ��ADC��Դ�ȶ�
	ADC_CR1 |= 0x01; //ADON=1,����ADC
//��ȡ����
	if(*(float*)EE_AD_K)
	{
		AD_K=*(float*)EE_AD_K;
	}
	if(*(float*)EE_votage_lim)
	{
		votage_lim=*(float*)EE_votage_lim;
	}
//��ʱ������
	timer.time=100;//100��tick
	timer.cb=bt_pro;
	timer.p=0;
	STimer_start(&timer);
}

void bt_pro(void)//61Hz
{
	static int time=0;
	u16 ad;

	time++;

	while(!(ADC_CSR & 0x80));
	ad=((((unsigned int)ADC_DRH)<<2)+ADC_DRL);
	
	ADC_CSR =AD_CHANNEL; //ѡ��AIN3��Ϊ����ͨ��
	ADC_CR1 |= 0x01; //����ADC
	//����ǵ�һ������ADC������Ҫ�ȴ�ʱ��>7us��ȷ��ADC��Դ�ȶ�
	ADC_CR1 |= 0x01; //ADON=1,����ADC
	
	if(ad<1024)
	{
		ad_value= ad_value*0.95f+ad*0.05f;
	}
//PC_ODR ^= LED_P;

	LED_tick++;
	if(LED_tick>LED_t)
	{
		LED_tick=0;
		//turn the led
		LED_ON;
		BEEP_OFF;
	}
	else if(LED_tick>=LED_MIN)
	{
		if(isBeep)
		{
			BEEP_ON;
		}
		else
		{
			LED_OFF;
		}
	}

	if(time%30==0)
	{
		if(AD_K==0)
		{
			printf("asdf\r\n");
		}
		ad=(u16)(votage_lim*AD_K);
		if(ad_value<ad)
		{
			LED_t=LED_MIN;
			//beep
			isBeep=1;
		}
		else
		{
			isBeep=0;
			LED_t=LED_MIN+(ad_value-ad)*2;
		}
	}
	//���ö�ʱ��
	timer.time=1;//5
}
