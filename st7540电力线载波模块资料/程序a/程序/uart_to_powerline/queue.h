/*
�ļ�����queue.h
ʱ�䣺2011-1-22

���ܣ�

*/
#ifndef QUEUE_H
#define QUEUE_H

#include "main.h"

typedef struct
{
	u8 start;
	u8 end;
	u8 len;
	u8 *q_data;
	u8 isFull;	//Ϊ1��־������,Ϊ0��ʾû����������ʱ���ǿգ�start��endһ������
	u8 data_cnt;
} Queue;

void Queue_ini(u8 *d,u8 num,Queue *q);

//�ɹ�����0��ʧ�ܷ��ش����
s8 Queue_set(s8 d,Queue *q);	
s8 Queue_get(s8 *d,Queue *q);
s8 Queue_pre_read(s8 *d,Queue *q);
u8 Queue_cnt(Queue *q);


#endif