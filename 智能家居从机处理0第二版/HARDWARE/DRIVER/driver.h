#ifndef __DRIVER_H
#define __DRIVER_H
#include "sys.h"
#include "stdlib.h"	



#define DRIVER_DIR_X   PGout(8) // X����ת���� 
#define DRIVER_OE_X    PGout(9) // X��ʹ�ܽ� �ߵ�ƽ��Ч
#define DRIVER_DIR_Y   PGout(6)// Y����ת���� 
#define DIRVER_OE_Y    PGout(7)// Y��ʹ�ܽ� �͵�ƽ��Ч
#define RCR_VAL_X    255  //ÿ������RCR_VAL_X+1���Σ��ж�һ�Σ����ֵ��0~255�����ô�һЩ���Խ����ж�Ƶ��
#define RCR_VAL_Y    255   //ÿ������RCR_VAL_Y+1���Σ��ж�һ�Σ����ֵ��0~255�����ô�һЩ���Խ����ж�Ƶ��

#define One_Circle_Pulse 25600 //ת��һȦ����Ҫ����������128ϸ�֣�
#define One_Circle_Distance 0.8000000       //ת��һȦ�ľ��루��ʵ�����������
#define One_Pulse_Distance One_Circle_Distance/One_Circle_Pulse //һ������ľ���



typedef enum
{
	CW = 1,//�ߵ�ƽ˳ʱ��
	CCW = 0,//�͵�ƽ��ʱ��
}DIR_Type;//���з��� 



extern long target_pos_X;//�з��ŷ��� X��Ŀ��ص�
extern long current_pos_X;//�з��ŷ��� X�ᵽ����ȷ�ص�

extern long target_pos_Y;//�з��ŷ��� Y��Ŀ��ص�
extern long current_pos_Y;//�з��ŷ��� Y�ᵽ����ȷ�ص�



void Driver_Init_X(void);//X����������ʼ��
void Locate_Rle_X(long num,u32 frequency,DIR_Type dir); //X����Զ�λ����
void Locate_Rle_X_dis(long dis,u32 frequency,DIR_Type dir); //X����Զ�λ���� ����
void Locate_Abs_X(long num,u32 frequency);//X����Զ�λ����

void Driver_Init_Y(void);//Y����������ʼ��
void Locate_Rle_Y(long num,u32 frequency,DIR_Type dir); //Y����Զ�λ����
void Locate_Rle_Y_dis(long dis,u32 frequency,DIR_Type dir); //Y����Զ�λ���� ����
void Locate_Abs_Y(long num,u32 frequency);//Y����Զ�λ����

void TIM1_OPM_RCR_Init(u16 arr,u16 psc);//TIM1_CH4 ���������+�ظ��������ܳ�ʼ��
void TIM1_Startup(u32 frequency);   //������ʱ��1

void TIM8_OPM_RCR_Init(u16 arr,u16 psc);//TIM8_CH2 ���������+�ظ��������ܳ�ʼ��
void TIM8_Startup(u32 frequency);   //������ʱ��8

#endif


