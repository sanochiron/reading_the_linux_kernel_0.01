#ifndef _TIMES_H
#define _TIMES_H

#include <sys/types.h>

struct tms {
	time_t tms_utime;	/* ユーザのCPU時間	*/
	time_t tms_stime;	/* カーネルのCPU時間	*/
	time_t tms_cutime;	/* 子プロセス:ユーザCPU時間	*/
	time_t tms_cstime;	/* 子プロセスのシステム時間	*/
};

extern time_t times(struct tms * tp);

#endif
