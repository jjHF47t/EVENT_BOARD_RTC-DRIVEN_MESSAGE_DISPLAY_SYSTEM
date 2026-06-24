#include "mini_project_functions.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
#include "KPM.h"
#include <stdio.h>
#include <string.h>

/* ---------- GLOBAL VARIABLES ---------- */
char line1[21];
char line2[21];

int prev_temp = -1;
int last_min = -1;

/* ---------- DAY ---------- */
char *dayname[7]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

/* ---------- MESSAGES ---------- */
char msg1[]="Good Morning! Classes Start Soon";
char msg2[]="C Programming Session in Classroom number 2";
char msg3[]="ARM kit issue time from 10AM - 10:30AM";
char msg4[]="C module theory exam in lab1";
char msg5[]="C module lab exam in lab2";
char msg6[]="Lunch Break from 1PM - 2PM";
char msg7[]="C Programming Session in Classroom 2";
char msg8[]="15 mins break for next ARM session";
char msg9[]="Revise today's programs at home!";
char msg10[]="End of Day See You Tomorrow!";

/* ---------- EVENTS ---------- */
EVENT events[10] =
{
    {msg1, sizeof(msg1)-1, 7,45,0,0,1},
    {msg2, sizeof(msg2)-1, 9,15,0,0,1},
    {msg3, sizeof(msg3)-1, 9,45,0,0,1},
    {msg4, sizeof(msg4)-1,10,15,0,0,1},
    {msg5, sizeof(msg5)-1,11,15,0,0,1},
    {msg6, sizeof(msg6)-1,12,45,0,0,1},
    {msg7, sizeof(msg7)-1,13,45,0,0,1},
    {msg8, sizeof(msg8)-1,15,15,0,0,1},
    {msg9, sizeof(msg9)-1,17, 0,0,0,1},
    {msg10,sizeof(msg10)-1,17,45,0,0,1}
};




/* ---------- CLEAR LINE2 ---------- */
/*--------------------------------------------------
 Function : clear_line2()
 Purpose  : Clears second line of LCD
---------------------------------------------------*/
void clear_line2()
{
    /* Move cursor to beginning of line 2 */
    CmdLCD(GOTO_LINE2_POS0);

    /* Write 16 blank spaces */
    StrLCD("                ");
}


/* ---------- ADC ---------- */ 

/*--------------------------------------------------
 Function : ADC_Read()
 Purpose  : Read ADC channel value
 Return   : 10-bit ADC result
---------------------------------------------------*/
int ADC_Read()
{
    /* Start ADC conversion */
    AD0CR |= (1<<24);

    /* Wait until conversion complete */
    while(!(AD0GDR & (1UL<<31)));

    /* Return ADC result */
    return (AD0GDR>>6)&0x3FF;
}

/*--------------------------------------------------
 Function : getKeyMapped()
 Purpose  : Convert keypad symbols
---------------------------------------------------*/

char getKeyMapped()
{
	char k= KeyScan();
	
	switch(k)
	{
		case 'A': return '+';   //+
		case 'B': return '-';   //-
		case 'C': return '*';   //*
		case 'D': return '/';    ///
		default: return k;
	}
}



/* ---------- LONG PRESS ---------- */
int LongPressDetect()
{
    int start_sec, curr_sec;
    int elapsed = 0;

    if(!(IOPIN0 & ADMIN_SW))   // button pressed
    {
        start_sec = SEC;

        while(!(IOPIN0 & ADMIN_SW))   // while holding
        {
            curr_sec = SEC;

            /* handle wrap (59 -> 0) */
            if(curr_sec >= start_sec)
                elapsed = curr_sec - start_sec;
            else
                elapsed = (60 - start_sec) + curr_sec;

            if(elapsed >= 2)   // EXACT 2 seconds
            {
                while(!(IOPIN0 & ADMIN_SW)); // wait release
                return 1;
            }
        }
    }
    return 0;
}


/* ---------- ADMIN MENU ---------- */
/*--------------------------------------------------
 Function : AdminMenu()
 Purpose  : Displays administrator menu

 Options:
    1 -> RTC Edit
    2 -> Event Edit
    3 -> Exit
---------------------------------------------------*/
void AdminMenu()
{
    char key;

    /* ---- ENTRY MESSAGE ---- */
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("Entering Admin");
    CmdLCD(0xC0);
    StrLCD("Mode...");
    delay_ms(1000);

    while(1)
    {
        CmdLCD(0x01);

        /* ---- MENU DISPLAY ---- */
        CmdLCD(0x80);
        StrLCD("1.RTC    2.EVENT");

        CmdLCD(0xC0);
        StrLCD("3.BACK");

        key = KeyScan();

        if(key=='1')
        {
            CmdLCD(0x01);
            StrLCD("RTC EDIT MODE");
            delay_ms(1000);
            RTC_Edit();
        }
        else if(key=='2')
        {
            CmdLCD(0x01);
            StrLCD("EVENT EDIT");
            delay_ms(1000);
            Event_Edit();
        }
        else if(key=='3')
        {
            CmdLCD(0x01);
            StrLCD("EXIT ADMIN MODE");
            delay_ms(1000);
            return;
        }
    }
}

