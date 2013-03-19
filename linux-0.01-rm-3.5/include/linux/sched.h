#ifndef _SCHED_H
#define _SCHED_H

#define NR_TASKS 64	/* システムが同時に制御できるシステム数	*/
#define HZ 100		/* クロック刻みのタイマ割り込み (10ms)	*/

/* INIT_TASK:task[0]は実行されるプロセスが無かった場合のアイドル用	*/
#define FIRST_TASK task[0]	  /* アイドルタスク = INIT_TASK	*/
#define LAST_TASK task[NR_TASKS-1]/* 最後のタスク = task[63]	*/

#include <linux/head.h>
#include <linux/fs.h>
#include <linux/mm.h>

#if (NR_OPEN > 32)
/* 原罪のclose-on-exec-flagsは1ﾜｰﾄﾞで、最大32個のﾌｧｲﾙ/ﾌﾟﾛｾｽになる	*/
#error "Currently the close-on-exec-flags are in one word, max 32 files/proc"
#endif

#define TASK_RUNNING		0 /* 実行中あるいは割当待ち	*/
#define TASK_INTERRUPTIBLE	1 /* シグナル受信を待機している	*/
#define TASK_UNINTERRUPTIBLE	2 /* シグナル受信を待機せずI/O	*/
#define TASK_ZOMBIE		3 /* ゾンビ状態:親のwait()待ち	*/
#define TASK_STOPPED		4 /* 実行は停止している		*/

#ifndef NULL
#define NULL ((void *) 0)
#endif

/* mm/memory.c - プロセスのページディレクトリのページテーブルを複製する */
extern int copy_page_tables(unsigned long from, unsigned long to, long size);
/* mm/memory.c - ページテーブル自体に設定されたメモリブロックを解放する */
extern int free_page_tables(unsigned long from, long size);

/* スケジューラの初期化関数 - kernel/sched.c */
extern void sched_init(void);
/* スケジューリング処理関数 - kernel/sched.c */
extern void schedule(void);
/* 例外ハンドラの初期化関数 - kernel/traps.c */
extern void trap_init(void);
/* 回復不可能なシステムエラ - kernel/panic.c */
extern void panic(const char * str);
/* 指定長の文字列を端末出力 - kernel/tty_io.c*/
extern int tty_write(unsigned minor,char * buf,int count);

typedef int (*fn_ptr)();

/* 浮動小数点コプロセッサのステート情報 */
struct i387_struct {
	long	cwd;	/* control word	*/
	long	swd;	/* status word	*/
	long	twd;	/* tag word	*/
	long	fip;	/* floating code instruction pointer	*/
	long	fcs;	/* floating code segment selector	*/
	long	foo;	/* floating operand offset		*/
	long	fos;	/* floating operand segment selector	*/
	long	st_space[20];	/* 8*10 bytes for each FP-reg = 80 bytes */
};

/* タスク・ステート・セグメント */
struct tss_struct {
	long	back_link;	/* 16 high bits zero */

	/* 特権レベル{0.1.2}毎のスタックポインタ	*/
	long	esp0;
	long	ss0;		/* 16 high bits zero */
	long	esp1;
	long	ss1;		/* 16 high bits zero */
	long	esp2;
	long	ss2;		/* 16 high bits zero */
	/* ページディレクトリのベースアドレス	*/
	long	cr3;
	/* インストラクションポインタ	*/
	long	eip;
	/* EFLAGS	*/
	long	eflags;
	/* 汎用レジスタ	*/
	long	eax,ecx,edx,ebx;
	long	esp;
	long	ebp;
	long	esi;
	long	edi;
	/* セグメントレジスタ */
	long	es;		/* 16 high bits zero */
	long	cs;		/* 16 high bits zero */
	long	ss;		/* 16 high bits zero */
	long	ds;		/* 16 high bits zero */
	long	fs;		/* 16 high bits zero */
	long	gs;		/* 16 high bits zero */
	/* LDT:セレクタ		*/
	long	ldt;		/* 16 high bits zero */
	/* I/O Map ベースアドレス */
	long	trace_bitmap;	/* bits: trace 0, bitmap 16-31 */
	/* 80387 Stat Segment	*/
	struct i387_struct i387;
};

struct task_struct {
/* these are hardcoded - don't touch */
	long state;		/* 状態: -1 unrunnable, 0 runnable, >0 stopped */
	long counter;		/* ｸｫﾝﾀｲﾑ: timer interrupt 実行中 */
	long priority;		/* ﾌﾟﾗｲｵﾘﾃｨ: 静的な優先度 */
	long signal;		/* ｼｸﾞﾅﾙ: 32 bit bitmap	 */
	fn_ptr sig_restorer;	/* sa_restorer との予想	 */
	fn_ptr sig_fn[32];	/* signal handler 32個	 */
/* various fields */
	int exit_code;		/* 親プロセスへの終了コード*/

	/* end_code:ｺｰﾄﾞ長, end_data:end_code+ﾃﾞｰﾀ長 */
	/* brk:brk system callで割り当てられたﾌﾞﾛｯｸ   */
	unsigned long end_code,end_data,brk,start_stack;

