#ifndef _UTIME_H
#define _UTIME_H

#include <sys/types.h>	/* I know - shouldn't do this, but ..	*/
			/* 僕は知ってる - しなくともいい, しかし..	*/

struct utimbuf {
	/*  UCT[1970年01月01日00時00分00秒]から今への経過秒数	*/
	time_t actime;	/* 参照時刻 */
	time_t modtime;	/* 修正時刻 */
};

extern int utime(const char *filename, struct utimbuf *times);

#endif