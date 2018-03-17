#include "driver.h"
#include "delay.h"
#include "usart.h"

/*************�����Լ��������*****************************/

u8 rcr_remainder_Y;        //Y���ظ�������������
u8 is_rcr_finish_Y=1;      //Y���ظ��������Ƿ��������
int finish_Y=0;
long rcr_integer_Y;	       //Y���ظ�������������
long target_pos_Y=0;       //Y���з��ŷ���
long current_pos_Y=0;      //Y���з��ŷ���
DIR_Type motor_dir_Y=CW;   //Y��˳ʱ��


/************** �����������ź��߳�ʼ�� ****************/
void Driver_Init_Y(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	//ʹ��PG�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PG6��7 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					//�����趨������ʼ��GPIOG
	GPIO_SetBits(GPIOG,GPIO_Pin_6);						 	//PG6����� ˳ʱ�뷽��  DRIVER_DIR_Y
	GPIO_SetBits(GPIOG,GPIO_Pin_7);						//PG7����� ʹ�����  DRIVER_OE_Y
}

/***********************************************
//TIM8_CH2(PC7) ���������+�ظ��������ܳ�ʼ��
//TIM8 ʱ��Ƶ�� 72MHz
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
************************************************/
void TIM8_OPM_RCR_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //TIM8ʱ��ʹ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //ʹ��GPIOC����ʱ��ʹ��	                                                                     	

  //���ø�����Ϊ�����������,���TIM8 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM8_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM8,TIM_UpdateSource_Regular); /********* ����ֻ�м��������Ϊ�����ж� ********/
	TIM_SelectOnePulseMode(TIM8,TIM_OPMode_Single);/******* ������ģʽ **********/
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����2ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /****** �Ƚ����2Nʧ�� *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��	 
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_ITConfig(TIM8, TIM_IT_Update ,ENABLE);  //TIM8   ʹ�ܻ���ʧ��ָ����TIM�ж�
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM8�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8									  
}
/******* TIM8�����жϷ������ *********/
void TIM8_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_FLAG_Update)!=RESET)//�����ж�
	{
		TIM_ClearITPendingBit(TIM8,TIM_FLAG_Update);//��������жϱ�־λ		
		if(is_rcr_finish_Y==0)//�ظ�������δ�������
		{
			if(rcr_integer_Y!=0) //�����������廹δ�������
			{
				TIM8->RCR=RCR_VAL_Y;//�����ظ�����ֵ
				rcr_integer_Y--;//����RCR_VAL_Y+1������				
			}else if(rcr_remainder_Y!=0)//������������ ��λ0
			{
				TIM8->RCR=rcr_remainder_Y-1;//������������
				rcr_remainder_Y=0;//����
				is_rcr_finish_Y=1;//�ظ��������������				
			}else goto out;   //rcr_remainder=0��ֱ���˳�			 
			TIM_GenerateEvent(TIM8,TIM_EventSource_Update);//����һ�������¼� ���³�ʼ��������
			TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE �����ʹ��	
			TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8			
			if(motor_dir_Y==CW) //�������Ϊ˳ʱ��   
				current_pos_Y+=(TIM8->RCR+1);//�����ظ�����ֵ
			else          //������Ϊ��ʱ��
				current_pos_Y-=(TIM8->RCR+1);//��ȥ�ظ�����ֵ			
		}else
		{
out:		is_rcr_finish_Y=1;//�ظ��������������
			finish_Y=1;
			TIM_CtrlPWMOutputs(TIM8,DISABLE);	//MOE ������ر�
			TIM_Cmd(TIM8, DISABLE);  //�ر�TIM8	
			
//			printf("Y�ᵱǰλ��=%fcm\r\n",current_pos_Y*One_Pulse_Distance);//��ӡ���
		}	
	}
}
/***************** ����TIM8 *****************/
void TIM8_Startup(u32 frequency)   //������ʱ��8
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM8,temp_arr);//�趨�Զ���װֵ	
	TIM_SetCompare4(TIM8,temp_arr>>1); //ƥ��ֵ2������װֵһ�룬����ռ�ձ�Ϊ50%	
	TIM_SetCounter(TIM8,0);//����������
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8
}
/********************************************
//��Զ�λ���� 
//num 0��2147483647
//frequency: 20Hz~100KHz
//dir: CW(˳ʱ�뷽��)  CCW(��ʱ�뷽��)
*********************************************/
void Locate_Rle_Y(long num,u32 frequency,DIR_Type dir) //Y����Զ�λ����
{
	if(num<=0) //��ֵС����0 ��ֱ�ӷ���
	{
		return;
	}
	if(TIM8->CR1&0x01)//��һ�����廹δ�������  ֱ�ӷ���
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		return;
	}
	motor_dir_Y=dir;//�õ�����	
	DRIVER_DIR_Y=motor_dir_Y;//���÷���
	
	if(motor_dir_Y==CW)//˳ʱ��
		target_pos_Y=current_pos_Y+num;//Ŀ��λ��
	else if(motor_dir_Y==CCW)//��ʱ��
		target_pos_Y=current_pos_Y-num;//Ŀ��λ��
	
	rcr_integer_Y=num/(RCR_VAL_Y+1);//�ظ�������������
	rcr_remainder_Y=num%(RCR_VAL_Y+1);//�ظ�������������
	is_rcr_finish_Y=0;//�ظ�������δ�������
	TIM8_Startup(frequency);//����TIM8
}

