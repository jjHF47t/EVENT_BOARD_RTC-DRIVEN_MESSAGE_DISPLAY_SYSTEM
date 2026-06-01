#ifndef RTC_DEFINES_H
#define RTC_DEFINES_H

#include <lpc214x.h>

/* ---------- CLOCK MACROS ---------- */
#define FOSC 12000000
#define CCLK (5*FOSC)    
#define PCLK (CCLK/4)

/* ---------- RTC PRESCALER ---------- */
#define PREINT_VAL (int)((PCLK / 32768) - 1)
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))

/* ---------- RTC CONTROL BITS ---------- */
#define RTC_ENABLE  (1<<0)
#define RTC_RESET   (1<<1)
#define RTC_CLKSRC  (1<<4)

/* ---------- DAY DEFINES ---------- */
#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

#endif

