//lcd.c
#include "defines.h"
#include "types.h"
#include <lpc21xx.h>
#include "lcd_defines.h"
#include "delay.h"

void WriteLCD(u8 data)
{
	SCLRBIT(IOCLR0,RW);// r/w=0(to perform write operation)
	WRITEBYTE(IOPIN0,LCD_DATA,data);
	SSETBIT(IOSET0,EN);// en=1
	delay_us(1);
	SCLRBIT(IOCLR0,EN);// en=0
	delay_ms(2); // internal process
}
void CmdLCD(u8 cmd)
{
	SCLRBIT(IOCLR0,RS);//rs=0(command reg select)
	WriteLCD(cmd);// write cmd onto the data pins
}
void CharLCD(u8 ascii)
{
	SSETBIT(IOSET0,RS);//rs=1 (data reg select)
	WriteLCD(ascii);// write data onto the data pins
}

void InitLCD(void)
{
	//p0.8 t0 p0.15 cfg as outputs
	WRITEBYTE(IODIR0,LCD_DATA,0xFF);
	//p0.16 t0 p0.18 cfg as outputs
	SETBIT(IODIR0,RS);
	SETBIT(IODIR0,RW);
	SETBIT(IODIR0,EN);
	
	delay_ms(15);
	CmdLCD(MODE_8BIT_1LINE);
	delay_ms(5);
	CmdLCD(MODE_8BIT_1LINE);
	delay_us(100);
	CmdLCD(MODE_8BIT_1LINE);
	
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DISP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD );
	CmdLCD(SHIFT_CUR_RIGHT );
}

void StrLCD(u8* str)
{
	while(*str)
	{
		CharLCD(*str++);
	}
}
void U32LCD(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
		{
			CharLCD('0');
			
		}
	else{
				while(n)
				{
					a[i++]=(n%10)+'0';
					n/=10;
				}
				for(--i; i>=0; i--)
				{
					CharLCD(a[i]);
				}
			}
}
void S32LCD(s32 n)
{
	if(n<0)
	{
		n=-n;
		CharLCD('-');
		U32LCD(n);
	}
	
}
void BinLCD(u32 n, u8 nbd)
{
	s32  i;
	for(i=(nbd-1); i>=0; i--)
	{
		CharLCD(((n>>i)&1)+48);
	}
}
void BuildCGRAM(u8* p, u8 nBytes)
{
	s32 i;
	//select cgram
	CmdLCD(GOTO_CGRAM);
	for(i=0; i<nBytes; i++)
	{
		//write byte by byte via data reg
		CharLCD(p[i]);
	}
	//go to DDRAM
	CmdLCD(GOTO_LINE2_POS0);
}
void f32LCD(f32 fnum, u8 ndp)
{
	u32 inum;
	if(fnum<0.0)
	{
		CharLCD('-');
		fnum=-fnum;
	}
	inum=fnum;
	U32LCD(inum);
	CharLCD('.');
	while(ndp)
	{
		fnum=(fnum-inum)*10;
		inum=fnum;
		CharLCD(inum+48);
		ndp--;
	}
}

void HexLCD(u32 n)
{
	u8 a[8],rem;
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
		while(n)
		{
			rem=n%16;
			(rem<10 )? (rem+=48 ):(rem+=55);
			a[i++]=rem;
			n/=16;
		}
		for(--i; i>=0; i--)
		{
			CharLCD(a[i]);
		}
}
void OctLCD(u32 n)
{
	s32 i=0;
	u8 a[12];
	if(n==0)
	{
		CharLCD('0');
	}
	else
		while(n)
		{
			a[i++]=(n%8)+48;
			n/=8;
		
		}
		for(--i; i>=0; i--)
		{
			CharLCD(a[i]);
		}
}


void DataLCD(char data)
{
    CharLCD(data);
}


