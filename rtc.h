#ifndef RTC_H
#define RTC_H

#include "rtc_defines.h"

/* ---------- TYPE DEFINITIONS ---------- */
typedef signed int s32;
typedef unsigned int u32;

/* ---------- GLOBAL VARIABLES ---------- */
extern s32 hour, min, sec, date, month, year, day;

/* ---------- FUNCTION DECLARATIONS ---------- */
void RTC_Init(void);

void GetRTCTimeInfo(s32 *, s32 *, s32 *);
void DisplayRTCTime(u32, u32, u32);

void GetRTCDateInfo(s32 *, s32 *, s32 *);
void DisplayRTCDate(u32, u32, u32);

void SetRTCTimeInfo(u32, u32, u32);
void SetRTCDateInfo(u32, u32, u32);

void GetRTCDay(s32 *);
void DisplayRTCDay(u32);
void SetRTCDay(u32);

#endif