/* ---------- RTC EDIT (MODIFIED) ---------- */
/*--------------------------------------------------
 Function : RTC_Edit()
 Purpose  : Modify RTC parameters

 Editable Fields:
    Hour
    Minute
    Second
    Date
    Day
    Month
    Year
---------------------------------------------------*/
void RTC_Edit()
{
    char key, k;
    int value, count;
    int valid;

    while(1)
    {
        CmdLCD(0x01);

        CmdLCD(0x80);
        StrLCD("1H  2M  3S  4DT");

        CmdLCD(0xC0);
        StrLCD("5DY 6MO 7YR 8EX");

        key = KeyScan();

        if(key == '8') return;

        if(key >= '1' && key <= '7')
        {
            count = 0;
            valid = 1;

            /* ---- LOAD CURRENT VALUE ---- */
            switch(key)
            {
                case '1': value = HOUR; break;
                case '2': value = MIN; break;
                case '3': value = SEC; break;
                case '4': value = DOM; break;
                case '5': value = DOW; break;
                case '6': value = MONTH; break;
                case '7': value = YEAR; break;
            }

            CmdLCD(0x01);

            /* ---- SHOW CURRENT VALUE ---- */
            CmdLCD(0x80);
            switch(key)
            {
                case '1': sprintf(line1,"HR:%02d  (=SAVE)",value); break;
                case '2': sprintf(line1,"MIN:%02d (=SAVE)",value); break;
                case '3': sprintf(line1,"SEC:%02d (=SAVE)",value); break;
                case '4': sprintf(line1,"DT:%02d  (=SAVE)",value); break;
                case '5': sprintf(line1,"DY:%s    (=SAVE)",dayname[value]); break;
                case '6': sprintf(line1,"MO:%02d  (=SAVE)",value); break;
                case '7': sprintf(line1,"YR:%04d  (=SAVE)",value); break;
            }
            StrLCD((u8 *)line1);

            /* ---- RANGE ---- */
            CmdLCD(0xC0);
            switch(key)
            {
                case '1': StrLCD("0-23 X:DEL +:BK"); break;
								case '2': StrLCD("0-59 X:DEL +:BK"); break;
								case '3': StrLCD("0-59 X:DEL +:BK"); break;
								case '4': StrLCD("1-31 X:DEL +:BK"); break;
								case '5': StrLCD("0-6  X:DEL +:BK"); break;
								case '6': StrLCD("1-12 X:DEL +:BK"); break;
								case '7': StrLCD("YYYY X:DEL +:BK"); break;
            }

            /* ---- INPUT LOOP ---- */
            while(1)
            {
                k = getKeyMapped();

                /* ---- BACK ---- */
                if(k == '/')
                {
                    CmdLCD(0x01);
                    StrLCD("Going Back...");
                    delay_ms(800);
                    break;
                }

                /* ---- BACKSPACE ---- */
                if(k == '-')
                {
                    if(count > 0)
                    {
                        value = value / 10;
                        count--;
                    }

                    /* Refresh display */
                    CmdLCD(0x80);
                    sprintf(line1,"                ");
                    StrLCD((u8 *)line1);

                    CmdLCD(0x80);
                    switch(key)
                    {
                        case '1': sprintf(line1,"HR:%02d  (=SAVE)",value); break;
                        case '2': sprintf(line1,"MIN:%02d (=SAVE)",value); break;
                        case '3': sprintf(line1,"SEC:%02d (=SAVE)",value); break;
                        case '4': sprintf(line1,"DT:%02d  (=SAVE)",value); break;

                        case '5':
                            if(value >= 0 && value <= 6)
                                sprintf(line1,"DY:%s   (=SAVE)",dayname[value]);
                            else
                                sprintf(line1,"DY:--   (=SAVE)");
                            break;

                        case '6': sprintf(line1,"MO:%02d  (=SAVE)",value); break;
                        case '7': sprintf(line1,"YR:%04d  (=SAVE)",value); break;
                    }
                    StrLCD((u8 *)line1);
                }

                /* ---- DIGITS ---- */
                else if(k >= '0' && k <= '9')
                {
                    if( (key == '7' && count < 4) || 
                        (key == '5' && count < 1) || 
                        (key != '7' && key != '5' && count < 2) )
                    {
                        if(count == 0) value = 0;

                        value = value*10 + (k - '0');
                        count++;

                        CmdLCD(0x80);
                        sprintf(line1,"                ");
                        StrLCD((u8 *)line1);

                        CmdLCD(0x80);
                        switch(key)
                        {
                            case '1': sprintf(line1,"HR:%02d  (=SAVE)",value); break;
                            case '2': sprintf(line1,"MIN:%02d (=SAVE)",value); break;
                            case '3': sprintf(line1,"SEC:%02d (=SAVE)",value); break;
                            case '4': sprintf(line1,"DT:%02d  (=SAVE)",value); break;

                            case '5':
                                if(value >= 0 && value <= 6)
                                    sprintf(line1,"DY:%s   (=SAVE)",dayname[value]);
                                else
                                    sprintf(line1,"DY:--   (=SAVE)");
                                break;

                            case '6': sprintf(line1,"MO:%02d  (=SAVE)",value); break;
                            case '7': sprintf(line1,"YR:%04d  (=SAVE)",value); break;
                        }
                        StrLCD((u8 *)line1);
                    }
                }

                /* ---- SAVE ---- */
                else if(k == '=')
                {
                    break;
                }
            }

            if(k == '/') continue;

            /* ---- NO INPUT ---- */
            if(count == 0)
            {
                CmdLCD(0x01);
                StrLCD("No Change Done");
                delay_ms(1000);
                continue;
            }

            CCR = 0x02;

            switch(key)
            {
                case '1': if(value <= 23) HOUR = value; else valid = 0; break;
                case '2': if(value <= 59) MIN  = value; else valid = 0; break;
                case '3': if(value <= 59) SEC  = value; else valid = 0; break;
                case '4': if(value >= 1 && value <= 31) DOM = value; else valid = 0; break;
                case '5': if(value <= 6) DOW = value; else valid = 0; break;
                case '6': if(value >= 1 && value <= 12) MONTH = value; else valid = 0; break;
                case '7': YEAR = value; break;
            }

            CCR = 0x11;

            CmdLCD(0x01);

            if(valid)
            {
                StrLCD("Saved Successfully");
                delay_ms(1500);
            }
            else
            {
                StrLCD("Invalid Value");
                delay_ms(1000);
            }
        }
    }
}

