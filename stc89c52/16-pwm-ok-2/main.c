
#include <reg52.h> 
#include <math.h>
typedef unsigned char uchar;
typedef unsigned int uint;
sbit PWM_OUT=P2^7; //PWM输出口
uint PWM_Value; //定义pwm值 
uint flag; //中断标志
void Delay(unsigned int s); //延时函数声明

void Init_Timer0() // 定时器初始化 
{
	TMOD=0x01;//定时器0 工作模式一，16位计数
	TH0=(65535-10)/256;//应该是每次10us
	TL0=(65535-10)%256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	PT0=1;
}

void timer0(void) interrupt1 //实测一次中断0.04432ms
{ 
	TH0=(65535-10)/256; 
	TL0=(65535-10)%256; 
	if (flag>PWM_Value)
	{
		PWM_OUT=0;
	}
	else
	{
		PWM_OUT=1;
	}
	if (flag>451)
	{
		flag=0;
	}
	flag++; 
}


void main()
{
	PWM_Value = 25; //pwm值为11-56,42时门框打开，25时门框闭合
	Init_Timer0();
	while(1);

}