	/* pid:ﾌﾟﾛｾｽ識別番号, father:親ﾌﾟﾛｾｽID	*/
	/* pgrp:ｸﾞﾙｰﾌﾟID, session:ｾｯｼｮﾝ番号	*/
	/* leader:ｾｯｼｮﾝﾘｰﾀﾞID			*/
	long pid,father,pgrp,session,leader;

	/* uid:ﾕｰｻﾞID, euid:実効ﾕｰｻﾞID, suid:保存ﾕｰｻﾞID		*/
	unsigned short uid,euid,suid;

	/* gid:ｸﾞﾙｰﾌﾟID, euid:実効ｸﾞﾙｰﾌﾟID, sgid:保存ｸﾞﾙｰﾌﾟID	*/
	unsigned short gid,egid,sgid;

	/* ｱﾗｰﾑｸﾛｯｸ(tick) */
	long alarm;

	/* utime:ﾕｰｻﾞ使用時間, stime:ｼｽﾃﾑ使用時間 */
	/* cutime:cstime=子ﾌﾟﾛｾｽ側から見たu,s:time*/
	/* start_time:ﾌﾟﾛｾｽの開始時刻		 */
	long utime,stime,cutime,cstime,start_time;

	/* コプロセッサを使用しているか		*/
	unsigned short used_math;
/* file system info */

	/* TTYﾏｲﾅｰﾃﾞﾊﾞｲｽ番号, 無い場合-1	*/
	int tty;		/* -1 if no tty, so it must be signed */

	/* ファイルパーミッション用マスク	*/
	unsigned short umask;

	/* カレントディレクトリ:i-node	*/
	struct m_inode * pwd;
	/* ルートディレクトリ:i-node	*/
	struct m_inode * root;
	/* ファイルハンドル:ビットマップ一覧		*/
	unsigned long close_on_exec;
	/* プロセスの使っているファイルディスクリプタ一覧	*/
	struct file * filp[NR_OPEN];
/* ldt for this task 0 - zero 1 - cs 2 - ds&ss */
	struct desc_struct ldt[3];
/* tss for this task */
	struct tss_struct tss;
};

/*
 *  INIT_TASK is used to set up the first task table, touch at
 * your own risk!. Base=0, limit=0x9ffff (=640kB)
 */
/*
 *  INIT_TASKはタスクテーブルの先頭に配置する為に使う。変更するの
 *  は自己責任だ。Base=0, limit=0x9ffff (=640KB)
 */
#define INIT_TASK \
/* state etc */	{ 0,15,15, \
/* signals */	0,NULL,{(fn_ptr) 0,}, \
/* ec,brk... */	0,0,0,0,0, \
/* pid etc.. */	0,-1,0,0,0, \
/* uid etc */	0,0,0,0,0,0, \
/* alarm */	0,0,0,0,0,0, \
/* math */	0, \
/* fs info */	-1,0133,NULL,NULL,0, \
/* filp */	{NULL,}, \
	{ \
		{0,0}, \
/* ldt */	{0x9f,0xc0fa00}, /* コード長:640Kb, Base:0x0, G=1, D=1, DPL=3, P=1, TYPE=0x0a */\
		{0x9f,0xc0f200}, /* コード長:640Kb, Base:0x0, G=1, D=1, DPL=3, P=1, TYPE=0x02 */\
	}, \
/*tss*/	{0,PAGE_SIZE+(long)&init_task,0x10,0,0,0,0,(long)&pg_dir,\
	 0,0,0,0,0,0,0,0, \
	 0,0,0x17,0x17,0x17,0x17,0x17,0x17, \
	 _LDT(0),0x80000000, \
		{} \
	}, \
}

/* システムが同時に処理する為のタスクテーブル */
extern struct task_struct *task[NR_TASKS];
/* コプロセッサが使われているかどうか比較する */
extern struct task_struct *last_task_used_math;
/* ＣＰＵ上で現在実行されているカレントタスク */
extern struct task_struct *current;
/* システムが起動してからタイマワリコミ(10ms) */
extern long volatile jiffies;
/* システムが起動したときの時刻  init/main.c */
extern long startup_time;

/* 現在時刻:秒単位	*/
#define CURRENT_TIME (startup_time+jiffies/HZ)

/* 実行中のプロセスを待機状態にする */
extern void sleep_on(struct task_struct ** p);
/* 実行中のプロセスをシグナル受信可能で待機状態にする */
extern void interruptible_sleep_on(struct task_struct ** p);
/* 待機状態のプロセスを呼び覚まさせる */
extern void wake_up(struct task_struct ** p);

/*
 * Entry into gdt where to find first TSS. 0-nul, 1-cs, 2-ds, 3-syscall
 * 4-TSS0, 5-LDT0, 6-TSS1 etc ...
 */
/*
 * GDT内の最初のTSSｴﾝﾄﾘを見つける為に。0-nul, 1-cs, 2-ds, 3-syscall
 * 4-TSS0, 5=LDT0, 6-TSS1 etc ...
 */
