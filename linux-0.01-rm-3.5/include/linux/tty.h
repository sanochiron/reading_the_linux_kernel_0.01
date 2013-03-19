/*
 * 'tty.h' defines some structures used by tty_io.c and some defines.
 *
 * NOTE! Don't touch this without checking that nothing in rs_io.s or
 * con_io.s breaks. Some constants are hardwired into the system (mainly
 * offsets into 'tty_queue'
 */
/*
 * 'tty.h' tty_io.cで使用される幾つかの構造体と幾つかの定義がされています。
 * 注意! rs_io.sとcon_io.sに問題がない箏を確認しないで触れないでください。
 * 幾つかの定数はシステム('tty_queue'内の主要オフセット)にハードワイヤー
 * ドされています。
 */

#ifndef _TTY_H
#define _TTY_H

#include <termios.h>

#define TTY_BUF_SIZE 1024	/* TTY キューのサイズ	*/

/* キュー(待ち行列)による保持:tty_structの入出力処理を行う	*/
struct tty_queue {
	unsigned long data;			/* 文字	*/
	unsigned long head;			/* 始端	*/
	unsigned long tail;			/* 終端	*/
	struct task_struct * proc_list;	/* プロセス一覧	*/
	char buf[TTY_BUF_SIZE];		/* バッファ	*/
};

/* 以下のマクロはキュー自体に対する操作を行う	*/
#define INC(a) ((a) = ((a)+1) & (TTY_BUF_SIZE-1))		/* 次の1バイトを指定 */
#define DEC(a) ((a) = ((a)-1) & (TTY_BUF_SIZE-1))		/* 前の1バイトを指定 */
#define EMPTY(a) ((a).head == (a).tail)				/* キューは空か判定  */
#define LEFT(a) (((a).tail-(a).head-1)&(TTY_BUF_SIZE-1))	/* キューの長さ取得  */
#define LAST(a) ((a).buf[(TTY_BUF_SIZE-1)&((a).head-1)])	/* バッファ終端取得  */
#define FULL(a) (!LEFT(a))					/* バッファ満杯判定  */
#define CHARS(a) (((a).head-(a).tail)&(TTY_BUF_SIZE-1))		/* 文字長を取得する  */

/* キューから文字取得 */
#define GETCH(queue,c) \
(void)({c=(queue).buf[(queue).tail];INC((queue).tail);})
/* キューから文字挿入 */
#define PUTCH(c,queue) \
(void)({(queue).buf[(queue).head]=(c);INC((queue).head);})

#define EOF_CHAR(tty) ((tty)->termios.c_cc[VEOF])		/* VEOF:ファイル終端		*/
#define INTR_CHAR(tty) ((tty)->termios.c_cc[VINTR])		/* INTR:割り込み			*/
#define STOP_CHAR(tty) ((tty)->termios.c_cc[VSTOP])		/* VSTOP:出力の停止(フロー制御)	*/
#define START_CHAR(tty) ((tty)->termios.c_cc[VSTART])		/* VSTART:出力の再開(フロー制御)	*/
#define ERASE_CHAR(tty) ((tty)->termios.c_cc[VERASE])		/* VEARSE:一文字を削除		*/

struct tty_struct {
	struct termios termios;			/* ターミナルの入出力属性 */
	int pgrp;				/* 属したプロセスグループ	*/
	int stopped;				/* 一時停止する為のフラグ	*/
	void (*write)(struct tty_struct * tty);	/* 書き込みの関数ポインタ	*/
	struct tty_queue read_q;		/* 読み取りキュー	*/
	struct tty_queue write_q;		/* 書き込みキュー	*/
	struct tty_queue secondary;		/* 補助キュー(ｶﾉﾆｸﾙﾓｰﾄﾞ)	*/
	};

extern struct tty_struct tty_table[];

/*	intr=^C		quit=^|		erase=del	kill=^U
	eof=^D		vtime=\0	vmin=\1		sxtc=\0
	start=^Q	stop=^S		susp=^Y		eol=\0
	reprint=^R	discard=^U	werase=^W	lnext=^V
	eol2=\0
*/

/* 制御文字を8進数のｱｽｷｺｰﾄﾞで定義している */
#define INIT_C_CC "\003\034\177\025\004\0\1\0\021\023\031\0\022\017\027\026\0"

void rs_init(void);
void con_init(void);
void tty_init(void);

int tty_read(unsigned c, char * buf, int n);
int tty_write(unsigned c, char * buf, int n);

void rs_write(struct tty_struct * tty);
void con_write(struct tty_struct * tty);

void copy_to_cooked(struct tty_struct * tty);

#endif
