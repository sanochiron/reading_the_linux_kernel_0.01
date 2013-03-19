/* INIT_TASK[task:0]の特権レベルを0から3に移行 */
#define move_to_user_mode() \
__asm__ ("movl %%esp,%%eax\n\t" 	/* espをeaxに退避する */\
	"pushl $0x17\n\t" 		/* SS=LDT:0x17:10111を登録 */\
	"pushl %%eax\n\t" 		/* ESP=Stack Pointerを登録 */\
	"pushfl\n\t" 			/* EFLAGS=Flag Reg..を登録 */\
	"pushl $0x0f\n\t" 		/* CS=LDT:0x0F:1111 を登録 */\
	"pushl $1f\n\t" 		/* EIP=iret直下ｱﾄﾞﾚｽを設定 */\
	"iret\n" 			/* ユーザモードに切り替える */\
	"1:\tmovl $0x17,%%eax\n\t" 	/* INIT_TASKの実行再開位置 */\
	"movw %%ax,%%ds\n\t" 		/* ds = es = fs = gs=0x17 */\
	"movw %%ax,%%es\n\t" \
	"movw %%ax,%%fs\n\t" \
	"movw %%ax,%%gs" \
	:::"ax")

#define sti() __asm__ ("sti"::)		/* 割り込みの許可 */
#define cli() __asm__ ("cli"::)		/* 割り込みの無視 */
#define nop() __asm__ ("nop"::)		/* 命令中スペース */

/* 割り込みからの復帰命令 */
#define iret() __asm__ ("iret"::)

/* IDT:割り込み/トラップ/システム/ゲートの各ディスクリプタを共通設定 */
#define _set_gate(gate_addr,type,dpl,addr) \
__asm__ ("movw %%dx,%%ax\n\t" \
	"movw %0,%%dx\n\t" \
	"movl %%eax,%1\n\t" \
	"movl %%edx,%2" \
	: \
	: "i" ((short) (0x8000+(dpl<<13)+(type<<8))), \
	"o" (*((char *) (gate_addr))), \
	"o" (*(4+(char *) (gate_addr))), \
	"d" ((char *) (addr)),"a" (0x00080000))

/* 割り込みゲートの登録 */
#define set_intr_gate(n,addr) \
	_set_gate(&idt[n],14,0,addr)

/* トラップゲートの登録 */
#define set_trap_gate(n,addr) \
	_set_gate(&idt[n],15,0,addr)

/* システムゲートの登録 */
#define set_system_gate(n,addr) \
	_set_gate(&idt[n],15,3,addr)

/* セグメントディスクリプタを設定 */
#define _set_seg_desc(gate_addr,type,dpl,base,limit) {\
	*(gate_addr) = ((base) & 0xff000000) | \
		(((base) & 0x00ff0000)>>16) | \
		((limit) & 0xf0000) | \
		((dpl)<<13) | \
		(0x00408000) | \
		((type)<<8); \
	*((gate_addr)+1) = (((base) & 0x0000ffff)<<16) | \
		((limit) & 0x0ffff); }

/* GDT:TSS/LDTの各ディスクリプタを共通設定 */
#define _set_tssldt_desc(n,addr,type) \
__asm__ ("movw $104,%1\n\t" \
	"movw %%ax,%2\n\t" \
	"rorl $16,%%eax\n\t" \
	"movb %%al,%3\n\t" \
	"movb $" type ",%4\n\t" \
	"movb $0x00,%5\n\t" \
	"movb %%ah,%6\n\t" \
	"rorl $16,%%eax" \
	::"a" (addr), "m" (*(n)), "m" (*(n+2)), "m" (*(n+4)), \
	 "m" (*(n+5)), "m" (*(n+6)), "m" (*(n+7)) \
	)

/* ＴＳＳディスクリプタを登録 */
#define set_tss_desc(n,addr) _set_tssldt_desc(((char *) (n)),addr,"0x89")
/* ＬＤＴディスクリプタを登録 */
#define set_ldt_desc(n,addr) _set_tssldt_desc(((char *) (n)),addr,"0x82")
