/***********************************************************
STC90C51RD+与HC-06蓝牙连线：
        蓝牙  STC90C51RD+
        RXD    TXD
        TXD    RXD
其他连线：
        P2.0、P2.1、P2.3分别接一个指示灯

！！！！
！！！！注意：若单片机TXD（P3.1）无上拉能力，必须在P3.1端接上拉电阻。本次测试需要接上拉电阻
！！！！

手机采用蓝牙串口通信助手
最后已改用Android自编蓝牙通信软件
                                        By itas109
***********************************************************/
#include <reg52.h>
#include <intrins.h>

typedef unsigned int u16;
typedef unsigned char u8;

u8 tmp;
u16 c = 0;

sbit led1 = P2 ^ 0;//指示灯0
sbit led2 = P2 ^ 1;//指示灯1
sbit led3 = P2 ^ 2;//指示灯2
sbit led4 = P2 ^ 3;//指示灯3

void init();   //串口初始化
void send(unsigned int a);//单字节发送函数
void ctrl(); //接收处理函数

void main()
{
	init();
	while (1)
	{
		// 是否有数据到来
		if (RI == 1)
		{
			RI = 0;

			// 暂存接收到的数据
			tmp = SBUF;
			ctrl();
		}
	}
}

//串口初始化
void init()
{
	//关中断
	ES = 0;
	SCON = 0x50;                        // REN=1允许串行接受状态，串口工作模式1,10位UART（1位起始位，8位数据位，1位停止位，无奇偶校验），波特率可变

	TMOD = 0x20;                        // 定时器1工作于方式2，8位自动重载模式, 用于产生波特率
	TH1 = TL1 = 0xFD;                   // 波特率9600 （本次测试采用晶振为11.0592）

	PCON &= 0x7f;                       // 波特率不倍增
	TR1 = 1;                            // 定时器1开始工作，产生波特率
	//发送标志位置0
	TI = 0;                             // 接收标志位置0
	RI = 0;

	//EA=0;
	ES = 1;

	//初始化设置3个指示灯全亮
	led1 = 0;
	led2 = 0;
	led3 = 0;
}

//单字节数据发送
void send(u16 a) {
//注意：若单片机TXD（P3.1）无上拉能力，必须在P3.1端接上拉电阻。本次测试需要接上拉电阻
	TI = 0;
	SBUF = a;
	while (TI == 0);
	TI = 0;

//发送指示灯标志，每接收一次，此灯亮灭交替
	if (c % 2)
		led4 = 1;
	else
		led4 = 0;
	c++;
}

//接收处理函数
void ctrl()
{
	switch (tmp)
	{
	//收到字符1，指示灯0灭
	case 1:
		led1 = 0;
		send(tmp);
		break;

	//收到字符2，指示灯1灭
	case 2:
		led2 = 0;
		send(tmp);
		break;

	//收到字符3，指示灯3亮
	case 3:
		led3 = 0;
		send(tmp);
		break;

	//收到字符4，指示灯0、1、2灭
	case 4:
		led1 = 1;
		led2 = 1;
		led3 = 1;
		send(tmp);
		break;

    default:
		send(tmp);
		break;
	}
}