/********************************************
//��Զ�λ���� 
//num 0��2147483647
//frequency: 20Hz~100KHz
//dir: CW(˳ʱ�뷽��)  CCW(��ʱ�뷽��)
*********************************************/
void Locate_Rle_Y_dis(long dis,u32 frequency,DIR_Type dir) //Y����Զ�λ����
{
	long num;
	num = 2964*dis;
	if(num<=0) //��ֵС����0 ��ֱ�ӷ���
	{
		return;
	}
	if(TIM8->CR1&0x01)//��һ�����廹δ�������  ֱ�ӷ���
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		return;
	}
	motor_dir_Y=dir;//�õ�����	
	DRIVER_DIR_Y=motor_dir_Y;//���÷���
	
	if(motor_dir_Y==CW)//˳ʱ��
		target_pos_Y=current_pos_Y+num;//Ŀ��λ��
	else if(motor_dir_Y==CCW)//��ʱ��
		target_pos_Y=current_pos_Y-num;//Ŀ��λ��
	
	rcr_integer_Y=num/(RCR_VAL_Y+1);//�ظ�������������
	rcr_remainder_Y=num%(RCR_VAL_Y+1);//�ظ�������������
	is_rcr_finish_Y=0;//�ظ�������δ�������
	TIM8_Startup(frequency);//����TIM8
}
/********************************************
//���Զ�λ���� 
//num   -2147483648��2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs_Y(long num,u32 frequency)//Y����Զ�λ����
{
	if(TIM8->CR1&0x01)//��һ�����廹δ������� ֱ�ӷ���
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		return;
	}
	target_pos_Y=num;//����Ŀ��λ��
	if(target_pos_Y!=current_pos_Y)//Ŀ��͵�ǰλ�ò�ͬ
	{
		if(target_pos_Y>current_pos_Y)
			motor_dir_Y=CW;//˳ʱ��
		else
			motor_dir_Y=CCW;//��ʱ��
		DRIVER_DIR_Y=motor_dir_Y;//���÷���
		
		rcr_integer_Y=abs(target_pos_Y-current_pos_Y)/(RCR_VAL_Y+1);//�ظ�������������
		rcr_remainder_Y=abs(target_pos_Y-current_pos_Y)%(RCR_VAL_Y+1);//�ظ�������������
		is_rcr_finish_Y=0;//�ظ�������δ�������
		TIM8_Startup(frequency);//����TIM8
	}
}


/*************�����Լ������ұ�*****************************/

u8 rcr_remainder_X;   //X���ظ�������������
u8 is_rcr_finish_X=1; //X���ظ��������Ƿ��������
int finish_X=0;
long rcr_integer_X;	//X���ظ�������������
long target_pos_X=0;  //X���з��ŷ���
long current_pos_X=0; //X����ŷ���
DIR_Type motor_dir_X=CW;//X��˳ʱ��

/************** �����������ź��߳�ʼ�� ****************/
void Driver_Init_X(void)   
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	//ʹ��PG�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	//PG8,9 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					//�����趨������ʼ��GPIOG
	GPIO_SetBits(GPIOG,GPIO_Pin_8);						 	//PG1����� ˳ʱ�뷽��  DRIVER_DIR_X
	GPIO_SetBits(GPIOG,GPIO_Pin_9);						//PG2����� ʹ�����  DRIVER_OE_X
}

