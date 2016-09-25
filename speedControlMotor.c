#include<pic.h>
#include<htc.h>
#include"Delay.h"
#include"eeprom.h"
void int_ini();
void timer_ini();
void pwm_ini();
void delay();
void countUp();
void countDown();
void countDisp();
void value_check(unsigned char value);
void value_check1(unsigned char value);
void check_status();
void countDispt(unsigned char);
#define cry_freq 16000000l
#define pwm_freq 1000u
#define reg (cry_freq/(pwm_freq*4*16))-1
unsigned char opt=1;
unsigned char dat1,dat2,dat3;
#define sg1 PORTB   //CA as a 7-segment
#define seg1 RC5
#define seg2 RC6
#define seg3 RC7
#define s1 RD0
#define s2 RD1
#define enter RD2
#define start RD3
#define stop RD4
bit onOff;
unsigned char count=0;
void main()
{
unsigned char countc,rd,st=1;
	TRISB=TRISC=0x00;
	TRISD=0x1f;
	int_ini();
	timer_ini();
	pwm_ini();
	onOff=0;
	rd=eeprom_rd(0x20);
	if(rd==0x01)
	{
	    rd=eeprom_rd(0x01);
	    count=rd;
    }
	countDisp();
	while(1)
	{
		check_status();//below statement is comment
		//count++;countDisp();DelayMs(1000);
		if(onOff)
		{
		    if(st)
		    {
                value_check1(count);    //Set PWM value
                st=0;
            }
            value_check(count);         //Set PWM value
            countDisp();                //Display value on 7-Segment
		}
		if(onOff==0)    // Stop PWM Output and Display 000 on 7-segment.
        {
            CCPR1L=0x00;
            dat1=dat2=dat3=0x3f;
        }
		la1:
		if(enter==0)    //Edit PWM value and wait for user input for infinite time
		{
		    DelayMs(2);
			if(enter==0)
			{
			    while(enter==0);
                eeprom_wrt(0x20,0x01);//below statement is comment
                dat1=dat2=dat3=0x3f;
                DelayMs(1000);
                DelayMs(1000);
                while(1)
                {
                    st=1;
                    countUp();
                    countDown();
                    countDisp();
                    eeprom_wrt(0x01,count);
                    if(enter==0)
					{
					    DelayMs(5);
                        if(enter==0)
                        {
                            while(enter==0);
                            goto la1;
                        }
					}
                }
			}
		}
	}
}
//------------ To check start/stop switch is in ON/OFF status -------------------
void check_status()
{
	if(start==0)
	{DelayMs(5);
		if(start==0)
		{while(start==0);onOff=1;}
	}
	if(stop==0)
	{DelayMs(5);
		if(stop==0)
		{while(stop==0);onOff=0;}
	}

}
//------------ To increment/decrement PWM value by changing voltage value from user side
void countUp()
{
	if(s1==0)
	{
		while(s1==0);
		count++;
	}
}
void countDown()
{
	if(s2==0)
	{
		 while(s2==0);
		 if(count!=0)
		 count--;
	}
}
//-------- Convert decimal value into unpacked BCD number and display it on 7-Segment
void countDisp()
{
	unsigned char countc,ar[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,'\0'};
	unsigned char lsb,msb,mid;
	countc=count;
	lsb=countc%10;
	countc=countc/10;
	mid=countc%10;
	msb=countc/10;
	dat1=ar[lsb];
	dat2=ar[mid];
	dat3=ar[msb];
}
void countDispt(unsigned char countc)
{
	unsigned char ar[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,'\0'};
	unsigned char lsb,msb,mid;
	lsb=countc%10;
	countc=countc/10;
	mid=countc%10;
	msb=countc/10;
	dat1=ar[lsb];
	dat2=ar[mid];
	dat3=ar[msb];
}
//---------- Delay function --------------------------------------------
void delay()
{
	unsigned int i,j;
	for(i=0;i<2;i++)
	 for(j=0;j<65535;j++);
}
//--------- Enable Timer interrupt for 7-segment multiplex -------------
void int_ini()
{
	INTCON=0b11000000;
	TMR1IE=1;
}
//---------- Configure Timer1 Register ---------------------------------
void timer_ini()
{
	TMR1H=0xc0;
	TMR1L=0x00;
	T1CON=0x00;
	TMR1ON=1;
}
//---------- Configure Timer2 for PWM ----------------------------------
void pwm_ini()
{
     CCP1CON=0x0c;        //From capture/compare/PWM select PWM
     PR2=reg;             //Set PWM period =1khz
     T2CON=0x02;          //prescaler = 16 and Timer2 ON;
}
//---------- 7- Segment Multiplex for 3 digit 7-Segment ---------------
void interrupt isr()
{
	switch(opt)
	{
		case 1:
				PORTC=0xff;
				seg1=0;
				PORTB=dat1;
		break;
		case 2:
				PORTC=0xff;
				seg2=0;
				sg1=dat2;
		break;
		case 3:
				PORTC=0xff;
				seg3=0;
				sg1=dat3;
		break;
	}
	opt++;
	if(opt==4)
	 opt=1;
	TMR1H=0xc0;
	TMR1L=0x00;
	TMR1IF=0;
}
//--------- Set PWM value -------------------------------
void value_check(unsigned char value)
{
         if(value<20)
         {
         	CCPR1L=0x00;              //off
         }
        else if((value>=20)&&(value<40))
         {
         	CCPR1L=12;    //10%
         }
        else if((value>=40)&&(value<60))
         {
         	CCPR1L=25;    //20%
         }
        else if((value>=60)&&(value<80))
         {
         	CCPR1L=31;    //25%
         }
        else if((value>=80)&&(value<100))
         {
         	CCPR1L=37;    //30%
         }
        else if((value>=100)&&(value<120))
         {
         	CCPR1L=49;    //40%
         }
        else if((value>=120)&&(value<140))
         {
         	CCPR1L=62;    //50%
         }
        else if((value>=140)&&(value<160))
         {
         	CCPR1L=75;    //60%
         }
         else if((value>=160)&&(value<180))
         {
         	CCPR1L=93;    //75%
         }
         else if((value>=180)&&(value<200))
         {
         	CCPR1L=111;    //90%
         }
         else if(value>=200)
         {
         	CCPR1L=124;    //100%
         }

        TMR2=0x00;
        TMR2IF=0;
        TMR2ON=1;
        while(TMR2IF==0);

}
//------- Set PWM value with delay and display it on 7-Segment
void value_check1(unsigned char value)
{
         if(value<20)
         { countDispt(0);
         	CCPR1L=0x00;              //off
         }
        if((value>=20))
         {countDispt(20);
         	CCPR1L=12;    //10%
         	DelayMs(200);
         }
         if((value>=40))
         {countDispt(40);
         	CCPR1L=25;    //20%
         	DelayMs(200);
         }
         if((value>=60))
         {countDispt(60);
         	CCPR1L=31;    //25%
         	DelayMs(800);
         }
         if((value>=80))
         {countDispt(80);
         	CCPR1L=37;    //30%
         	DelayMs(800);
         }
        if((value>=100))
         {countDispt(100);
         	CCPR1L=49;    //40%
         	DelayMs(800);
         }
        if((value>=120))
         {countDispt(120);
         	CCPR1L=62;    //50%
         	DelayMs(800);
         }
        if((value>=140))
         {countDispt(140);
         	CCPR1L=75;    //60%
         	DelayMs(800);
         }
         if((value>=160))
         {countDispt(160);
         	CCPR1L=93;    //75%
         	DelayMs(800);
         }
         if((value>=180))
         {countDispt(180);
         	CCPR1L=111;    //90%
         	DelayMs(800);
         }
         if(value>=200)
         {countDispt(200);
         	CCPR1L=124;    //100%
         	DelayMs(800);
         }

        TMR2=0x00;
        TMR2IF=0;
        TMR2ON=1;
        while(TMR2IF==0);

}
