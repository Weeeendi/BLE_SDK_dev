/**
 * @file vl_time.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,if you need RTC function,please enable VL_USE_RTC
 * @version 1.0.0
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "vl_time.h"
#include "vl_type.h"

#if VL_USE_RTC

#define SEC_PER_HOUR 3600 
#define FOURYEARDAY (365+365+365+366)  //4年一个周期内的总天数（1970~2038不存在2100这类年份，故暂不优化）
#define TIMEZONE    (8)                //时区调整,默认北京时间

static uint8_t month_day[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //平年 
static uint8_t Leap_month_day[12]={31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //闰年 

static UINT32 unix_timestamp = 0;

/**
 * @brief 判断是否是闰年
 * 
 * @param year 
 * @return uint8_t 1:闰年 0:平年
 */
static uint8_t isLeapYear(uint16_t year)
{
	uint8_t res=0;
	
	if(year%4 == 0) // 能够被4整除 
	{
		if((year%100 == 0) && (year%400 != 0))	//能够被100整除，但是不能够被400整除 
		{
			res = 0;
		}
		else
		{
			res =1;
		}
	}
	return res;
}

void covUnixTimeStp2Rtc(uint32_t unixTime, rtc_time_t *rtcTime,bool daylightSaving)
{
    uint32_t totleDayNum=0, totleSecNum=0;
    uint16_t remainDayofYear=0,tempYear=0;
    uint8_t *pr=NULL;
    int TimezoneCaled = 0;
    
    //夏令时
    if (daylightSaving) {
        unixTime += SEC_PER_HOUR;
    } 

    totleDayNum = unixTime/(24*60*60); //总天数(注意加括号)
    totleSecNum = unixTime%(24*60*60); //当天剩余的秒速
 
    memset(rtcTime, 0x00, sizeof(rtc_time_t));
    
    // 1.先计算时间 HH:MM:SS
    rtcTime->ui8Hour = totleSecNum/3600;
    rtcTime->ui8Minute = (totleSecNum%3600)/60; 
    rtcTime->ui8Second = (totleSecNum%3600)%60;
 
    // 2.对时间进行时区调整（注意：这里可能造成日期 +1 或 -1）
    TimezoneCaled = rtcTime->ui8Hour;
    TimezoneCaled += rtcTime->time_zone;
    
    if(TimezoneCaled>23){
        //printf("modify day..\n");
        rtcTime->ui8Hour = TimezoneCaled - 24;
        remainDayofYear++;  // 日期+1
    }else if(TimezoneCaled<0){
        rtcTime->ui8Hour = 0 - (TimezoneCaled);
        remainDayofYear--; //日期-1
    }

    if(rtcTime->ui8Hour)
 
    // 3.计算哪一年
    rtcTime->ui16Year = 1970 + (totleDayNum / FOURYEARDAY) * 4;   // 4年为一个周期
    remainDayofYear += totleDayNum % FOURYEARDAY;
    
    tempYear = isLeapYear(rtcTime->ui16Year)?366:365;
    while(remainDayofYear >= tempYear)  // 计算4年整数倍外的年。
    {
        rtcTime->ui16Year++;
        remainDayofYear -= tempYear;
        tempYear = isLeapYear(rtcTime->ui16Year)?366:365;
    }
    
    // 4.计算哪一月的哪一天
    pr = isLeapYear(rtcTime->ui16Year)?Leap_month_day:month_day;
    remainDayofYear++;          // 这里开始计算具体日期。remainDayofYear为 0 时其实是 1 号，所以这里要 +1
    while(remainDayofYear > *(pr+rtcTime->ui8Month))
    {
		remainDayofYear -= *(pr+rtcTime->ui8Month);
        rtcTime->ui8Month++;
    }

    rtcTime->ui8Month++; //month
    rtcTime->ui8DayOfMonth = remainDayofYear;  //day
}

uint32_t covRtc2UnixTimeStp(rtc_time_t *rtcTime,bool daylightSaving )
{
	uint32_t daynum=0, SecNum=0; //保存时间到起始时间的天数
	uint16_t tempYear=1970, tempMonth=0;
    uint8_t dst = 1;
 
	//1.年的天数 
	while(tempYear < rtcTime->ui16Year) 
	{
		if(isLeapYear(tempYear)){
			daynum += 366;
		}
		else{
			daynum += 365;
		}
		tempYear++;
	}
	//2.月的天数
 	while(tempMonth < rtcTime->ui8Month-1) 
 	{
        if(isLeapYear(rtcTime->ui16Year)){ //闰年
            daynum += Leap_month_day[tempMonth];
        }
        else{
		    daynum += month_day[tempMonth];
        }
		tempMonth++;
	}
    //3.天数
	daynum += (rtcTime->ui8DayOfMonth-1);
 
    //4.时分秒
    SecNum = daynum*24*60*60; //s    
    SecNum += rtcTime->ui8Hour*60*60;    
    SecNum += rtcTime->ui8Minute*60;    
    SecNum += rtcTime->ui8Second;
 
    //5.时区调整
    SecNum -= rtcTime->time_zone*60*60;
    if (dst && daylightSaving) {
        SecNum -= SEC_PER_HOUR;
    }
    return SecNum;
}

void set_rtc_time(rtc_time_t LocalTime){
    return rtc_set_time(&LocalTime);
}

uint32_t get_UnixTimeStamp(bool daylightSaving){
    rtc_time_t rtc_param;
    rtc_get_time(&rtc_param);
    return covRtc2UnixTimeStp(&rtc_param,daylightSaving);
}



#endif