  
#include <SFE_TSL2561.h>
/*
���ܣ�������
*/
void SFE_TSL2561_read(  unsigned char cmd,unsigned char count,unsigned char data[])
{
    //1������ʼ�ź�
    //2�������豸��ַ
    //3���ȴ��豸Ӧ��
    //4����������
    //5���ȴ��豸Ӧ��
    //6������ʼ�ź�
    //7�������豸��ַ
    //8���ȴ��豸Ӧ��
    //9����ʼ�����ֽ����ݣ�ÿ������һ���ֽں����豸Ӧ��
    int i;
    IIC_Start();
    IIC_Send_Byte(TSL2561_ADDR<<1);
    IIC_Wait_Ack();
    IIC_Send_Byte(cmd);
    IIC_Wait_Ack();
    
    IIC_Start();
    IIC_Send_Byte((TSL2561_ADDR<<1)+1);
    IIC_Wait_Ack();
    
    for(i=0;i<count;i++)
    {
        data[i]=IIC_Read_Byte(1);
    }
    IIC_Stop(); 
}
/*
���ܣ�д����
*/
void SFE_TSL2561_write( unsigned char cmd,unsigned char count,unsigned char data[])
{
    //1������ʼ�ź�
    //2�������豸��ַ
    //3���ȴ��豸Ӧ��
    //4����������
    //5���ȴ��豸Ӧ��
    //6����ʼ�����ֽ�����1��2��3...,ÿ������һ���ֽڶ�Ҫ�ȴ��豸Ӧ��֮����Բ���ֹͣ����
  
    int i;
    IIC_Start();
    IIC_Send_Byte(TSL2561_ADDR<<1);
    IIC_Wait_Ack();
    IIC_Send_Byte(cmd);
    IIC_Wait_Ack();
    for(i=0;i<count;i++)
    {
        IIC_Send_Byte(data[i]);
        IIC_Wait_Ack();
    }
    IIC_Stop();
}
//��ʼ���豸
void SFE_TSL2561_init(void)
{  
    //1����ʼ��ͨ�Žӿ�
    //2�����豸��Դ
    unsigned char data=3;
    IIC_Init();
    SFE_TSL2561_write(TSL2561_REG_COMMAND+TSL2561_REG_CONTROL,1,&data);
}
//�����ݼĴ�����data[0]��data[1]�ֱ𱣴���data0��data1������
void SFE_TSL2561_read_data0_and_data1(unsigned int data[])
{
    unsigned char datas[4];
    SFE_TSL2561_read(TSL2561_REG_COMMAND|(1<<4)+TSL2561_REG_DATA0LOW,sizeof(datas),datas);
    data[0]=datas[0]|(datas[1]<<8);
    data[1]=datas[2]|(datas[3]<<8);  
}
void SFE_TSL2561_read_gain_and_intge(unsigned int data[])
{
    unsigned char dat;
    SFE_TSL2561_read(TSL2561_REG_COMMAND+TSL2561_REG_TIMING,1,&dat);
    if(dat&(1<<4))
        data[0]=1;
    else
        data[0]=0;
    data[1]=dat&0x3;
    
}






