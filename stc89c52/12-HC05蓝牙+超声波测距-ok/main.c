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
#include <stdio.h>

#define uchar unsigned char
#define uint unsigned int

#define BAUD 9600L
#define FOSC 22118400L
#define CYCLE (12000000.0/FOSC) //周期 us  

sbit Trig = P1 ^ 6;
sbit Echo = P1 ^ 7;


uint tmp;
uint c = 0;

sbit led1 = P2 ^ 0;//指示灯0
sbit led2 = P2 ^ 1;//指示灯1
sbit led3 = P2 ^ 2;//指示灯2

void uartInit();
void sendByte(uchar dat);
void writeString(char *str);
void sleep();
float getDist();
void ctrl();


void main() {
	char buffer[16] = {0};
	Trig = 0;
	Echo = 1;
	uartInit();
	while (1) {
		if (RI == 1) {	// 是否有数据到来
			RI = 0;

			sprintf(buffer, "%.2f cm\n", getDist());
			writeString(buffer);

			tmp = SBUF;
			ctrl();
			sleep();
		}
	}
}


void uartInit() {
	TMOD = 0x20;        //定时器1用于串口
	TH1 = TL1 = -(FOSC / 12 / 32 / BAUD); //设置装入初值
	TR1 = 1;                            //启动定时器1
	SM0 = 0;
	SM1 = 1;                            //设置串口为方式一
	REN = 1;                            //接收使能
	ES = 1;                             //打开中断
	EA = 1;

	//初始化设置3个指示灯全亮
	led1 = 0;
	led2 = 0;
	led3 = 0;
}

void sendByte(uchar dat) {
	ES = 0;     //关闭中断
	SBUF = dat; //写入数据
	while (!TI); //等待数据发送结束
	TI = 0;     //手动置0
	ES = 1;     //打开中断
}

void writeString(char *str) {
	while (*str) {
		sendByte(*str);
		str++;
	}
}

void sleep() {
	uchar i, j, k;
	for (i = 0; i < 255; i++)
		for (j = 0; j < 255; j++)
			for (k = 0; k < 16; k++);
}

float getDist() {
	uchar i = 100;
	float dist = 0.0f;
	uint count = 0;

	Trig = 1;       //打开触发
	while (i--);    //延时一会儿
	TMOD = 0x01;    //计时器0方式1
	TH0 = TL0 = 0;  //装入初值
	Trig = 0;       //关闭触发
	while (!Echo);  //测距过程中
	TR0 = 1;        //打开中断
	while (Echo);   //等待输出电平结束
	TR0 = 0;        //关闭中断
	count = (TH0 << 8) | TL0; //读取16位计时器里的数字
	dist = CYCLE * count * 0.017;   //0.034cm/us
	return dist;
}

//接收处理函数
void ctrl()
{
	switch (tmp)
	{
	//收到字符1，指示灯全亮
	case 1:
		led1 = 0;
		led2 = 0;
		led3 = 0;
		sendByte(tmp);
		break;

	default:
		led1 = 1;
		led2 = 1;
		led3 = 1;
		sendByte(tmp);
		break;
	}
}