/* コメントから察することの出来るように、Linus TorvaldsはGDT[3]にシス
 * テムコール用に独立したセグメントを設定した。しかし、そうはなってない
 */
/* head.s - 初期化されたGDTｴﾝﾄﾘの数 */
#define FIRST_TSS_ENTRY 4
/* head.s - 一番先頭のLDTｴﾝﾄﾘの位置 */
#define FIRST_LDT_ENTRY (FIRST_TSS_ENTRY+1)
/* GDT[TSS]を参照するための添字計算  */
#define _TSS(n) ((((unsigned long) n)<<4)+(FIRST_TSS_ENTRY<<3))
/* GDT[LDT]を参照するための添字計算  */
#define _LDT(n) ((((unsigned long) n)<<4)+(FIRST_LDT_ENTRY<<3))
/* TRﾚｼﾞｽﾀにﾀｽｸを切り替える為の補助的関数	*/
#define ltr(n) __asm__("ltr %%ax"::"a" (_TSS(n)))
/* LDTﾚｼﾞｽﾀを切り替える為の補助的関数	*/
#define lldt(n) __asm__("lldt %%ax"::"a" (_LDT(n)))
/* 現在実行されているタスクの数を得る	*/
#define str(n) \
__asm__("str %%ax\n\t" \
	"subl %2,%%eax\n\t" \
	"shrl $4,%%eax" \
	:"=a" (n) \
	:"a" (0),"i" (FIRST_TSS_ENTRY<<3))
/*
 *	switch_to(n) should switch tasks to task nr n, first
 * checking that n isn't the current task, in which case it does nothing.
 * This also clears the TS-flag if the task we switched to has used
 * tha math co-processor latest.
 */
/*
 *	switch_to(n) タスクナンバーnにタスクを切り替える必要があります。
 * 最初にnが現在のタスクでない箏をチェックし、その場合には何もしません。
 * また、最新のコプロセッサを反映する為に切り替えたら、TS-flagをクリア
 * する。
 */
#define switch_to(n) {\
struct {long a,b;} __tmp; 			/* task[n] != current	*/\
__asm__ __volatile__("cmpl %%ecx,current\n\t"	/* yes - return 1	*/\
	"je 1f\n\t"				/* ecx = 切り替るタスク	*/\
	"xchgl %%ecx,current\n\t"		/* GDT[TSS]セレクタ値保存*/\
	"movw %%dx,%1\n\t"			/* タスクを切り替えて次に	*/\
	"ljmp *%0\n\t"				/* コプロセッサが使われた	*/\
	"cmpl %%ecx,%2\n\t"			/* i387_structは不使用	*/\
	"jne 1f\n\t"				/* i387_structの為切替	*/\
	"clts\n" \
	"1:\n\t" \
	::"m" (*&__tmp.a),"m" (*&__tmp.b), \
	"m" (last_task_used_math),"d" _TSS(n),"c" ((long) task[n])); \
}

/* ページ:アラインメントの為にマスクをかける */
#define PAGE_ALIGN(n) (((n)+0xfff)&0xfffff000)

#define _set_base(addr,base) do { unsigned long __pr; \
__asm__ __volatile__ ("movw %%dx,%1\n\t" \
	"rorl $16,%%edx\n\t" \
	"movb %%dl,%2\n\t" \
	"movb %%dh,%3" \
	:"=&d" (__pr) \
	:"m" (*((addr)+2)), \
	 "m" (*((addr)+4)), \
	 "m" (*((addr)+7)), \
         "0" (base) \
        ); } while(0)

#define _set_limit(addr,limit) do { unsigned long __lr; \
__asm__ __volatile__ ("movw %%dx,%1\n\t" \
	"rorl $16,%%edx\n\t" \
	"movb %2,%%dh\n\t" \
	"andb $0xf0,%%dh\n\t" \
	"orb %%dh,%%dl\n\t" \
	"movb %%dl,%2" \
	:"=&d" (__lr) \
	:"m" (*(addr)), \
	 "m" (*((addr)+6)), \
	 "0" (limit) \
        ); } while(0)

/* Set LDT BaseAddress	*/
#define set_base(ldt,base) _set_base( ((char *)&(ldt)) , base )
/* Set LDT LimitAddress	*/
#define set_limit(ldt,limit) _set_limit( ((char *)&(ldt)) , (limit-1)>>12 )

static inline unsigned long _get_base(char * addr)
{
	unsigned long __base;
	__asm__("movb %3,%%dh\n\t"
		"movb %2,%%dl\n\t"
		"shll $16,%%edx\n\t"
		"movw %1,%%dx"
		:"=&d" (__base)
		:"m" (*((addr)+2)),
		 "m" (*((addr)+4)),
		 "m" (*((addr)+7)));
	return __base;
}

/* Get LDT BaseAddress	*/
#define get_base(ldt) _get_base( ((char *)&(ldt)) )

/* Get LDT LimitNumber	*/
#define get_limit(segment) ({ \
unsigned long __limit; \
__asm__ __volatile__("lsll %1,%0\n\tincl %0":"=r" (__limit):"r" (segment)); \
__limit;})

#endif