/* ---------- EVENT EDIT ---------- */
/*--------------------------------------------------
 Function : Event_Edit()
 Purpose  : Activate/Deactivate event messages

 Options:
    1 -> Activate Event
    2 -> Deactivate Event
    3 -> Exit
---------------------------------------------------*/
void Event_Edit()
{
    char key, msg_no;
    int index;

    CmdLCD(0x01);
    StrLCD("MSG NO 0-9");

    msg_no = KeyScan();

    if(msg_no=='0') index=9;
    else if(msg_no>='1' && msg_no<='9') index=msg_no-'1';
    else return;

    while(1)
    {
        CmdLCD(0x01);
        CmdLCD(0x80); StrLCD("1.ACT    2.DEACT");
        CmdLCD(0xC0); StrLCD("3.BACK");

        key = KeyScan();

        /* ---- ACTIVATE ---- */
        if(key=='1')
        {
            CmdLCD(0x01);

            if(events[index].status == 1)
            {
                StrLCD("Already Active");
            }
            else
            {
                events[index].status = 1;
                StrLCD("Activated OK");
            }

            delay_ms(1000);
        }

        /* ---- DEACTIVATE ---- */
        else if(key=='2')
        {
            CmdLCD(0x01);

            if(events[index].status == 0)
            {
                StrLCD("Already Deact");
            }
            else
            {
                events[index].status = 0;
                StrLCD("Deactivated OK");
            }

            delay_ms(1000);
        }

        /* ---- EXIT ---- */
        else if(key=='3')
        {
            CmdLCD(0x01);
            StrLCD("EXIT EVENT");
            delay_ms(1000);
            return;
        }
    }
}

/* ---------- CALCULATE END ---------- */
/*--------------------------------------------------
 Function : calc_end()
 Purpose  : Calculate event end time

 Logic:
    End Time = Start Time + 15 Minutes
---------------------------------------------------*/
void calc_end()
{
    int i;
    for(i=0;i<10;i++)
    {
        events[i].end_hr = events[i].start_hr;
        events[i].end_min = events[i].start_min + 15;

        if(events[i].end_min>=60)
        {
            events[i].end_hr++;
            events[i].end_min-=60;
        }
    }
} 

