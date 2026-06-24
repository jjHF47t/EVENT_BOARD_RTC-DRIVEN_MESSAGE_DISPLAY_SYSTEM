#include <lpc214x.h>                 // LPC2148 register definitions
#include <stdio.h>                   // sprintf()

#include "lcd.h"                     // LCD functions
#include "lcd_defines.h"             // LCD command macros
#include "delay.h"                   // Delay functions
#include "KPM.h"                     // Keypad functions
#include "mini_project_functions.h"  // Project functions
#include "rtc.h"                     // RTC functions and macros

#define DEBUG                        // Debug macro

int main()
{
    /* Variables for RTC values */
    int hr,min,sec;                  // Hour, minute, second
    int day,date,month,year;         // Day, date, month, year

    /* Variables for temperature */
    int adc,temp;

    /* Loop variables */
    int i,found;

    /* ---------- LCD INITIALIZATION ---------- */

    InitLCD();                       // Initialize LCD

    InitKPM();                       // Initialize keypad

    delay_ms(100);                   // Wait for LCD stabilization

    Startup_Message();               // Show startup scrolling message

    /* ---------- GPIO CONFIGURATION ---------- */

    PINSEL0 = 0;                     // Configure Port0 pins as GPIO
    PINSEL1 = 0;                     // Configure Port1 pins as GPIO
    PINSEL2 = 0;                     // Configure Port2 pins as GPIO

    IODIR0 &= ~ADMIN_SW;             // Admin switch as input

    IODIR0 |= RED_LED | GREEN_LED;   // RED and GREEN LEDs as outputs

    /* ---------- ADC CONFIGURATION ---------- */

    PINSEL1 |= (1<<24);              // Configure P0.28 as AD0.1

    AD0CR = (1<<1) |                // Select ADC channel 1
            (4<<8) |                // ADC clock divider
            (1<<21);                // ADC enable

    /* ---------- RTC CONFIGURATION ---------- */

    PREINT  = 0x01C8;                // RTC integer prescaler

    PREFRAC = 0x61C0;                // RTC fractional prescaler

    RTC_Init();                      // Initialize RTC

    /* ---------- CALCULATE EVENT END TIMES ---------- */

    calc_end();                      // Add 15 mins to each event

    /* ---------- MAIN LOOP ---------- */

    while(1)
    {
        /* Read current RTC values */

        hr    = HOUR;                // Read hour
        min   = MIN;                 // Read minute
        sec   = SEC;                 // Read second

        day   = DOW;                 // Read day of week
        date  = DOM;                 // Read date
        month = MONTH;               // Read month
        year  = YEAR;                // Read year

        /* ---------- TEMPERATURE READING ---------- */

        adc = ADC_Read();            // Read ADC value

        temp = (adc * 330) / 1023;   // Convert ADC to temperature

        /* Default LED indication */

        IOSET0 = GREEN_LED;          // Green LED ON

        IOCLR0 = RED_LED;            // Red LED OFF

        /* ---------- NEXT EVENT DISPLAY ---------- */

        /* Execute once whenever minute changes */

        if(MIN != last_min && SEC == 0)
        {
            int event_active = 0;    // Assume no event is active

            /* Check whether any event is running */

            for(i=0;i<10;i++)
            {
                if(events[i].status &&
                   (hr==events[i].start_hr &&
                    min>=events[i].start_min) &&
                   ((hr<events[i].end_hr) ||
                   (hr==events[i].end_hr &&
                    min<events[i].end_min)))
                {
                    event_active = 1; // Event found active

                    break;
                }
            }

            /* Show next event only if no event is active */

            if(!event_active)
            {
                show_next_event_once();

                prev_temp = -1;
            }

            /* Update last minute */

            last_min = MIN;
        }

        /* ---------- EVENT CHECK ---------- */

        found = 0;                   // No active event found yet

        for(i=0;i<10;i++)
        {
            if(events[i].status &&
              (hr==events[i].start_hr &&
               min>=events[i].start_min) &&
              ((hr<events[i].end_hr) ||
              (hr==events[i].end_hr &&
               min<events[i].end_min)))
            {
                /* Event currently running */

                IOSET0 = GREEN_LED;  // Green LED ON

                IOCLR0 = RED_LED;    // Red LED OFF

                clear_line2();       // Clear second LCD line

                /* Scroll event message */

                scroll_msg(
                           events[i].msg,
                           events[i].len,
                           events[i].start_hr,
                           events[i].start_min,
                           events[i].end_hr,
                           events[i].end_min,
                           i
                          );

                found = 1;           // Active event found

                break;
            }
        }

        /* ---------- NORMAL RTC DISPLAY ---------- */

        if(!found)
        {
            /* No event running */

            IOSET0 = RED_LED;        // Red LED ON

            IOCLR0 = GREEN_LED;      // Green LED OFF

            /* Display time and day */

            CmdLCD(0x80);            // Move cursor to line1

            sprintf(line1,
                    "%02d:%02d:%02d     %-3s",
                    hr,min,sec,
                    dayname[day]);

            StrLCD((u8*)line1);

            /* Display date and temperature */

            CmdLCD(0xC0);            // Move cursor to line2

            sprintf(line2,
                    "%02d/%02d/%04d  %2d%cC",
                    date,
                    month,
                    year,
                    temp,
                    0xDF);           // Degree symbol

            StrLCD((u8*)line2);

            delay_ms(200);           // Refresh delay
        }

        /* ---------- ADMIN MODE ---------- */

        if(LongPressDetect())        // Check long press
        {
            AdminMenu();             // Enter admin mode
        }
    }
}

