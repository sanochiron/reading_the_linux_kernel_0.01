/*
 *  head.s contains the 32-bit startup code.
 *
 * NOTE!!! Startup happens at absolute address 0x00000000, which is also where
 * the page directory will exist. The startup code will be overwritten by
 * the page directory.
 */
/*
 *  head.s には32-bit startup codeが含まれる.
 *
 *  注意!! 始動はﾍﾟｰｼﾞﾃﾞｨﾚｸﾄﾘの存在する領域の絶対ｱﾄﾞﾚｽ 0x00000000で始まる.
 *         このｽﾀｰﾄｱｯﾌﾟｺｰﾄﾞはﾍﾟｰｼﾞﾃﾞｨﾚｸﾄﾘによって上書きされるでしょう.
 */

# 現時点でCPUはﾌﾟﾛﾃｸﾄﾓｰﾄﾞで動作しており、GDT[0x08]のCODE SEGMENTを参照している
# boot.s {null=0x00, code=0x08, data=0x10 }の各ディスクリプタが設定されている
.code32				/* GASに疑似命令 : 32ﾋﾞｯﾄの機械語を生成する */
.text
.globl idt,gdt,pg_dir,startup_32
pg_dir:				/* 0x0000 - 0x10000 = ﾍﾟｰｼﾞﾃﾞｨﾚｸﾄﾘで上書き */
startup_32:

	# IA32_Arh_Dev_Man Vol3:P380によれば、保護モードに移行した直後は
	# CPUがﾘｱﾙﾓｰﾄﾞ時のｾｸﾞﾒﾝﾄｷｬｯｼｭを利用している為更新する必要がある
	# ds = es = fs = gs = gdt[0x10](data segment)	
	movl $0x10,%eax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs

	# 直下のCALL命令の為にﾌﾟﾛﾃｸﾄﾓｰﾄﾞ用のｽﾀｯｸに切替える
	lss stack_start,%esp	# stack_start = ss:esp
				# kernel/sched.cに定義

	# 実際にｶｰﾈﾙが使うIDTとGDTに切替える(ﾌﾟﾛﾃｸﾄﾓｰﾄﾞ用)
	call setup_idt		# push(cs:eip)
	call setup_gdt		# push(cs:eip)


	# GDTを修正直後にCPUのﾃﾞｨｽｸﾘﾌﾟﾀｷｬｯｼｭを明示的に更新してあげよう
	# でなければ古いﾃﾞｨｽｸﾘﾌﾟﾀｷｬｯｼｭを参照してしまう危険があるからだ
	movl $0x10,%eax		# reload all the segment registers
	mov %ax,%ds		# after changing gdt. CS was already
	mov %ax,%es		# reloaded in 'setup_gdt'
	mov %ax,%fs
	mov %ax,%gs

	# IDTの割り込みｹﾞｰﾄ:ignore_int()の為に初期化する
	lss stack_start,%esp

	# 1MBの境界に当たるｱﾄﾞﾚｽﾊﾞｽ:A20ﾗｲﾝのﾃﾞｺｰﾄﾞを確認する
	# 0x000000-0x100000を順番に参照してﾗｯﾌﾟﾗｳﾝﾄﾞしないか
	xorl %eax,%eax		# xor %eax %eax = 0 最適化(ﾃｸﾆｯｸ)
1:	incl %eax		# check that A20 really IS enabled
	movl %eax,0x000000
	cmpl %eax,0x100000
	je 1b			# (1b - backword) : 後方参照
				# (1f - forword)  : 前方参照

	# 80386:80387の浮動少数点演算ｺﾌﾟﾛｾｯｻの有無を確認する
	# 0 - CR0:EM(Emulate Math Coprocessor)の設定でｴﾐｭﾚｰﾄ
	movl %cr0,%eax		# check math chip
	andl $0x80000011,%eax	# Save PG,ET,PE
	testl $0x10,%eax
	jne 1f			# ET is set - 387 is present
	orl $4,%eax		# else set emulate bit
1:	movl %eax,%cr0
	jmp after_page_tables

/*
 *  setup_idt
 *
 *  sets up a idt with 256 entries pointing to
 *  ignore_int, interrupt gates. It then loads
 *  idt. Everything that wants to install itself
 *  in the idt-table may do so themselves. Interrupts
 *  are enabled elsewhere, when we can be relatively
 *  sure everything is ok. This routine will be over-
 *  written by the page tables.
 */
setup_idt:
	/* interrupt gate descriptor - low  */
	lea ignore_int,%edx	# edx = low word ignore_int ↓
	movl $0x00080000,%eax	# eax = high word 0x0008:offset
	movw %dx,%ax		/* selector = 0x0008 = cs */
	/* interrupt gate descriptor - high */
	movw $0x8E00,%dx	/* interrupt gate - dpl=0, present */

	/* initialize */
	lea idt,%edi		# edi = interrupt descriptor table
	mov $256,%ecx		# ecx = 256 entries
rp_sidt:
	movl %eax,(%edi)	# edi=low,edi+4=high
	movl %edx,4(%edi)	# 関節メモリ参照edi+4
	addl $8,%edi		# edi + 8
	dec %ecx	# ecx--
	jne rp_sidt	# jump not equal

	/* Interrupt Descriptor Table 登録 */
	lidt idt_descr
	ret

/*
 *  setup_gdt
 *
 *  This routines sets up a new gdt and loads it.
 *  Only two entries are currently built, the same
 *  ones that were built in init.s. The routine
 *  is VERY complicated at two whole lines, so this
 *  rather long comment is certainly needed :-).
 *  This routine will beoverwritten by the page tables.
 */
setup_gdt:
	/* Global Descriptor Table 登録 */
	lgdt gdt_descr
	ret

