/***********************************************************
STC90C51RD+��HC-06�������ߣ�
        ����  STC90C51RD+
        RXD    TXD
        TXD    RXD
�������ߣ�
        P2.0��P2.1��P2.3�ֱ��һ��ָʾ��

��������
��������ע�⣺����Ƭ��TXD��P3.1��������������������P3.1�˽��������衣���β�����Ҫ����������
��������

�ֻ�������������ͨ������
����Ѹ���Android�Ա�����ͨ�����
                                        By itas109
***********************************************************/
#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

#define uchar unsigned char
#define uint unsigned int

#define BAUD 9600L
#define FOSC 22118400L
#define CYCLE (12000000.0/FOSC) //���� us  

sbit Trig = P1 ^ 6;
sbit Echo = P1 ^ 7;


uint tmp;
uint c = 0;

sbit led1 = P2 ^ 0;//ָʾ��0
sbit led2 = P2 ^ 1;//ָʾ��1
sbit led3 = P2 ^ 2;//ָʾ��2

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
		if (RI == 1) {	// �Ƿ������ݵ���
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
	TMOD = 0x20;        //��ʱ��1���ڴ���
	TH1 = TL1 = -(FOSC / 12 / 32 / BAUD); //����װ���ֵ
	TR1 = 1;                            //������ʱ��1
	SM0 = 0;
	SM1 = 1;                            //���ô���Ϊ��ʽһ
	REN = 1;                            //����ʹ��
	ES = 1;                             //���ж�
	EA = 1;

	//��ʼ������3��ָʾ��ȫ��
	led1 = 0;
	led2 = 0;
	led3 = 0;
}

void sendByte(uchar dat) {
	ES = 0;     //�ر��ж�
	SBUF = dat; //д������
	while (!TI); //�ȴ����ݷ��ͽ���
	TI = 0;     //�ֶ���0
	ES = 1;     //���ж�
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

	Trig = 1;       //�򿪴���
	while (i--);    //��ʱһ���
	TMOD = 0x01;    //��ʱ��0��ʽ1
	TH0 = TL0 = 0;  //װ���ֵ
	Trig = 0;       //�رմ���
	while (!Echo);  //��������
	TR0 = 1;        //���ж�
	while (Echo);   //�ȴ������ƽ����
	TR0 = 0;        //�ر��ж�
	count = (TH0 << 8) | TL0; //��ȡ16λ��ʱ���������
	dist = CYCLE * count * 0.017;   //0.034cm/us
	return dist;
}

//���մ�����
void ctrl()
{
	switch (tmp)
	{
	//�յ��ַ�1��ָʾ��ȫ��
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