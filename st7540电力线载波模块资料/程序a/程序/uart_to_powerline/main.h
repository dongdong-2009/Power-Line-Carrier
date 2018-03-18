/*
�ֵ�����
*/
#ifndef MAIN_H
#define MAIN_H


#include "iostm8s103.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char s8;
typedef signed int s16;
typedef signed long s32;
typedef volatile unsigned char vu8;
typedef volatile unsigned int vu16;
typedef volatile unsigned long vu32;
typedef volatile signed char vs8;
typedef volatile signed int vs16;
typedef volatile signed long vs32;

#define DisableInt	_asm("SIM")
#define EnableInt	_asm("RIM")
#define SYS_SERVICE	_asm("TRAP")
#define LOW_POWER_WAIT	_asm("WFI")

extern u8 SYS_task;

#define START_TASK(x)       SYS_task |= x

#define TASK_CMD	            1	//����̨
#define TASK_BT		            2	//��ص�ѹ���


//ϵͳ�ṩ�ĺ���:
void wait(vu32 i);		//�ӳٺ���
void void_fun(void);	//�պ���
void err(void);			//����������ѭ����

#endif