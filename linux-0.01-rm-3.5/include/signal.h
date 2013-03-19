#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/types.h>

/* 操作に不可分性(atomicity)を与え、命令語の操作を互いに不可分(atomic)にする。途中で別の命令の効果が入らない */
typedef int sig_atomic_t;		/* アトミックな操作を保証 - 単一のマシン語命令で更新可能なメモリ幅 */
typedef unsigned int sigset_t;		/* 32 bits */

#define _NSIG             32		/* 総数	*/
#define NSIG		_NSIG

#define SIGHUP		 1		/* Hangup			 */	/* 制御端末かﾟﾛｾｽのﾊﾝｸﾞｱｯﾌﾟ	*/
#define SIGINT		 2		/* Interrupt from keyboard	 */	/* キーボードからの割り込み　^C	*/
#define SIGQUIT		 3		/* Quit from keyboard		 */	/* キーボードからの終了操作  ^\	*/
#define SIGILL		 4		/* Illegal Instruction		 */	/* 不正なハードウェア命令		*/
#define SIGTRAP		 5		/* Trace/breakpoint trap	 */	/* ﾄﾗｯﾌﾟﾌｫｰﾙﾄ(ﾌﾞﾚｰｸﾎﾟｲﾝﾄ)	*/
#define SIGABRT		 6		/* Abort signal from abort()	 */	/* 異常な操作(abort)		*/
#define SIGIOT		 6		/* IO trap.A synonym for SIGABRT */	/* PDP-11時代のSIGABRT		*/
#define SIGUNUSED	 7		/* Unused signal(will be SIGSYS) */	/* 意図的に空けてあるｼｸﾞﾅﾙ	*/
#define SIGFPE		 8		/* Floating point exception	 */	/* 浮動小数点演算例外		*/
#define SIGKILL		 9		/* Kill signal			 */	/* プロセスの強制終了		*/
#define SIGUSR1		10		/* User-defined signal 1	 */	/* ﾕｰｻﾞ定義のｼｸﾞﾅﾙ #1		*/
#define SIGSEGV		11		/* segmentation violation	 */	/* セグメンテーション例外		*/
#define SIGUSR2		12		/* User-defined signal 2	 */	/* ﾕｰｻﾞ定義のｼｸﾞﾅﾙ #2		*/
#define SIGPIPE		13		/* write to pipe with no readers */	/* 読み手のいないﾊﾟｲﾌﾟに出力	*/
#define SIGALRM		14		/* Timer signal from alarm()	 */	/* タイマーアラームの通知(alarm)	*/
#define SIGTERM		15		/* Termination signal		 */	/* kill()から終了シグナル検知	*/
#define SIGSTKFLT	16		/* Stack fault on coprocessor	 */	/* コプロセッサのｽﾀｯｸﾌｫｰﾙﾄ	*/
#define SIGCHLD		17		/* Child stopped or terminated	 */	/* 子プロセスが終了または停止	*/

/* POSIX は 例えサポートされていない場合でも以下のシグナルの定義を要求しています。	*/
#define SIGCONT		18		/* Continue if stopped			*/	/* 停止状態から実行再開	*/
#define SIGSTOP		19		/* Stop process				*/	/* プロセスの実行停止	*/
#define SIGTSTP		20		/* Stop typed at tty			*/	/* プロセスの停止要求	*/
#define SIGTTIN		21		/* tty input for background process 	*/	/* ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞﾌﾟﾛｾｽの入力要求	*/
#define SIGTTOU		22		/* tty output for background process	*/	/* ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞﾌﾟﾛｾｽの出力要求	*/

/* Ok, I haven't implemented sigactions, but trying to keep headers POSIX */
/* Ok, 僕はまだ実装してないけど、その時のためにPOSIX Headersは残しておこう	  */
#define SA_NOCLDSTOP	1	/*子ﾌﾟﾛｾｽの停止時、親ﾌﾟﾛｾｽにSIGCHLDｼｸﾞﾅﾙを送らない*/

/* POSIX はsigprocmask()を使う際にこれらの値を要求する		*/
#define SIG_BLOCK          0	/* for blocking signals		*/	/* 無視するシグナル	*/
#define SIG_UNBLOCK        1	/* for unblocking signals	*/	/* 無視しないシグナル	*/
#define SIG_SETMASK        2	/* for setting the signal mask	*/	/* シグナルマスク設定	*/

/* 関数ポインタとして使うマクロ						  */
#define SIG_DFL		((void (*)(int))0)	/* default signal handling */	/* 標準のシグナルハンドリング	*/
#define SIG_IGN		((void (*)(int))1)	/* ignore signal */		/* シグナルを無視した際の処理	*/

struct sigaction {
	void (*sa_handler)(int);	/* SIG_DFL, SIG_IGN もしくはハンドラへの関数ポインタ	*/
	sigset_t sa_mask;		/* ハンドラ処理中ブロックされるシグナル			*/
	int sa_flags;			/* SA_NOCLDSTOP : 特殊なフラグ				*/
};

void (*signal(int _sig, void (*_func)(int)))(int);
int raise(int sig);
int kill(pid_t pid, int sig);
int sigaddset(sigset_t *mask, int signo);
int sigdelset(sigset_t *mask, int signo);
int sigemptyset(sigset_t *mask);
int sigfillset(sigset_t *mask);
int sigismember(sigset_t *mask, int signo); /* 1 - is, 0 - not, -1 error */
int sigpending(sigset_t *set);
int sigprocmask(int how, sigset_t *set, sigset_t *oldset);
int sigsuspend(sigset_t *sigmask);
int sigaction(int sig, struct sigaction *act, struct sigaction *oldact);

#endif /* _SIGNAL_H */
