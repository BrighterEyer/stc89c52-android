#include <reg52.h>
sbit SG_PWM=P0^0;
unsigned char count=0;
unsigned char PWM_count=3;                  //1--0度，2--45度，3--90度，4--135度，5--180度
unsigned char a=0;
void Timer_Init()
{
  TMOD=0X01;      //T0定时方式1
  TH0=0Xfe;
  TL0=0X33;       //计数初值设置为0.5ms
  ET0=1;          //打开定时器0的中断
  TR0=1;          //打开定时器0
  EA=1;           //开总中断
}
void Timer() interrupt 1      //特别注意此处，0--外部中断0，1--定时器中断0，2--外部中断1，3--定时器中断1，4--串行口中断1
{
  TR0=0;
  TH0=0Xfe;
  TL0=0X33;       //重新赋计数初值为0.5ms
  if(count<=PWM_count)
  {
    SG_PWM=1;
  }
  else
  {
    SG_PWM=0;
  }
  count++;
  if(count>=40)
  {
    count=0;
    a++;
  }
  TR0=1;
}
void main()
{
  Timer_Init();
  while(1)
  {
      if(a==16)                             //20是比较舒服的连贯动作，20以上动作些许不连贯，10的时候出现打架情况，15的时候打架几乎没有，16比较完美
      {
        PWM_count=1;
      }
      if(a==32)
      {
        PWM_count=3;
        a=0;
      }
  }
}