/***********************************************
//TIM_CH4(PA11) ���������+�ظ��������ܳ�ʼ��
//TIM1 ʱ��Ƶ�� 72MHz
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
************************************************/
void TIM1_OPM_RCR_Init(u16 arr,u16 psc)
{		 					
 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //TIM1ʱ��ʹ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIOA����ʱ��ʹ��	                                                                     	

  //���ø�����Ϊ�����������,���TIM1 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Regular); /********* ����ֻ�м��������Ϊ�����ж� ********/
	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);/******* ������ģʽ **********/
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����2ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /****** �Ƚ����2Nʧ�� *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_ITConfig(TIM1, TIM_IT_Update ,ENABLE);  //TIM1   ʹ�ܻ���ʧ��ָ����TIM�ж�
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1									  
}
/******* TIM1�����жϷ������ *********/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_FLAG_Update)!=RESET)//�����ж�
	{
		TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);//��������жϱ�־λ		
		if(is_rcr_finish_X==0)//�ظ�������δ�������
		{
			if(rcr_integer_X!=0) //�����������廹δ�������
			{
				TIM1->RCR=RCR_VAL_X;//�����ظ�����ֵ
				rcr_integer_X--;//����RCR_VAL_X+1������				
			}else if(rcr_remainder_X!=0)//������������ ��λ0
			{
				TIM1->RCR=rcr_remainder_X-1;//������������
				rcr_remainder_X=0;//����
				is_rcr_finish_X=1;//�ظ��������������				
			}else goto out;   //rcr_remainder=0��ֱ���˳�			 
			TIM_GenerateEvent(TIM1,TIM_EventSource_Update);//����һ�������¼� ���³�ʼ��������
			TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	
			TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1			
			if(motor_dir_X==CW) //�������Ϊ˳ʱ��   
				current_pos_X+=(TIM1->RCR+1);//�����ظ�����ֵ
			else          //������Ϊ��ʱ��
				current_pos_X-=(TIM1->RCR+1);//��ȥ�ظ�����ֵ			
		}else
		{
out:		is_rcr_finish_X=1;//�ظ��������������
			finish_X=1;
			TIM_CtrlPWMOutputs(TIM1,DISABLE);	//MOE ������ر�
			TIM_Cmd(TIM1, DISABLE);  //�ر�TIM8				
//			printf("X�ᵱǰλ��=%fcm\r\n",current_pos_X*One_Pulse_Distance);//��ӡ���
		}	
	}
}
/***************** ����TIM8 *****************/
void TIM1_Startup(u32 frequency)   //������ʱ��1
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM1,temp_arr);//�趨�Զ���װֵ	
	TIM_SetCompare4(TIM1,temp_arr>>1); //ƥ��ֵ2������װֵһ�룬����ռ�ձ�Ϊ50%	
	TIM_SetCounter(TIM1,0);//����������
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}
/********************************************
//��Զ�λ���� 
//num 0��2147483647
//frequency: 20Hz~100KHz
//dir: CW(˳ʱ�뷽��)  CCW(��ʱ�뷽��)
*********************************************/
void Locate_Rle_X(long num,u32 frequency,DIR_Type dir) //X����Զ�λ����
{
	if(num<=0) //��ֵС����0 ��ֱ�ӷ���
	{
		return;
	}
	if(TIM1->CR1&0x01)//��һ�����廹δ�������  ֱ�ӷ���
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		return;
	}
	motor_dir_X=dir;//�õ�����	
	DRIVER_DIR_X=motor_dir_X;//���÷���
	
	if(motor_dir_X==CW)//˳ʱ��
		target_pos_X=current_pos_X+num;//Ŀ��λ��
	else if(motor_dir_X==CCW)//��ʱ��
		target_pos_X=current_pos_X-num;//Ŀ��λ��
	
	rcr_integer_X=num/(RCR_VAL_X+1);//�ظ�������������
	rcr_remainder_X=num%(RCR_VAL_X+1);//�ظ�������������
	is_rcr_finish_X=0;//�ظ�������δ�������
	TIM1_Startup(frequency);//����TIM1
}
/********************************************
//��Զ�λ����  ���� 
//num 0��2147483647
//frequency: 20Hz~100KHz
//dir: CW(˳ʱ�뷽��)  CCW(��ʱ�뷽��)
*********************************************/
void Locate_Rle_X_dis(long dis,u32 frequency,DIR_Type dir) //X����Զ�λ����
{
	long num;
	num = 2964*dis;
	if(num<=0) //��ֵС����0 ��ֱ�ӷ���
	{
		return;
	}
	if(TIM1->CR1&0x01)//��һ�����廹δ�������  ֱ�ӷ���
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		return;
	}
	motor_dir_X=dir;//�õ�����	
	DRIVER_DIR_X=motor_dir_X;//���÷���
	
	if(motor_dir_X==CW)//˳ʱ��
		target_pos_X=current_pos_X+num;//Ŀ��λ��
	else if(motor_dir_X==CCW)//��ʱ��
		target_pos_X=current_pos_X-num;//Ŀ��λ��
	
	rcr_integer_X=num/(RCR_VAL_X+1);//�ظ�������������
	rcr_remainder_X=num%(RCR_VAL_X+1);//�ظ�������������
	is_rcr_finish_X=0;//�ظ�������δ�������
	TIM1_Startup(frequency);//����TIM1
}
/********************************************
//���Զ�λ���� 
//num   -2147483648��2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs_X(long num,u32 frequency)//X����Զ�λ����
{
	if(TIM1->CR1&0x01)//��һ�����廹δ������� ֱ�ӷ���
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		return;
	}
	target_pos_X=num;//����Ŀ��λ��
	if(target_pos_X!=current_pos_X)//Ŀ��͵�ǰλ�ò�ͬ
	{
		if(target_pos_X>current_pos_X)
			motor_dir_X=CW;//˳ʱ��
		else
			motor_dir_X=CCW;//��ʱ��
		DRIVER_DIR_X=motor_dir_X;//���÷���
		
		rcr_integer_X=abs(target_pos_X-current_pos_X)/(RCR_VAL_X+1);//�ظ�������������
		rcr_remainder_X=abs(target_pos_X-current_pos_X)%(RCR_VAL_X+1);//�ظ�������������
		is_rcr_finish_X=0;//�ظ�������δ�������
		TIM1_Startup(frequency);//����TIM1
	}
}






