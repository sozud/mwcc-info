/*  Metrowerks Standard Library  */

/*  $Date: 1999/02/03 19:45:31 $ 
 *  $Revision: 1.4 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	time.win32.c
 *	
 *	Routines
 *	--------
 *		__get_clock
 *		__get_time
 *
 *		__to_gm_time
 *
 */

/* #include <x86_prefix.h>*/
#include <stdlib.h>
/*
hh 980122 Replaced <windows.h> with the following TWO includes because it is seriously
          broken.  The following 2 includes must be carefully ordered as shown, because
          they are broken too.
#include <windows.h>
*/
#include <WINDEF.H>
#include <WINBASE.H>

#include <time.h>

#include "assert.h"

time_t __get_time(void)
{
  SYSTEMTIME time;
	long years, leap_years, leap_days, days, i, secs;

	static int MonthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
							/*jan feb mar apr may jun jul aug sep oct nov dec */

	/* First, get the time from WIN32 */

	GetLocalTime(&time);

	/* Calculate the number of full years */

	years = time.wYear - 1970;		// 961010 KO */

	/* Calculate the number of leap years in that time */

	leap_years = years;
	if (time.wMonth > 2)
	{
	    leap_years++;
	}
	leap_days = (leap_years + 1) / 4;


	/* Calculate total days */

	days = (years * 365) + leap_days;

	/* Calculate days in this year */

	for (i=1; i<time.wMonth; i++)
	{
	    days += MonthDays[i-1];
	}
	days += time.wDay - 1;
	
	/* and calculate the seconds */

	secs =  (((((days * 24) + time.wHour) * 60) + time.wMinute) * 60) + time.wSecond;
	
	return secs;
	
}

clock_t __get_clock(void)
{
/* Same as __get_time but with millisecs resulotion. */

  SYSTEMTIME time;
	long years, leap_years, leap_days, days, i, secs, msecs;

	static int MonthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
							/*jan feb mar apr may jun jul aug sep oct nov dec */

	/* First, get the time from WIN32 */

	GetLocalTime(&time);

	/* Calculate the number of full years */

	years = time.wYear - 1970;		/* 961010 KO */	

	/* Calculate the number of leap years in that time */

	leap_years = years;
	if (time.wMonth > 2)
	{
	    leap_years++;
	}
	leap_days = (leap_years + 1) / 4;


	/* Calculate total days */

	days = (years * 365) + leap_days;

	/* Calculate days in this year */

	for (i=1; i<time.wMonth; i++)
	{
	    days += MonthDays[i-1];
	}
	days += time.wDay - 1;
	
	/* and calculate the seconds */

	secs =  (((((days * 24) + time.wHour) * 60) + time.wMinute) * 60) + time.wSecond;
	
	msecs = secs * 1000 + time.wMilliseconds;
	
	return msecs;
}

int __to_gm_time(time_t * time)
{
	TIME_ZONE_INFORMATION tzi;

	if (GetTimeZoneInformation(&tzi) == TIME_ZONE_ID_UNKNOWN)
#if __dest_os==__win32_os
		return 0;
#else
	     ;
#endif
	*time += (tzi.Bias * 60);
	return(1);
}

/*  Change Record
 *	10-Oct-96 KO   Changed so that time is measured from 1970 rather than 1900. 
 * hh 980122 Replaced <windows.h> with the following TWO includes because it is seriously
          broken.  The following 2 includes must be carefully ordered as shown, because
          they are broken too.
 * mf 980624 don't include x86_prefix.h
 */
