#include <reg52.h>
sbit SG_PWM=P0^0;
unsigned char count=0;
unsigned char PWM_count=3;                  //1--0�ȣ�2--45�ȣ�3--90�ȣ�4--135�ȣ�5--180��
unsigned char a=0;
void Timer_Init()
{
  TMOD=0X01;      //T0��ʱ��ʽ1
  TH0=0Xfe;
  TL0=0X33;       //������ֵ����Ϊ0.5ms
  ET0=1;          //�򿪶�ʱ��0���ж�
  TR0=1;          //�򿪶�ʱ��0
  EA=1;           //�����ж�
}
void Timer() interrupt 1      //�ر�ע��˴���0--�ⲿ�ж�0��1--��ʱ���ж�0��2--�ⲿ�ж�1��3--��ʱ���ж�1��4--���п��ж�1
{
  TR0=0;
  TH0=0Xfe;
  TL0=0X33;       //���¸�������ֵΪ0.5ms
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
      if(a==16)                             //20�ǱȽ���������ᶯ����20���϶���Щ�����ᣬ10��ʱ����ִ�������15��ʱ���ܼ���û�У�16�Ƚ�����
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
