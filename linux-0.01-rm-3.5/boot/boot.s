;
;	boot.s
;
; boot.s is loaded at 0x7c00 by the bios-startup routines, and moves itself
; out of the way to address 0x90000, and jumps there.
; boot.s は bios-startup routinesを経て0x7c00へ読み込まれる。その次に自分自身を
; 邪魔にならない0x90000へ移動させて、そこへ制御を移行する。
;
; It then loads the system at 0x10000, using BIOS interrupts. Thereafter
; it disables all interrupts, moves the system down to 0x0000, changes
; to protected mode, and calls the start of system. System then must
; RE-initialize the protected mode in it's own tables, and enable
; interrupts as needed.
; その時bios-interruptsを用いてシステムを0x10000へ読み込んでいく。その後から
; 全ての割り込みを無効にして、システムを0x0000に移動させて、プロテクトモードに
; 切り替えてからシステムの開始を呼び出す。その時プロテクトモード用のテーブルは
; 再初期化しなければならない。更に必要に応じて割り込みを有効にする必要がある。
;
; NOTE! currently system is at most 8*65536 bytes long. This should be no
; problem, even in the future. I want to keep it simple. This 512 kB
; kernel size should be enough - in fact more would mean we'd have to move
; not just these start-up routines, but also do something about the cache-
; memory (block IO devices). The area left over in the lower 640 kB is meant
; for these. No other memory is assumed to be "physical", ie all memory
; over 1Mb is demand-paging. All addresses under 1Mb are guaranteed to match
; their physical addresses.
; 注意! 現在システムは最長(8*65536)バイトにしなければならない。この制限は将来にも
; 問題が生じる事はない。物事はなるべく単純にしておきたいものだ。この512KBのカーネ
; ルサイズは十分だけど、実際はスタートアップルーチンを移動する必要があるだけでなく
; キャッシュメモリ(block IO devices)にも何か施さなければならないだろう。640KBに
; 残されている領域はそれたの為のものだ。それ以外の1MB以上の全てのメモリはデマンド
; ページングであり物理的であるとは想定されていない。即ち1MB未満の全てのアドレスは
; 物理アドレスに一致する事が保証されている。
; NOTE1 abouve is no longer valid in it's entirety. cache-memory is allocated
; above the 1Mb mark as well as below. Otherwise it is mainly correct.
;
; NOTE 2! The boot disk type must be set at compile-time, by setting
; the following equ. Having the boot-up procedure hunt for the right
; disk type is severe brain-damage.
; The loader has been made as simple as possible (had to, to get it
; in 512 bytes with the code to move to protected mode), and continuos
; read errors will result in a unbreakable loop. Reboot by hand. It
; loads pretty fast by getting whole sectors at a time whenever possible.


! ﾌﾛｯﾋﾟｰﾃﾞｨｽｸ1ﾄﾗｯｸあたりのｾｸﾀ数
; 1.44Mb disks:
sectors = 18
; 1.2Mb disks:
; sectors = 15
; 720kB disks:
; sectors = 9

! Assembler Directive Instruction (疑似命令:pesudo-instruction)
! .globl.global により実行可能ファイルフォーマット[a.out]における
! .text .data .bss セグメントの全てを同一アドレス範囲に配置させる
.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:	! text segment (命令コード)
.data
begdata:	! data segment (初期化されたデータ)
.bss
begbss:		! bss segment  (初期値を持たないデータ)
.text

BOOTSEG = 0x07c0			! original address of boot-sector
					; ブートセクタのオリジナルアドレス
INITSEG = 0x9000			! we move boot here - out of way
					; ここへブートセクタを移動 - 邪魔にならない位置
SYSSEG  = 0x1000			! system loaded at 0x10000 (65536).
					; システムは0x10000(65536)へ読み込まれる
ENDSEG	= SYSSEG + SYSSIZE		! where to stop loading
					; システムの終端長を計算

