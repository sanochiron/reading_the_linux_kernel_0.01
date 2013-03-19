#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <sys/types.h>

#define _LOW(v)		( (v) & 0377)
#define _HIGH(v)	( ((v) >> 8) & 0377)

/* options for waitpid, WUNTRACED not supported */
/* waitpid のオプション、WUNTRACEDはｻﾎﾟｰﾄされない */
#define WNOHANG		1
#define WUNTRACED	2

#define WIFEXITED(s)	(!((s)&0xFF)		/* 正常に終了した	*/
#define WIFSTOPPED(s)	(((s)&0xFF)==0x7F)	/* シグナルで終了した	*/
#define WEXITSTATUS(s)	(((s)>>8)&0xFF)		/* 終了ステータス取得	*/
#define WTERMSIG(s)	((s)&0x7F)		/* 終了シグナルの取得	*/
#define WSTOPSIG(s)	(((s)>>8)&0xFF)		/* 停止シグナルの取得	*/

/* シグナルで終了した？ */
#define WIFSIGNALED(s)	(((unsigned int)(s)-1 & 0xFFFF) < 0xFF)

pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);

#endif
