#ifndef MINI_PROJECT_FUNCTIONS_H
#define MINI_PROJECT_FUNCTIONS_H

#include <lpc214x.h>

/* ---------- SWITCH ---------- */
#define ADMIN_SW   (1<<25)

/* ---------- LED ---------- */
#define RED_LED   (1<<23)
#define GREEN_LED (1<<24)

/* ---------- DAY ---------- */
extern char *dayname[7];

/* ---------- LCD BUFFERS ---------- */
extern char line1[21];
extern char line2[21];

extern int prev_temp;
extern int last_min;
extern int prev_temp;
extern int last_min;

/* ---------- EVENT STRUCTURE ---------- */
typedef struct
{
    char *msg;
    int len;
    int start_hr;
    int start_min;
    int end_hr;
    int end_min;
    int status;
} EVENT;

/* ---------- GLOBAL VARIABLES ---------- */
extern EVENT events[10];

/* ---------- FUNCTION DECLARATIONS ---------- */
char getKeyMapped(void);
void clear_line2(void);
int ADC_Read(void);
int LongPressDetect(void);
void AdminMenu(void);
void RTC_Edit(void);
void Event_Edit(void);
void calc_end(void);
void scroll_msg(char *msg,int len,int sh,int sm,int eh,int em,int index);
void show_next_event_once(void);
void Startup_Message(void);
int CheckPassword(void);

#endif

