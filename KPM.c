//KPM.c
#include "defines.h"
#include<lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "KPM_defines.h"
/*u8 KpmLUT[4][4]={{1,2,3,4},
									{5,6,7,8},
									{9,10,11,12},
									{13,14,15,16}};*/
									

u8 KpmLUT[4][4]={{'1','2','3','+'},
									{'4','5','6','-'},
									{'7','8','9','*'},
									{'C','0','=','/'}};
void InitKPM(void)
{
	//cfg row pins as outputs(p1.16 to p1.19)
	WRITENIBBLE(IODIR1,ROW0,15);
}	

u8 ColScan(void)
{
	if((READNIBBLE(IOPIN1,COL0))<15)
		return 0;
	
	else 
		return 1;
}
u8 RowCheck(void)
{
	u8 rno;
	for(rno=0; rno<=3; rno++)
	{
		WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));
		if(ColScan()==0)
		{
			break;
		}
	}
	WRITENIBBLE(IOPIN1,ROW0,0x0);
	return rno;
}
u8 ColCheck(void)
{
	u8 cno;
	for(cno=0; cno<=3; cno++)
	{
		if(STATUSBIT(IOPIN1,(COL0+cno))==0)
		{
			break;
		}
	}
	return cno;	
}

u8 KeyScan(void)
{
	u8 keyv,rno,cno;
	//wait switch press
	while(ColScan());
	//take the rno
	rno =RowCheck();
	//take the cno
	cno=ColCheck();
	//collect key value from KpmLUT
	keyv=KpmLUT[rno][cno];
	//wait for switch relese
	while(!ColScan());
	return keyv;
}

u32 ReadNum(void)
{
	u8 key;
	u32 sum=0;
	while(1)
	{
	key=KeyScan();
	if((key>='0')&&(key<='9'))
	{
		//CmdLCD(0xc0);
		//CharLCD(key);
		sum=(sum*10)+(key-48);
		
	}
	else if(key=='C')
		break;
	}
	return sum;
}

