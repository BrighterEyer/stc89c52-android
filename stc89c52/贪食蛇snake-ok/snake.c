#include<reg51.h>
#include<intrins.h>
#include<stdlib.h>
typedef unsigned int u16;
typedef unsigned char u8;
sbit SER=P3^4;
sbit RCLK=P3^5;
sbit SRCLK=P3^6;
sbit up=P3^1;
sbit down=P3^0;
sbit lt=P3^2;
sbit rt=P3^3;

u8	code Hc595_Coordx[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
u8	code Hc595_Coordy[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
u8	 foodx;
u8   foody;
u8   snake_x[20]={0};
u8   snake_y[20]={0};
u8 length;
u8 direction;
void initSnake();
void delay(u16 i);
void Hc595SendByte(u8 dat);
void move();
void Keypros();
void creatFood();
void snake_Grow(void);
void drawsnake();



void initSnake()
{
  	snake_x[0]=0;
	snake_y[0]=0;
	length=1;
	direction=2;
}
void delay(u16 i)
{
  while(i--);
}
void Hc595SendByte(u8 dat)
{
	u8 a;
	SRCLK=0;
	RCLK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RCLK=1;
	_nop_();
	_nop_();
	RCLK=0;
}



void move()
{
     
    u8 len = length- 1;
    for(len; len > 0; len--)
    {
        snake_x[len] = snake_x[len - 1];
        snake_y[len] = snake_y[len - 1];	//һֱ����ͷ
    }
    switch(direction)
    {
    case 1:
        snake_y[0]--;
        break;
    case 2:
        snake_y[0]++;						 //��������ͷת�䷽��
        break;
    case 3:
        snake_x[0]--;
        break;
    case 4:
        snake_x[0]++;
        break;
    default:
        break;
    }

}


//̰���ߵ����������ж�����
void Keypros()
{
	if(up==0)
	{
		delay(1000);  //��������
		if(up==0)
		{
		  if(direction==3|direction==4)
		  direction=1;             //�ڵ�ַ1��д������num
		}
		while(!up);
	}
	if(down==0)
	{
		delay(1000);  //��������
		if(down==0)
		{	if(direction==3|direction==4)
			direction=2;	  
		}
		while(!down);
	}
	if(lt==0)
	{
		delay(100);  //��������
		if(lt==0)
		{
		    if(direction==1|direction==2)
			direction=3; 
		}
		while(!lt);
	}
	if(rt==0)
	{
		delay(1000);  //��������
		if(rt==0)
		{
		   if(direction==1|direction==2)
		   direction=4; 
		}
		while(!rt);
	}		
}
void creatFood()	 //�������ʳ��
{
    u8 i;
    foodx=rand()%8;
    foody=rand()%8;
    for(i=0; i<length; i++)
    {
        if((foodx==snake_x[i])&&(foody==snake_y[i]))	//���ʳ�����ߵ������ص���Ҫ���´���
                {
                    creatFood();
                }       
    }
}
void snake_Grow(void)
{
    if(snake_x[0] == foodx && snake_y[0] == foody)
    {
        creatFood();
        snake_x[length] = snake_x[length - 1];
        snake_y[length] = snake_y[length - 1];
        length++;
    }
}



void drawsnake()   //�����ߺ�ʳ��
{
     u8	i;
     for(i=0;i<length;i++)
	{
	  	Hc595SendByte(Hc595_Coordy[snake_y[i]]);
		  P0=Hc595_Coordx[snake_x[i]];
        delay(20);
		P0=0xff;

	    Hc595SendByte(Hc595_Coordy[foody]);   
	    P0=Hc595_Coordx[foodx];
        delay(20);
	    P0=0xff;		
	}
}

 void JudgeDeath(void)
{
    u8 i;

    // �ж���ײǽ����
    if((snake_x[0]>7)||(snake_y[0]>7))
    {
         Hc595SendByte(0x00);
        while(1);
    }

    // �ж���ײ���Լ���������
    for(i=4; i<length; i++)
    {
        if((snake_x[0]==snake_x[i])&&(snake_y[0]==snake_y[i]))
        {
            P0=0xFF;
            while(1);
        }
    }
}


void Timer0Init()
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������

	TH0=0XD8;	//����ʱ������ֵ����ʱ10ms
	TL0=0XF0;	
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0=1;//�򿪶�ʱ��			
}

void main()
{   
   
	Timer0Init();
	initSnake();	
	while(1)
	{ 
	 drawsnake();
	 JudgeDeath();
	 snake_Grow();
	 Keypros();
	}
}

void Timer0() interrupt 1
{
	static u16 i;
	TH0=0XD8;	//����ʱ������ֵ����ʱ10ms
	TL0=0XF0;
	i++;
	if(i==50)
	{
	 i=0;
     move();
	
	}	
}
