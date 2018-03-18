/*
�ֵ�����

*/

#include "queue.h"

void Queue_ini(u8 *d,u8 num,Queue *q)
{
	q->start=0;
	q->end=0;
	q->len=num;
	q->isFull=0;	//��
	q->q_data=d;
	q->data_cnt=0;
}

s8 Queue_get(s8 *d,Queue *q)
{
	if(q->end==q->start)	//Ҫô��������Ҫô��
	{
		if(q->isFull==0)	//�����п�
		{
			return 1;
		}
		else	//������
		{
			q->isFull=0;
		}
	}
	*d=q->q_data[q->start++];
	(q->data_cnt)--;
	if(q->start>=q->len)
	{
		q->start=0;
	}
	return 0;
}

s8 Queue_set(s8 d,Queue *q)
{
	if(q->isFull==1)	//����������
	{
		return 1;
	}
	q->q_data[q->end++]=d;
	(q->data_cnt)++;
	if(q->end>=q->len)
	{
		q->end=0;
	}
	if(q->start==q->end)	//˵������ǡ������
	{
		q->isFull=1;
	}
	return 0;
}

u8 Queue_cnt(Queue *q)
{
	return (q->data_cnt);
}


s8 Queue_pre_read(s8 *d,Queue *q)
{
	if(q->end==q->start)	//Ҫô��������Ҫô��
	{
		if(q->isFull==0)	//�����п�
		{
			return 1;
		}
		else ;	//������
	}
	*d=q->q_data[q->start];
	
	return 0;
}