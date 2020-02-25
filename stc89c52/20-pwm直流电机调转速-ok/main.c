#include<reg51.h>
#define TH0_TL0 (65536-1000)//�趨�жϵļ��ʱ��
unsigned char count0 = 50;//�͵�ƽ��ռ�ձ�
unsigned char count1 = 0;//�ߵ�ƽ��ռ�ձ�

bit Flag = 1;//�������ת��־λ,1��ת��0��ת

sbit Key_add = P3 ^ 1; //�������
sbit Key_dec = P3 ^ 2; //�������
sbit Key_turn = P3 ^ 3; //�������

sbit PWM1 = P2 ^ 6; //PWM ͨ�� 1����ת����
sbit PWM2 = P2 ^ 7; //PWM ͨ�� 2����ת����

unsigned char Time_delay;

/************��������**************/
void Delay(unsigned char x);
void Motor_speed_high(void);
void Motor_speed_low(void);
void Motor_turn(void);
void Timer0_init(void);

/****************��ʱ����**********************/
void Delay(unsigned char x)
{
	Time_delay = x;
	while (Time_delay != 0); //�ȴ��жϣ��ɼ���PWM���ʱ����
}

/*******���������pwmռ�ձȣ��������**********/
void Motor_speed_high(void)
{
	if (Key_add == 0)
	{
		Delay(10);
		if (Key_add == 0)
		{
			count0 += 5;

			if (count0 >= 100)
			{
				count0 = 100;
			}
		}
		while (!Key_add); //�ȴ����ɿ�
	}
}

/******���������pwmռ�ձȣ��������*****/
void Motor_speed_low(void)
{
	if (Key_dec == 0)
	{
		Delay(10);
		if (Key_dec == 0)
		{
			count0 -= 5;

			if (count0 <= 0)
			{
				count0 = 0;
			}
		}
		while (!Key_dec );
	}
}

/************������������**************/
void Motor_turn(void)
{
	if (Key_turn == 0)
	{
		Delay(10);
		if (Key_turn == 0)
		{
			Flag = ~Flag;
		}
		while (!Key_turn);
	}
}

/***********��ʱ��0��ʼ��***********/
void Timer0_init(void)
{
	TMOD = 0x01; //��ʱ��0�����ڷ�ʽ1
	TH0 = TH0_TL0 / 256;
	TL0 = TH0_TL0 % 256;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
}

/*********������********************/
void main(void)
{
	Timer0_init();

	while (1)
	{
		Motor_turn();
		Motor_speed_high();
		Motor_speed_low();
	}
}

/**************��ʱ0�жϴ���******************/
void Timer0_int(void) interrupt 1 using 1
{
	TR0 = 0;//���ö�ʱ����ֵ�ڼ䣬�رն�ʱ��
	TL0 = TH0_TL0 % 256;
	TH0 = TH0_TL0 / 256 ;//��ʱ��װ��ֵ
	TR0 = 1;

	if (Time_delay != 0) //��ʱ������
	{
		Time_delay--;
	}

	if (Flag == 1) //�����ת
	{
		PWM1 = 0;

		if (++count1 < count0)
		{
			PWM2 = 1;
		}
		else
			PWM2 = 0;

		if (count1 >= 100)
		{
			count1 = 0;
		}
	}
	else //�����ת
	{
		PWM2 = 0;

		if (++count1 < count0)
		{
			PWM1 = 1;
		}
		else
			PWM1 = 0;

		if (count1 >= 100)
		{
			count1 = 0;
		}
	}
}
/************************************************

void Delay(unsigned char x)
{
Time_delay = x;

while(Time_delay != 0);//�ȴ��жϣ��ɼ���PWM���ʱ����
}

Delay(10);

if(Time_delay != 0)//��ʱ������
{
  Time_delay--;
}

��ʵ������������������ġ�
Delay��10��������ʱ���������Ǻ������ʵģ������������������ͬʱ��һ��ʼֻ���������µ�Time-

delay��ֵ�ˣ���ֵ����������ǣ������ʱ����ȥ�ˡ��㿴�ڸ�ֵ��Time-delay�����10�����ڶ�������

��while��Time-delay��=10��������������ע����˵�ĵȴ��жϣ����������жϺ�����Կ���������

���ˣ������˰��ն����н�Time-delay��һ��ÿ�ж�һ�ξͼ�һ��ֱ������0֮��Ҳ����˵��ʱ��ʱ��

��ʮ���жϵ�ʱ����ע���У��ɼ���PWM���ʱ����      Ҳ�Ϳ����֪�ˡ�
*****************************************************/