/*
 * I put the kernel paget tables right after the page directory,
 * using 2 of then to span 8Mb of physical memory. People with
 * more than 8MB will have to expand this.
 */
/* 僕はページディレクトリ直後にカーネルページテーブルを挿入して
 * 二個は8MBの物理メモリに使われる。皆さんが8MB以上持って場合は
 * ここを拡張する必要があるでしょう。(上位Versionのコメント)
 */

# Page Table = 4KB領域[4byteのｴﾝﾄﾘが1024個]
.org 0x1000	# PTE(Page Table Entry:4b-4kと対応)
pg0:		# 1024 * 4K = 4MB Mapping

.org 0x2000	# 8MB
pg1:

.org 0x3000
pg2:		# This is not used yet, but if you
		# want to expand past 8 Mb, you'll have
		# to use it.
		# ここはまだ使わないけど、8MBより拡張するなら
		# 使用しなければならないでしょう。

.org 0x4000

# init/main()に制御を移行する直前に、明示的にパラメータをスタックへ格納する
# 8MBのﾍﾟｰｼﾞﾝｸﾞを有効にさせた直後に、RET命令でinit/main()へ復帰して移行する
after_page_tables:		# These are the parameters to main :-)
	pushl $0		/* *envp[] */
	pushl $0		/* *argv[] */
	pushl $0		/*  argc   */
	pushl $L6		# return address for main, if it decides to.
	pushl $main
	jmp setup_paging
L6:
	jmp L6			# main should never return here, but
				# just in case, we know what happens.
				# mainは決してここに戻らない。しかし
				# 念の為、僕等は何が問題なのか知る。

/* This is the default interrupt "handler" :-) */
.align 2
# 640KB~1MBのIOｱﾄﾞﾚｽ空間で、 0xb8000~0xbFFFFはVGA Color Text Modeに使われる
# 列:80 * 行:25 = 2000字扱え、偶数ﾊﾞｲﾄはASCII文字で奇数ﾊﾞｲﾄは属性を指定する
ignore_int:
	# 2行:1列目 = 文字自体をインクリメント
	incb 0xb8000+160		# put something on the screen
	# 文字色 = 0x02(緑)
	movb $2,0xb8000+161		# so that we know something
	iret				# happened


/*
 * Setup_paging
 *
 * This routine sets up paging by setting the page bit
 * in cr0. The page tables are set up, identity-mapping
 * the first 8MB. The pager assumes that no illegal
 * addresses are produced (ie >4Mb on a 4Mb machine).
 *
 * NOTE! Although all physical memory should be identity
 * mapped by this routine, only the kernel page functions
 * use the >1Mb addresses directly. All "normal" functions
 * use just the lower 1Mb, or the local data space, which
 * will be mapped to some other place - mm keeps track of
 * that.
 *
 * For those with more memory than 8 Mb - tough luck. I've
 * not got it, why should you :-) The source is here. Change
 * it. (Seriously - it shouldn't be too difficult. Mostly
 * change some constants etc. I left it at 8Mb, as my machine
 * even cannot be extended past that (ok, but it was cheap :-)
 * I've tried to show which constants to change by having
 * some kind of marker at them (search for "8Mb"), but I
 * won't guarantee that's all :-( )
 */
.align 2
setup_paging:
	# ﾍﾟｰｼﾞﾃﾞｨﾚｸﾄﾘ[pg0.pg1]設定 : pg0:4MB pg1:4MB=8MB
	movl $1024*3,%ecx		# 3 pages : pg_dir+2 page tables
	xorl %eax,%eax
	xorl %edi,%edi			/* pg_dir is at 0x000 */
	cld;rep;stosl			/* eax -> es:edi++    */

	# pg?-0x00001007 & 0xfffff000 = 0x1000 -> base address
	#     0x00001007 & 0x00000fff = 0x07   -> flag bits
	movl $pg0+7,pg_dir		/* set present bit/user r/w */
	movl $pg1+7,pg_dir+4		/*  --------- " " --------- */

	# pg1[1024] = pg1+4092 : pg1[1024] ~ pg[0] = 8MB
	movl $pg1+4092,%edi
	movl $0x7ff007,%eax		/*  8Mb - 4096 + 7 (r/w user,p) */

	# SI:DI 方向:ﾃﾞｸﾘﾒﾝﾄ:設定
	std

	# 8MBの物理ｱﾄﾞﾚｽ & ｵﾌﾟｼｮﾝをﾏｯﾋﾟﾝｸﾞ
1:	stosl			/* fill pages backwards - more efficient :-) */
	subl $0x1000,%eax
	jge 1b

	# CR3(Control Register 3)にﾍﾟｰｼﾞﾃﾞｨﾚｸﾄﾘのﾍﾞｰｽｱﾄﾞﾚｽを設定、CR0で有効にする
	xorl %eax,%eax		/* pg_dir is at 0x0000 */
	movl %eax,%cr3		/* cr3 - page directory start */
	movl %cr0,%eax
	orl $0x80000000,%eax
	movl %eax,%cr0		/* set paging (PG) bit */
	ret			/* this also flushes prefetch-queue */

.align 2
.word 0
idt_descr:
	.word 256*8-1		# idt contains 256 entries
	.long idt
.align 2
.word 0
gdt_descr:
	.word 256*8-1		# so does gdt (not that that's any
	.long gdt		# magic number, but it works for me :^)

	.align 8
idt:	.fill 256,8,0		# idt is uninitialized

gdt:	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00c09a00000007ff	/* 8Mb */
	.quad 0x00c09200000007ff	/* 8Mb */
	.quad 0x0000000000000000	/* TEMPORARY - don't use */
	.fill 252,8,0			/* space for LDT's and TSS's etc */
