#include "watch_clock.h"
#include "lvgl.h"
#include "ch32v30x.h"
#include "debug.h"

#define CLOCK_START_SECONDS (12U * 3600U)
#define DIAL_SIZE 218
#define DIAL_CENTER (DIAL_SIZE / 2)

u8 RTC_Init(void);
u8 Is_Leap_Year(u16 year);
u8 RTC_Alarm_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec);
u8 RTC_Get(void);
u8 RTC_Get_Week(u16 year, u8 month, u8 day);
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec);

typedef struct
{
    vu8 hour;
    vu8 min;
    vu8 sec;

    vu16 w_year;
    vu8  w_month;
    vu8  w_date;
    vu8  week;
} _calendar_obj;

_calendar_obj calendar;

static uint32_t clock_start_tick;
static lv_obj_t * hour_line;
static lv_obj_t * minute_line;
static lv_obj_t * second_line;

/* LVGL keeps these arrays by pointer, so they must outlive the line objects. */
static lv_point_precise_t hour_points[2] = {{DIAL_CENTER, DIAL_CENTER}, {DIAL_CENTER, DIAL_CENTER - 46}};
static lv_point_precise_t minute_points[2] = {{DIAL_CENTER, DIAL_CENTER}, {DIAL_CENTER, DIAL_CENTER - 70}};
static lv_point_precise_t second_points[2] = {{DIAL_CENTER, DIAL_CENTER}, {DIAL_CENTER, DIAL_CENTER - 82}};

u8 const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
const u8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

u8 RTC_Init(void)
{
    uint8_t temp = 0;
    uint8_t first_init;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    // BKP_DeInit();
    RCC_LSICmd(ENABLE);
    // RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp < 250)
    {
        temp++;
        Delay_Ms(20);
    }
    if(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
        return 1;

    first_init = (BKP_ReadBackupRegister(BKP_DR1) != 0xA1A1);
    if(first_init)
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForLastTask();
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    /* RTC_EnterConfigMode();
    RTC_SetPrescaler(39999);
    RTC_WaitForLastTask();
    //RTC_Set(2026, 9, 27, 10, 58, 55);
    RTC_ExitConfigMode();
    BKP_WriteBackupRegister(BKP_DR1, 0XA1A1);
    return 0; */
    if (first_init) {
        RTC_SetPrescaler(39999);
        RTC_WaitForLastTask();

        if (RTC_Set(2026, 9, 27, 10, 58, 55) != 0)
            return 1;

        BKP_WriteBackupRegister(BKP_DR1, 0xA1A1);
    }
    return 0;
}

u8 Is_Leap_Year(u16 year)
{
    if(year % 4 == 0)
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)
                return 1;
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}

u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if(syear < 1970 || syear > 2099)
        return 1;
    for(t = 1970; t < syear; t++)
    {
        if(Is_Leap_Year(t))
            seccount += 31622400;
        else
            seccount += 31536000;
    }
    smon -= 1;
    for(t = 0; t < smon; t++)
    {
        seccount += (u32)mon_table[t] * 86400;
        if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400;
    }
    seccount += (u32)(sday - 1) * 86400;
    seccount += (u32)hour * 3600;
    seccount += (u32)min * 60;
    seccount += sec;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RTC_SetCounter(seccount);
    RTC_WaitForLastTask();
    return 0;
}

u8 RTC_Get(void)
{
    static u16 daycnt = 0;
    u32        timecount = 0;
    u32        temp = 0;
    u16        temp1 = 0;
    timecount = RTC_GetCounter();
    temp = timecount / 86400;
    if(daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))
            {
                if(temp >= 366)
                    temp -= 366;
                else
                {
                    break;
                }
            }
            else
                temp -= 365;
            temp1++;
        }
        calendar.w_year = temp1;
        temp1 = 0;
        while(temp >= 28)
        {
            if(Is_Leap_Year(calendar.w_year) && temp1 == 1)
            {
                if(temp >= 29)
                    temp -= 29;
                else
                    break;
            }
            else
            {
                if(temp >= mon_table[temp1])
                    temp -= mon_table[temp1];
                else
                    break;
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1;
        calendar.w_date = temp + 1;
    }
    temp = timecount % 86400;
    calendar.hour = temp / 3600;
    calendar.min = (temp % 3600) / 60;
    calendar.sec = (temp % 3600) % 60;
    calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date);
    return 0;
}