/* ---------- SCROLL ---------- */
/*--------------------------------------------------
 Function : scroll_msg()

 Purpose:
    Scroll event message on LCD

 Features:
    - Shows countdown timer
    - Supports admin entry during scrolling
    - Stops when event time expires
---------------------------------------------------*/
void scroll_msg(char *msg,int len,int sh,int sm,int eh,int em,int index)
{
    int i,j,hr,min,sec,remain;
    int total_len = len + 16;

    while(1)
    {
        /* ?? CHECK ADMIN PRESS HERE */
        if(LongPressDetect())
        {
            AdminMenu();
            return;   // exit scrolling after admin
        }

        if(events[index].status==0) return;

        hr=HOUR; min=MIN; sec=SEC;

        remain = (eh*3600+em*60) - (hr*3600+min*60+sec);
        if(remain<=0) return;

        for(i=0;i<total_len;i++)
        {
            /* ?? ALSO CHECK INSIDE FOR LOOP */
            if(LongPressDetect())
            {
                AdminMenu();
                return;
            }

            for(j=0;j<16;j++) line1[j]=' ';

            for(j=0;j<16;j++)
            {
                int k = i-(16-j);
                if(k>=0 && k<len)
                    line1[j]=msg[k];
            }

            line1[16]='\0';

            CmdLCD(GOTO_LINE1_POS0);
            StrLCD((u8 *)line1);

            remain = (eh*3600+em*60) - (HOUR*3600+MIN*60+SEC);
            if(remain<=0) return;

            sprintf(line2,"      %02d:%02d      ",remain/60,remain%60);
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD((u8 *)line2);

            delay_ms(200);
        }
    }
}


/*--------------------------------------------------
 Function : show_next_event_once()

 Purpose:
    Display next upcoming event once

 Displays:
    Line1 -> Event Time
    Line2 -> Event Message Scroll
---------------------------------------------------*/
void show_next_event_once()
{
    int i,j,k;
    int now = HOUR*60 + MIN;

    for(i=0;i<10;i++)
    {
        int event_time = events[i].start_hr*60 + events[i].start_min;

        if(events[i].status && event_time >= now)
        {
            int len = events[i].len;
            char *msg = events[i].msg;
					
						IOSET0 = RED_LED;     // RED ON
            IOCLR0 = GREEN_LED;   // GREEN OFF

            /* ---- CLEAR LCD ---- */
            CmdLCD(0x01);

            /* ---- SHOW NEXT EVENT TIME ONLY ---- */
            CmdLCD(GOTO_LINE1_POS0);
            sprintf(line1,"Next Event:%02d:%02d",
                    events[i].start_hr,
                    events[i].start_min);
            StrLCD((u8 *)line1);

            /* ---- SCROLL ONLY ONCE ---- */
            for(j=0;j<len+16;j++)
            {
                for(k=0;k<16;k++) line1[k]=' ';

                for(k=0;k<16;k++)
                {
                    int idx = j-(16-k);
                    if(idx>=0 && idx<len)
                        line1[k]=msg[idx];
                }

                line1[16]='\0';

                CmdLCD(GOTO_LINE2_POS0);
                StrLCD((u8 *)line1);

                delay_ms(150);
            }

            return;
        }
    }
}



/*--------------------------------------------------
 Function : Startup_Message()

 Purpose:
    Display welcome screen at power-up

 LCD Output:

    EVENT BOARD RTC
    DRIVEN PROJECT!!
---------------------------------------------------*/
void Startup_Message()
{
    /* Clear LCD */
    CmdLCD(0x01);

    /* Display project title on line 1 */
    CmdLCD(0x80);
    StrLCD("EVENT BOARD RTC");

    /* Display subtitle on line 2 */
    CmdLCD(0xC0);
    StrLCD("DRIVEN PROJECT!!");

    /* Keep message visible for 1 second */
    delay_ms(1000);
}

/*----------------------------------------------------------
  Function Name : CheckPassword()
  Description   : Reads a 4-digit password from the keypad,
                  displays '*' for each digit entered, compares
                  the entered password with the predefined
                  password ("1234"), and grants or denies access.

  Return Value  : 1 -> Password is correct
                  0 -> Password is incorrect
----------------------------------------------------------*/
int CheckPassword(void)
{
    char pass[5];   // Array to store the entered password (+1 for '\0')
    int i;          // Loop counter

    /* Clear LCD and prompt user to enter password */
    CmdLCD(0x01);
    StrLCD((u8*)"Enter Password");

    /* Move cursor to second line */
    CmdLCD(0xC0);

    /* Read 4 digits from keypad */
    for(i = 0; i < 4; i++)
    {
        pass[i] = KeyScan();   // Get one key from keypad

        /* Display '*' instead of actual digit for security */
        CharLCD('*');
    }

    /* Append null character to make it a valid string */
    pass[4] = '\0';

    /* Compare entered password with predefined password */
    if(strcmp(pass, "1234") == 0)
    {
        /* Password matched */
        CmdLCD(0x01);                    // Clear LCD
        StrLCD((u8*)"Access Granted");   // Display success message
        delay_ms(1000);                  // Show message for 1 second

        return 1;                        // Indicate password is correct
    }
    else
    {
        /* Password mismatch */
        return 0;                        // Indicate password is incorrect
    }
}
