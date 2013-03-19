#ifndef _TIME_H
#define _TIME_H

#ifndef _TIME_T
#define _TIME_T
typedef long time_t;		/*  UTC[1970年01月01日00時00分00秒]から今への経過秒数 */
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#define CLOCKS_PER_SEC 100	/* system timer_interrupt = 一秒間のtick刻み = 0.01 */

typedef long clock_t;		/* clock_t clock() / CLOCKS_PER_SEC = 経過秒数	   */

struct tm {
	int tm_sec;		/* second	: 秒 (0-61)	*/
	int tm_min;		/* minute	: 分 (0~59)	*/
	int tm_hour;		/* hour		: 時 (0~23)	*/
	int tm_mday;		/* Month Day	: 日 (1~31)	*/
	int tm_mon;		/* month	: 月 (0~11)	*/
	int tm_year;		/* year		: 年 1900年以降	*/
	int tm_wday;		/* Week Day	: 曜日 : (0~6)	*/
	int tm_yday;		/* Year Day	: 1月1日からの通算日 : (0~365)	*/
	int tm_isdst;		/* Is Daylight Saving Timee : 季節時間ﾌﾗｸﾞ	*/
};

clock_t clock(void);
time_t time(time_t * tp);
double difftime(time_t time2, time_t time1);
time_t mktime(struct tm * tp);

char * asctime(const struct tm * tp);
char * ctime(const time_t * tp);
struct tm * gmtime(const time_t *tp);
struct tm *localtime(const time_t * tp);
size_t strftime(char * s, size_t smax, const char * fmt, const struct tm * tp);
void tzset(void);

#endif