u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
    u16 temp2;
    u8  yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    if(yearH > 19)
        yearL += 100;
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if(yearL % 4 == 0 && month < 3)
        temp2--;
    return (temp2 % 7);
}

static lv_obj_t * create_hand(lv_obj_t * parent, lv_point_precise_t points[2],
                              int32_t width, uint32_t color)
{
    lv_obj_t * line = lv_line_create(parent);
    if(line == NULL) return NULL;

    lv_obj_set_size(line, DIAL_SIZE, DIAL_SIZE);
    lv_obj_center(line);
    lv_obj_set_style_line_color(line, lv_color_hex(color), 0);
    lv_obj_set_style_line_width(line, width, 0);
    lv_obj_set_style_line_rounded(line, true, 0);
    lv_line_set_points(line, points, 2);
    return line;
}

static void set_hand(lv_obj_t * line, lv_point_precise_t points[2],
                     int32_t length, int16_t angle)
{
    points[1].x = DIAL_CENTER + length * lv_trigo_sin(angle) / LV_TRIGO_SIN_MAX;
    points[1].y = DIAL_CENTER - length * lv_trigo_cos(angle) / LV_TRIGO_SIN_MAX;
    lv_line_set_points(line, points, 2);
}

static void watch_clock_update(lv_timer_t * timer)
{
    uint32_t elapsed;
    uint32_t time_of_day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;

    time_of_day = RTC_GetCounter() % 86400U;
    hour   = time_of_day / 3600U;
    minute = (time_of_day / 60U) % 60U;
    second = time_of_day % 60U;

    set_hand(hour_line, hour_points, 46, (int16_t)((hour % 12U) * 30U + minute / 2U));
    set_hand(minute_line, minute_points, 70, (int16_t)(minute * 6U + second / 10U));
    set_hand(second_line, second_points, 82, (int16_t)(second * 6U));
}

void watch_clock_init(void)
{
    lv_obj_t * screen = lv_screen_active();
    lv_obj_t * dial = lv_obj_find_by_name(screen, "dial_inner");
    lv_obj_t * hub = lv_obj_find_by_name(screen, "hand_hub");
    lv_obj_t * old_hour = lv_obj_find_by_name(screen, "hand_hour");
    lv_obj_t * old_minute = lv_obj_find_by_name(screen, "hand_minute");
    lv_obj_t * old_second = lv_obj_find_by_name(screen, "hand_second");

    if(dial == NULL || hub == NULL || old_hour == NULL || old_minute == NULL || old_second == NULL) return;

    hour_line = create_hand(dial, hour_points, 7, 0xD9DCE1);
    minute_line = create_hand(dial, minute_points, 5, 0xF4F5F6);
    second_line = create_hand(dial, second_points, 2, 0xED1B2F);
    if(hour_line == NULL || minute_line == NULL || second_line == NULL) {
        if(hour_line != NULL) lv_obj_delete(hour_line);
        if(minute_line != NULL) lv_obj_delete(minute_line);
        if(second_line != NULL) lv_obj_delete(second_line);
        hour_line = minute_line = second_line = NULL;
        return;
    }

    lv_obj_set_hidden(old_hour, true);
    lv_obj_set_hidden(old_minute, true);
    lv_obj_set_hidden(old_second, true);
    lv_obj_move_to_index(hub, -1);

    clock_start_tick = lv_tick_get();
    watch_clock_update(NULL);
    lv_timer_create(watch_clock_update, 1000, NULL);
}