! start:からgo:は自身を既存ｾｸﾞﾒﾝﾄ 0x07c0(31K)から0x9000(576KB)の所へ
! 256w(512b)移動した後、ｾｸﾞﾒﾝﾄ間ｼﾞｬﾝﾌﾟで移動先にCPUの制御を移行させる
entry start		! ld86にﾌﾟﾛｸﾞﾗﾑのｴﾝﾄﾘﾎﾟｲﾝﾄを指示 : (0x0000)
start:
	mov	ax,#BOOTSEG
	mov	ds,ax			! ds = 0x07c0
	mov	ax,#INITSEG
	mov	es,ax			! es = 0x9000
	mov	cx,#256			! cx = 256
	sub	si,si			; 転送元アドレス ds:si = 0x07c0:0x0000
	sub	di,di			; 転送先アドレス es:di = 0x9000:0x0000
	rep				! repeat (cx-- != 0) movw (move word)
	movw				
	jmpi	go,INITSEG		! Jump InterSegment (ｾｸﾞﾒﾝﾄ間ｼﾞｬﾝﾌﾟ)
					; cs:ip = 0x9000:0x0000 (INITSEG:go)

! 以降は、CPUが自身の移動先 0x900000[cs:ip]から命令の実行をしていく
! 此所で、移動前の領域を参照しない様に全セグメントを 0x90000に初期化
! ｽﾀｯｸはBIOSﾜｰｸｴﾘｱとｼｽﾃﾑ読込時に邪魔にならない 0x90200以降に配置する
go:	mov	ax,cs
	mov	ds,ax			! ds = es = ss = 0x9000
	mov	es,ax			! esはstart:からgo:までに0x9000に初期化済
					! ｵﾘｼﾞﾅﾙのｿｰｽｺｰﾄﾞでは省かれている事に注目
	mov	ss,ax			! (push, pop, call)の為にｽﾀｯｸを設定する
	mov	sp,#0x400		; arbitrary value >>512

! print some inane message
! 改行ｺｰﾄﾞを含めた24文字の Loading System ...を出力する
!
! BIOS割り込み 0x10 機能番号 ah = 0x03 [カーソル位置の読込]
; http://www.delorie.com/djgpp/doc/rbinter/id/93/0.html

	mov	ah,#0x03	; read cursor pos
	xor	bh,bh
	int	0x10		; dh=行(0-24) dl=列(0-79)

! BIOS割り込み 0x10 機能番号 ah = 0x13 [文字列を書き込む ]
; http://www.delorie.com/djgpp/doc/rbinter/id/15/2.html
	
	mov	cx,#24		; 合計24文字
	mov	bx,#0x0007	; page 0, attribute 7 (normal)
	mov	bp,#msg1	; es:bp = 文字列が格納されたアドレス(ポインタ)
	mov	ax,#0x1301	; write string, move cursor
				; 0x01 = 末尾にカーソルを移動
	int	0x10

; ok, we've written the message, now
; we want to load the system (at 0x10000)
! ok, メッセージは書き込めた。さぁ、システムを
! 0x100000(64KB)に読み込もう。

	! 0x0000のBIOS割り込みベクタテーブルが存在している
	! 0x1000はBIOS割り込みルーチンで読み込まれるためだ
	mov	ax,#SYSSEG
	mov	es,ax		; segment of 0x010000	es=システム読込先
	call	read_it		; reading floppy disk	es=入力パラメータ
	call	kill_motor	; Floopy Disk Controllerでモータを停止する

; if the read went well we get current cursor position ans save it for
; posterity.
! うまく読み込めたら後々使うためにカーソル位置を保存しておこう

	! ブートシグネチャ領域[列.行]を上書きする - use con_init()
	mov	ah,#0x03	; read cursor pos
	xor	bh,bh
	int	0x10		; save it in known place, con_init fetches
	mov	[510],dx	; it from 0x90510.
		
; now we want to move to protected mode ...
! いよいよプロテクトモードへ移行するときだ ...

	cli			; no interrupts allowed !

; first we move the system to it's rightful place
! さぁシステムを正規の場所に移動しよう

	mov	ax,#0x0000
	cld			; 'direction'=0, movs moves forward
