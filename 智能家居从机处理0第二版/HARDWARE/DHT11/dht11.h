#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

 
//IO��������
#define DHT11_IO_IN_Indoor()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
#define DHT11_IO_OUT_Indoor() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}

#define DHT11_IO_IN_Outdoor()  {GPIOG->CRH&=0XFFF0FFFF;GPIOG->CRH|=8<<16;}  
#define DHT11_IO_OUT_Outdoor() {GPIOG->CRH&=0XFFF0FFFF;GPIOG->CRH|=3<<16;}

////IO��������											   
#define	DHT11_DQ_OUT_Indoor PGout(11) //���ݶ˿�	PA0 
#define	DHT11_DQ_IN_Indoor  PGin(11)  //���ݶ˿�	PA0 

#define	DHT11_DQ_OUT_Outdoor PGout(12) //���ݶ˿�	PA0 
#define	DHT11_DQ_IN_Outdoor  PGin(12)  //���ݶ˿�	PA0 


u8 DHT11_Init_Indoor(void);//��ʼ��DHT11_1
u8 DHT11_Read_Data_Indoor(u8 *temp,u8 *humi);//��ȡ��ʪ��_1
u8 DHT11_Read_Byte_Indoor(void);//����һ���ֽ�_1
u8 DHT11_Read_Bit_Indoor(void);//����һ��λ_1
u8 DHT11_Check_Indoor(void);//����Ƿ����DHT11_1
void DHT11_Rst_Indoor(void);//��λDHT11_1   

u8 DHT11_Init_Outdoor(void);//��ʼ��DHT11_2
u8 DHT11_Read_Data_Outdoor(u8 *temp,u8 *humi);//��ȡ��ʪ��_2
u8 DHT11_Read_Byte_Outdoor(void);//����һ���ֽ�_2
u8 DHT11_Read_Bit_Outdoor(void);//����һ��λ_2
u8 DHT11_Check_Outdoor(void);//����Ƿ����DHT11_2
void DHT11_Rst_Outdoor(void);//��λDHT11_2
#endif















