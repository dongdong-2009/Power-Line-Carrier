#include "dht11.h"
#include "delay.h"

      
//��λDHT11
void DHT11_Rst_Indoor(void)	   
{                 
	  DHT11_IO_OUT_Indoor(); 	//SET OUTPUT
    DHT11_DQ_OUT_Indoor=0; 	//����DQ
    delay_ms(20);    	//��������18ms
    DHT11_DQ_OUT_Indoor=1; 	//DQ=1 
	delay_us(30);     	//��������20~40us
}

//��λDHT11
void DHT11_Rst_Outdoor(void)	   
{                 
	  DHT11_IO_OUT_Outdoor(); 	//SET OUTPUT
    DHT11_DQ_OUT_Outdoor=0; 	//����DQ
    delay_ms(20);    	//��������18ms
    DHT11_DQ_OUT_Outdoor=1; 	//DQ=1 
	delay_us(30);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check_Indoor(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN_Indoor();//SET INPUT	 
    while (DHT11_DQ_IN_Indoor&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN_Indoor&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check_Outdoor(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN_Outdoor();//SET INPUT	 
    while (DHT11_DQ_IN_Outdoor&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN_Outdoor&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit_Indoor(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN_Indoor&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN_Indoor&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN_Indoor)return 1;
	else return 0;		   
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit_Outdoor(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN_Outdoor&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN_Outdoor&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN_Outdoor)return 1;
	else return 0;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte_Indoor(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit_Indoor();
    }						    
    return dat;
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte_Outdoor(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit_Outdoor();
    }						    
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data_Indoor(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst_Indoor();
	if(DHT11_Check_Indoor()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte_Indoor();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data_Outdoor(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst_Outdoor();
	if(DHT11_Check_Outdoor()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte_Outdoor();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init_Indoor(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PG�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PG11�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);				 //��ʼ��IO��
 	GPIO_SetBits(GPIOG,GPIO_Pin_11);						 //PG11 �����
			    
	DHT11_Rst_Indoor();  //��λDHT11
	return DHT11_Check_Indoor();//�ȴ�DHT11�Ļ�Ӧ
} 

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init_Outdoor(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PG�˿�ʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PG11�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);				 //��ʼ��IO��
 	GPIO_SetBits(GPIOG,GPIO_Pin_12);						 //PG11 �����
			    
	DHT11_Rst_Outdoor();  //��λDHT11
	return DHT11_Check_Outdoor();//�ȴ�DHT11�Ļ�Ӧ
} 