do_move:
	mov	es,ax		; destination segment	(転送先アドレス es:di = 0x0000:0x0000)
	add	ax,#0x1000
	cmp	ax,#0x9000
	jz	end_move	; 最後の転送(0x8000-0x9000(64KB)を終えた? yes-end_move
	mov	ds,ax		; source segment	(転送元アドレス ds:si = 0x1000:0x0000)
	sub	di,di
	sub	si,si
	mov 	cx,#0x8000
	rep
	movsw
	j	do_move

; then we load the segment descriptors
! 次にセグメントディスクリプタを読み込む

! CPUをﾘｱﾙﾓｰﾄﾞからﾌﾟﾛﾃｸﾄﾓｰﾄﾞへ切り替える為、ﾌﾞｰﾄｽﾄﾗｯﾌﾟﾛｰﾀﾞの512byteに
! 収まる範囲で必要最低限のGDTとIDTを仮設定しておき、切替後に再初期化する
end_move:

	mov	ax,cs		; right, forgot this at first. didn't work :-)
	mov	ds,ax		; 移動前のデータ領域を参照しない様に更新させる
	lidt	idt_48		; load idt with 0,0
	lgdt	gdt_48		; load gdt with whatever appropriate

; that was painless, now we enable A20
! これはたいした事じゃない、さぁA20を有効にしよう

	! 8042:KBCの入力バッファが空になるまで待機
	call	empty_8042		; Check KBC status register

	! 制御コマンド - [入力レジスタを出力レジスタに出力]
	mov	al,#0xD1		; command write
	out	#0x64,al

	! 空になれば承認された
	call	empty_8042

	! パラメータ - [入力レジスタにデータを格納]
	mov	al,#0xDF		; A20 on
	out	#0x60,al

	! 空になれば承認された
	call	empty_8042

; well, that went ok, I hope. Now we have to reprogram the interrupts :-(
; we put them right after the intel-reserved hardware interrupts, at
; int 0x20-0x2F. There they won't mess up anything. Sadly IBM really
; messed this up with the original PC, and they haven't been able to
; rectify it afterwards. Thus the bios puts interrupts at 0x08-0x0f,
; which is used for the internal hardware interrupts as well. We just
; have to reprogram the 8259's, and it isn't fun.
! ああ、ここまでは多分うまくいった。そう思うよ。さてこれかわ割り込みを再設定
! (reprogram)しなきゃならない。:-( Linuxでは割り込みをIntelが予約したすぐ
! 後 int 0x20-0x2Fに押し込むんだ。ここなら何も邪魔されないからね。
! 悲しいことに、IBMは最初のPCを作る際割り込みの設定をメチャクチャにしてしま
! ったんだ。結局彼らはこれを直すことが出来なかったんだ。だから、 BIOSの割り
! 込みはハードウェアの内部割り込みが使っているのと同じ領域 0x08-0x0fにある
! んだよ。というわけで、僕らはこれから8259をプログラムし直さないといけない
! そしてこいつは全然楽しくないんだ。

	! CPUの例外ﾍﾞｸﾀ(0~31)の範囲と、8259の割り込みﾍﾞｸﾀ(0~15)の範囲の
	! 衝突を防ぐために移動する。ﾍﾞｸﾀは割り込み要因を知る為の識別番号

	! ICW1 - 8259に初期化開始コマンド(0x11)を発行する
	! word 0x00eb,0x00eb = 反応が遅い為に延滞して調整
	mov	al,#0x11		; initialization sequence
	out	#0x20,al		; send it to 8259A-1
	.word	0x00eb,0x00eb		; jmp $+2, jmp $+2
	out	#0xA0,al		; and to 8259A-2
	.word	0x00eb,0x00eb

	! ICW2 - CPUの例外ﾍﾞｸﾀと8259が通知する割り込みﾍﾞｸﾀの衝突を回避する
	! IRQ0-IRQ7 を割り込みﾍﾞｸﾀ0x20~0x2F(32-39)に
	! IRQ8-IRQ15の割り込みﾍﾞｸﾀ0x28~0x2f(40~47)にリマッピング
	mov	al,#0x20		; start of hardware int's (0x20)
	out	#0x21,al
	.word	0x00eb,0x00eb
	mov	al,#0x28		; start of hardware int's 2 (0x28)
	out	#0xA1,al
	.word	0x00eb,0x00eb

	! ICW3 - マスタとスレープはIRQ2に連結する
	mov	al,#0x04		; 8259-1 is master
	out	#0x21,al
	.word	0x00eb,0x00eb
	mov	al,#0x02		; 8259-2 is slave
	out	#0xA1,al
	.word	0x00eb,0x00eb

	! ICW4 - マイクロプロセッサモードに設定する
	mov	al,#0x01		; 8086 mode for both
	out	#0x21,al
	.word	0x00eb,0x00eb
	out	#0xA1,al
	.word	0x00eb,0x00eb

	! OCW1 - 8259の割り込み発行をマスクする
	! CPU[cli]は割り込みを受け付けずに保留する。1命令で禁止出来て手軽で早い
	! PIC割り込み発行をマスクするのは、init()でIDTにｷｰﾎﾞｰﾄﾞやﾊｰﾄﾞﾃﾞｨｽｸの
	! ﾊﾝﾄﾞﾗを設定して解除するためだ。
	mov	al,#0xFF		; mask off all interrupts for now
	out	#0x21,al
	.word	0x00eb,0x00eb
	out	#0xA1,al

; well, that certainly wasn't fun :-(. Hopefully it works, and we don't
; need no steenking BIOS anyway (except for the initial loading :-).
; The BIOS-routine wants lots of unnecessary data, and it's less
; "interesting" anyway. This is how REAL programmers do it.
! うーん、確かにこいつはちっとも面白くない(ああ疲れた)。とにかく、これでうまく
! 動いてくれると思う。それに、どっちにしてももうBIOSはいじくる必要が無いんだ
! (最初の読み込み以外は、わかるよね)、BIOSルーチンはやたらとたくさんの不要な
! データを欲しがるし、こいつは全然おもしろいことじゃない。「本物」のプログ
! ラマならこうするさ。
;
; Well, now's the time to actually move into protected mode. To make
; things as simple as possible, we do no register set-up or anything,
; we let the gnu-compiled 32-bit programs do that. We just jump to
; absolute address 0x00000, in 32-bit protected mode.
! さて、今度こそ本当にﾌﾟﾛﾃｸﾄﾓｰﾄﾞに移行する時だ。出来るだけ物事を単純に保つ
! 為に、ﾚｼﾞｽﾀ設定やらそういった事は何もしないでおく。ここではGNUのツールで
! ｺﾝﾊﾟｲﾙされる32ﾋﾞｯﾄのﾌﾟﾛｸﾞﾗﾑにそれをやらせるんだ。ただ単に32ﾋﾞｯﾄのﾌﾟﾛﾃｸ
! ﾄﾓｰﾄﾞで絶対ｱﾄﾞﾚｽ0x00000へ飛ぶだけだよ。

	! lmsw (Load Machine Status Word) 命令を用いて切替える
	! 286より上位のCPUなら、mov cr0, axと記述することが可能
	! 切替えた直後は、jmp or callでCPUの命令実行ﾊﾟｲﾌﾟﾗｲﾝを
	! 更新しなければならない。
	mov	ax,#0x0001	; protected mode (PE) bit
	lmsw	ax		; This is it!
	jmpi	0,8		; jmp offset 0 of segment 8 (cs)

; This routine checks that the keyboard command queue is empty
; No timeout is used - if this hangs there is something wrong with
; the machine, and we probably couldn't proceed anyway.
! この関数はｷｰﾎﾞｰﾄﾞｺﾝﾄﾛｰﾗ(KBC)のﾊﾞｯﾌｧｷｭｰが空になるまで待つんだけど
! ﾀｲﾑｱｳﾄは発生しない。ここﾊﾝｸﾞしてしまうようならﾏｼﾝに不具合があって
! 多分これ以上先には進めないだろう。
empty_8042:
	.word	0x00eb,0x00eb
	in	al,#0x64	; 8042 status port
	test	al,#2		; is input buffer full?
	jnz	empty_8042	; yes - loop
	ret

; This routine loads the system at address 0x10000, making sure
; no 64kB boundaries are crossed. We try to load it as fast as
; possible, loading whole tracks whenever we can.
;
; in:	es - starting address segment (normally 0x1000)
;
; This routine has to be recompiled to fit another drive type,
; just change the "sectors" variable at the start of the file
; (originally 18, for a 1.44Mb drive)
;
! このルーチンは、システムアドレス0x10000を跨いでいない事を確認して
! 読み込む。出来るだけ早く読み込める様トラック全体を読み込んでいこう
!
! in:	es - セグメント開始アドレス (通常は0x10000)
!
! このルーチンは、ドライブタイプが違っていたら再コンパイルしなくちゃ
! ならない。ちょうどファイル先頭にある変数"sectors"を変更させること
! (オリジナルは18の1.44MBドライブ用)
;
! 次のディレクティブで読み込む際の変数群(16bit)を定義している
! sreadの初期値:1は、BIOSから予め読み込まれた自身を指したもの
! なので、最初に読み込まれるセクタ数は処理上:2から始まっている

sread:	.word 1			; sectors read of current track
				! 現在のトラックから読み終えたセクタ数
head:	.word 0			; current head
				! 現在のヘッド
track:	.word 0			; current track
				! 現在のトラック

read_it:
! 読込先ｾｸﾞﾒﾝﾄのﾍﾞｰｽｱﾄﾞﾚｽが 64KB境界を跨いでないかをテストしている
! 言い換えると、ESのﾍﾞｰｽｱﾄﾞﾚｽは 64KB境界:0x?0000でなければならない
! 原因:ｼｽﾃﾑをFDからﾒﾓﾘへ転送する際BIOS内部で使用されるDMAに起因する
!  http://docs.freebsd.org/doc/2.2.8-RELEASE/usr/share/doc/ja/handbook/handbook321.html
;
! 制限は DMAが読み書きするﾒﾓﾘ領域が物理的な 64KB境界を跨いでいない事
! (例) 0x10001 から 0x20000といった物理的な 64KB境界を超えた読み書きは
!      0x1FFFF 以降がラップアラウンドされ 0x10000に読み書きをしてしまう

	; if (esが64KB境界を跨いでいる時) { 無限ループ }
	mov ax,es		; ｾｸﾞﾒﾝﾄｱﾄﾞﾚｽが4KBｱﾗｲﾝﾒﾝﾄであるか確認する
	test ax,#0x0fff		; AND演算の結果をｽﾃｰﾀｽﾌﾗｸﾞに反映させるだけ
die:	jne die			; es must be at 64kB boundary
				; es は 64KB境界でなければならない
	xor bx,bx		; bx is starting address within segment
				; bx は ｾｸﾞﾒﾝﾄ内の開始ｱﾄﾞﾚｽである
rp_read:
! ｼｽﾃﾑをFDからﾒﾓﾘに完全に読み終えることが出来ているかを判定している
! 読込先ｾｸﾞﾒﾝﾄが読込停ｾｸﾞﾒﾝﾄ(#ENDSEG)以上に重なった時は読み終えた
! 重なっていない場合には ok1_read()にジャンプして読込を再開させる

	; if (es >= ENDSEG) { 完全に読み終えた CPUの制御をCALL元に返す }
	mov ax,es
	cmp ax,#ENDSEG		; have we loaded all yet?
				; 全て読み終えた？
	jb ok1_read
	ret
ok1_read:
! 現在のﾄﾗｯｸからｾｸﾞﾒﾝﾄ境界(64KB)を超えずに読み込める最大ｾｸﾀ数を計算
! 未読込ｾｸﾀ数 = 1ﾄﾗｯｸあたりのｾｸﾀ数 - 現在のﾄﾗｯｸから読み終えたｾｸﾀ数
! 未読込ﾊﾞｲﾄ数に変換して全て読み込んだ時にｾｸﾞﾒﾝﾄ(64KB)から溢れた場合
! 溢れずに読み込める最大ｾｸﾀ数を算出する
;
! 大切な事は、ここでの64KB境界がread_it()における64KB境界と違うこと
! read_it  = 物理アドレス上の64KB境界 - [ES]
! ok1_read = 相対アドレス上の64KB境界 - [BX]

	mov ax,#sectors
	sub ax,sread		; 現在のﾄﾗｯｸ内の未読込ｾｸﾀ数を求める
	mov cx,ax		; cx = ax = 未読込ｾｸﾀ数
	shl cx,#9		; cx = cx * 512 + offset(bx)
	add cx,bx		;  = 全ての未読込ｾｸﾀを読み終えた時の合計ﾊﾞｲﾄ数
	jnc ok2_read
	je ok2_read		;  ｾｸﾞﾒﾝﾄ(64KB)から溢れない場合は、ok2_readへ

	! 全ての未読込ｾｸﾀを読み込むとｾｸﾞﾒﾝﾄ(64KB)から溢れる時
	! 溢れずに読み込める最大ｾｸﾀ数を計算して ok2_readに進む
	xor ax,ax
	sub ax,bx
	shr ax,#9
ok2_read:
! 現在のﾄﾗｯｸからｾｸﾞﾒﾝﾄ境界(64KB)を超えずに読み込める最大ｾｸﾀ数を計算
! ok1_read()で求めた1ﾄﾗｯｸの未読込ｾｸﾀ数よりｼｽﾃﾑをES:BXに読み込ませる
!  #head - 1ﾄﾗｯｸを全て読み終えてもう片面を読み込んでいない場合は更新
!  #track- 両面を全て読み終えた場合ﾄﾗｯｸ:ﾍｯﾄﾞの両方を次トラックに更新

	call read_track		! ok1_readで求めた未読込ｾｸﾀ数(al) - es:bx

	! if (現在のﾄﾗｯｸから全てのｾｸﾀを読み終えてない場合) { ok3_readへ }
	! if (現在のﾄﾗｯｸから全てのｾｸﾀを読み終えている場合) { 両面かどうか}
	mov cx,ax		; cx = ax = read_trackで読み取られたｾｸﾀ数
	add ax,sread		; ax = ax + 現在のﾄﾗｯｸから読み終えたｾｸﾀ数
	cmp ax,#sectors		; 現在のﾄﾗｯｸから全てのｾｸﾀを読み終えた?
	jne ok3_read		; No -> ok3_read
	
	; if (head == 1) { 片面読み終えてない ok4_readへ }
	; else if (head == 0) { 両面読み終えている 次ﾄﾗｯｸに更新 }
	mov ax,#1
	sub ax,head		; (head 0 or 1) ZF=1なら両面全て読み終えてる
	jne ok4_read		; 両面全て読み終えてない場合は ok4_readへ

	; ﾄﾗｯｸ番号を更新
	inc track
ok4_read:
	; ﾍｯﾄﾞ番号を更新
	mov head,ax
	xor ax,ax
ok3_read:
! 読込先ｾｸﾞﾒﾝﾄ及びｾｸﾞﾒﾝﾄ内ｵﾌｾｯﾄを更新する
! ES(64KB)にまだ読み込める領域がある場合はBXのみを更新してrp_readへ
! ES(65KB)にもう読み込める領域がない場合はESとBXを更新してrp_readへ

	; if (読込先ｾｸﾞﾒﾝﾄに読み込める領域が残っている場合) { rp_readへ }
	; else { es += 0x1000; bx = 0x0000; goto rp_read }
	! sreadを更新
	mov sread,ax		; 最終ｾｸﾀまで読み終えていない場合はそのまま
				; 読み終えた場合は次ﾄﾗｯｸの先頭ｾｸﾀ(0)に更新
	! bxを更新
	shl cx,#9		; cx = read_trackで読み取られたｾｸﾀ数 + 512 + offset(bx)
	add bx,cx

	! ｾｸﾞﾒﾝﾄ一杯(64KB)まで読み終えていない場合 ESを更新せずに rp_readへ
	jnc rp_read
	mov ax,es

	! esを更新
	add ax,#0x1000
	mov es,ax		; 読込先ｾｸﾞﾒﾝﾄのﾍﾞｰｽｱﾄﾞﾚｽを次の64KB境界に移す
	xor bx,bx		; ｾｸﾞﾒﾝﾄ内ｵﾌｾｯﾄを 0に初期化する

	jmp rp_read		; 完全に読み終えたかの判定処理に戻る

! 現在のﾄﾗｯｸから、読込開始ｾｸﾀ(cl)から指定された読込ｾｸﾀ数(al):es:bxに読み込む
; BIOS割り込み 0x13 機能番号 ah = 0x02 [ﾌﾛｯﾋﾟｰﾃﾞｨｽｸから読み込む]
; http://www.delorie.com/djgpp/doc/rbinter/id/13/6.html
read_track:
	push ax			; 全ての汎用レジスタをスタックへ退避する
	push bx			; 読込に失敗した時の為に読込前の情報を保存
	push cx
	push dx
	mov dx,track		; 現在のﾄﾗｯｸ番号
	mov cx,sread		; 現在のﾄﾗｯｸから読み終えたｾｸﾀ数
	inc cx			 ;  cl = 詠み込むｾｸﾀ数
	mov ch,dl		 ;  ch = 読込開始ｾｸﾀ
	mov dx,head		 ; 現在のﾍｯﾄﾞ
	mov dh,dl		 ;  dh = ﾍｯﾄﾞ番号
	mov dl,#0		 ;  dl =ﾄﾞﾗｲﾌﾞ番号 (0 = A Drive)

	; 読込毎にﾍｯﾄﾞ番号(0 or 1)に設定
	and dx,#0x0100
	mov ah,#2

	int 0x13		! read sector
	jc bad_rt		; エラーが発生した場合、bad_rtへジャンプする
	pop dx
	pop cx
	pop bx
	pop ax			; 全ての汎用ﾚｼﾞｽﾀをｽﾀｯｸから復帰する
	ret

! ﾃﾞｨｽｸ読込ｴﾗｰ : ﾄﾞﾗｲﾌﾞをﾘｾｯﾄしてﾄﾗｯｸの読込を再試行
; BIOS割り込み 0x13 機能番号 ah = 0x00 [FDCの初期化]
; 入力 dl = ﾄﾞﾗｲﾌﾞ番号
bad_rt:	mov ax,#0
	mov dx,#0
	int 0x13
	pop dx			; 読込前の情報を復帰
	pop cx
	pop bx
	pop ax
	jmp read_track	   ; 再読込


/*
 * This procedure turns off the floppy drive motor, so
 * that we enter the kernel in a known state, and
 * don't have to worry about it later.
 */
! FDCのDDR(0x03F2)を使用してドライブを停止する処理
! http://community.osdev.info/index.php?(FDC)765A
! http://stanislavs.org/helppc/765.html
kill_motor:			! ディスクを痛めない為に
	push dx
	mov dx,#0x3f2		; FDCのﾃﾞｼﾞﾀﾙ出力ﾚｼﾞｽﾀへ書き込む
	mov al,#0		; Aﾄﾞﾗｲﾌﾞ DMAと割り込みの停止
	outb			; 0x03f2ﾎﾟｰﾄへalを出力する
	pop dx
	ret

! インテルの技術資料では、プロテクトモードへ切り替えるにはGDTとIDTを
! 必ず設定しなければならない。つまり、無効には出来ないと書かれている
! という事で、GDTRとLDTRには移行に最低限必要な GDTとIDTの設定をする
gdt:
	; GDT + 0x00 = NULLﾃﾞｨｽｸﾘﾌﾟﾀ
	.word	0,0,0,0		; dummy

	; GDT + 0x08 = ｶｰﾈﾙｺｰﾄﾞﾃﾞｨｽｸﾘﾌﾟﾀ
	.word	0x07FF		; 8Mb - limit=2047 (2048*4096=8Mb)
	.word	0x0000		; base address=0
	.word	0x9A00		; code read/exec 読取/実行
	.word	0x00C0		; granularity=4096, 386	: 4KB単位

	; GDT + 0x10 = ｶｰﾈﾙﾃﾞｰﾀﾃﾞｨｽｸﾘﾌﾟﾀ
	.word	0x07FF		; 8Mb - limit=2047 (2048*4096=8Mb)
	.word	0x0000		; base address=0
	.word	0x9200		; data read/write 読取/書込
	.word	0x00C0		; granularity=4096, 386	: 4KB単位

idt_48:	; LDTRに必要なメモリ上の6ﾊﾞｲﾄのﾃﾞｰﾀ構造
	; 割り込みは現時点で全て無効にしてあるため、全て0で初期化
	.word	0			; idt limit=0
	.word	0,0			; idt base=0L

gdt_48:	; GDTRに必要なﾒﾓﾘ上の6ﾊﾞｲﾄのﾃﾞｰﾀ構造
	.word	0x800		; gdt limit=2048, 256 GDT entries
	.word	gdt,0x9		; gdt base = 0X9xxxx
				; 0x0009 &lt;&lt; 16 + gdt(offset)
msg1:
	.byte 13,10			! 改行コード
	.ascii "Loading system ..."
	.byte 13,10,13,10		! 合計24文字

.text
endtext:
.data
enddata:
.bss
endbss:
