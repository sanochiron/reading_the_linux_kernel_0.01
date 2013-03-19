#ifndef _TERMIOS_H
#define _TERMIOS_H

#define TTY_BUF_SIZE 1024

/* 0x54 is just a magic number to make these relatively uniqe ('T') */
/* 0x54 は これらの値を比較的ユニークにする唯のマジックナンバーだ ('T') */

/* 端末属性の取得及び設定を行う	struct termios	*/
#define TCGETS		0x5401	/* tcgetattr(fd, argp)			: 現在のシリアルポートの設定を取得する	*/
#define TCSETS		0x5402	/* tcgetattr(fd, TCSANOW, argp)		: 現在のシリアルポートの設定を変更する	*/
#define TCSETSW		0x5403	/* tcsetattr(fd, TCSADRAIN, argp	: 全出力を転送した後で変更を有効とする 	*/
#define TCSETSF		0x5404	/* tcsetattr(fd, TCSAFLUSH, argp)	: TCSETSW + 有効時に全入力を破棄させる	*/
#define TCGETA		0x5405	/* 古いSystem V端末用に残されたﾄﾞﾗｲﾊﾞｲﾝﾀｰﾌｪｲｽの為に存在している。struct termio	*/
#define TCSETA		0x5406
#define TCSETAW		0x5407
#define TCSETAF		0x5408

/* 回線に対する制御を行う	*/
#define TCSBRK		0x5409	/* tcsendbreak(fd, arg)		: BREAK 文字を送出する			*/
#define TCXONC		0x540A	/* tcflow(fd, arg)		: データの送信か受信を一時停止させる	*/
#define TCFLSH		0x540B	/* tcflush(fd, arg)		: データの送信か受信の転送を破棄する	*/

/* 排他における設定を行う	*/
#define TIOCEXCL	0x540C	/* 端末を専有モードにする、これ以降のopen()操作は無視される事になる	*/
#define TIOCNXCL	0x540D	/* 端末を共有モードにする、これ以降のopen()操作が可能となる事になる	*/

/* 端末に対する制御を行う	*/
#define TIOCSCTTY	0x540E	/* 端末を呼び出し元のプロセスにおける制御端末へと関連付ける	*/

/* グループIDの制御を行う	*/
#define TIOCGPGRP	0x540F	/* tcgetpgrp(fd)	: 端末に関連のﾌﾟﾛｾｽｸﾞﾙｰﾌﾟIDを取得する	*/
#define TIOCSPGRP	0x5410	/* tcsetpgrp(fd)	: 端末に現在のﾌﾟﾛｾｽｸﾞﾙｰﾌﾟIDを設定する	*/

/* バッファへの制御を行う	*/
#define TIOCOUTQ	0x5411	/* 現在の出力キューにある文字数を取得する			*/
#define TIOCSTI		0x5412	/* 現在の入力キューに対し文字群を設定する			*/

/* ｳｨﾝﾄﾞｳの取得及び設定を行う	struct winsize	*/
#define TIOCGWINSZ	0x5413	/* 端末に関連付けられたウィンドウサイズを取得する		*/
#define TIOCSWINSZ	0x5414	/* 端末に関連付けられたウィンドウサイズを設定する		*/

/* モデムに対し制御を行う	*/
#define TIOCMGET	0x5415	/* モデムのｽﾃｰﾀｽﾋﾞｯﾄを取得する		*/
#define TIOCMBIS	0x5416	/* モデムのｽﾃｰﾀｽﾋﾞｯﾄを設定する	(OR)	*/
#define TIOCMBIC	0x5417	/* モデムのｽﾃｰﾀｽﾋﾞｯﾄを削除する		*/
#define TIOCMSET	0x5418	/* モデムのｽﾃｰﾀｽﾋﾞｯﾄを設定する		*/

/* 回線をローカルにマーク	*/
#define TIOCGSOFTCAR	0x5419	/* CLOCALﾌﾗｸﾞの状態を取得する		*/
#define TIOCSSOFTCAR	0x541A	/* CLOCALﾌﾗｸﾞの状態を設定する		*/


/* SVR4とﾊﾞｰｸﾚｰｼｽﾃﾑには、端末ｳｨﾝﾄﾞｳのｻｲｽﾞの現在のｻｲｽﾞを記録しておき
 * ｻｲｽﾞが変わるとｶｰﾈﾙがﾌｧｸﾞﾗｳﾝﾄﾞﾌﾟﾛｾｽに通知する手段がある。ｶｰﾈﾙでは
 * 端末と擬似端末のおのおのに対してwinsize構造体を管理している。   */
struct winsize {
	unsigned short ws_row;		/* 行数 */
	unsigned short ws_col;		/* 列数 */
	unsigned short ws_xpixel;	/* 水平方向のピクセス単位のサイズ(未使用) */
	unsigned short ws_ypixel;	/* 垂直方向のピクセル単位のサイズ(未使用) */
};


/* System Vと初期Verには<termio.h>という名称のヘッダが存在しており
 * 構造体の名称はtermioだったが、POSIX.1により名称にSが追加された */
#define NCC 8
struct termio {
	unsigned short c_iflag;		/* input mode flags */
	unsigned short c_oflag;		/* output mode flags */
	unsigned short c_cflag;		/* control mode flags */
	unsigned short c_lflag;		/* local mode flags */
	unsigned char c_line;		/* line discipline */
	unsigned char c_cc[NCC];	/* control characters */
};	/* 古いSystem V用のｲﾝﾀｰﾌｪｲｽ */


/* POSIX.1は端末装置に対して古典的なioctl()を使わない事に注意する
 * ioctl()では最終引数のデータ型は操作に依存して変化する為である */
#define NCCS 17
struct termios {
	unsigned long c_iflag;		/* 入力モードフラグ : ドライバの文字入力を制御	*/
	unsigned long c_oflag;		/* 出力モードフラグ : ドライバの文字出力を制御	*/
	unsigned long c_cflag;		/* 制御モードフラグ : RS-232Cｼﾘｱﾙﾗｲﾝを制御	*/
	unsigned long c_lflag;		/* ドライバとユーザのインターフェイス		*/
	unsigned char c_line;		/* 通信制御		*/
	unsigned char c_cc[NCCS];	/* 制御文字		*/
};

/* c_cc characters */
#define VINTR 0		/* SIGINTの送信		: ^C	*/
#define VQUIT 1		/* SIGQUITの送信		: ^\	*/
#define VERASE 2	/* 一文字削除	 	: ^H	*/
#define VKILL 3		/* 一行削除		: ^U	*/
#define VEOF 4		/* ﾌｧｲﾙ終端		: ^D	*/
#define VTIME 5		/* 読込時のﾀｲﾑｱｳﾄ時間 0.1秒単位	*/
#define VMIN 6		/* 読込時の最小ﾊﾞｲﾄ数	: \1	*/
#define VSWTC 7		/* ｽｲｯﾁ文字		: \0	*/
#define VSTART 8	/* 出力の再開(ﾌﾛｰ制御)	: ^Q	*/
#define VSTOP 9		/* 出力の停止(ﾌﾛｰ制御)	: ^S	*/
#define VSUSP 10	/* SIGSTPの送信		: ^Z	*/
#define VEOL 11		/* 行終端		: \0	*/
#define VREPRINT 12	/* 入力の再表示		: ^R	*/
#define VDISCARD 13	/* 出力の破棄		: ^O	*/
#define VWERASE 14	/* 一単語削除		: ^W	*/
#define VLNEXT 15	/* 次文字をそのまま入力	: ^V	*/
#define VEOL2 16	/* EOL2文字		: \0	*/

/* c_iflag bits */
#define IGNBRK	0000001		/* BREAKを無視する		*/
#define BRKINT	0000002		/* BREAKでSIGINTを生成する	*/
#define IGNPAR	0000004		/* ﾊﾟﾘﾃｨｴﾗ-の文字を無視する	*/
#define PARMRK	0000010		/* ﾊﾟﾘﾃｨｴﾗｰの文字を明示する	*/
#define INPCK	0000020		/* ﾊﾟﾘﾃｨﾁｪｯｸを行う		*/
#define ISTRIP	0000040		/* 8bit目をｸﾘｱする		*/
#define INLCR	0000100		/* NL(改行)をCR(復帰)に置換える	*/
#define IGNCR	0000200		/* CR(復帰)を無視する		*/
#define ICRNL	0000400		/* CR(復帰)をNL(改行)に置換える	*/
#define IUCLC	0001000		/* 大文字は小文字に変換する	*/
#define IXON	0002000		/* XON/XOFFのﾌﾛｰ制御を出力で行う	*/
#define IXANY	0004000		/* 任意文字で出力再開 - 通常:^S	*/
#define IXOFF	0010000		/* XON/XOFFのﾌﾛｰ制御を入力で行う	*/
#define IMAXBEL	0020000		/* 入力ｷｭｰ満杯の時ﾍﾞﾙを鳴らす	*/

/* c_oflag bits */
#define OPOST	0000001		/* 出力ﾓｰﾄﾞﾌﾗｸﾞを有効にする	*/
#define OLCUC	0000002		/* 大文字を小文字に変換する	*/
#define ONLCR	0000004		/* NL(改行)をCR-NLに置換える	*/
#define OCRNL	0000010		/* CR(復帰)をNLに置き換える	*/
#define ONOCR	0000020		/* 0桁目でCRを出力をしない	*/
#define ONLRET	0000040		/* NLがCRの機能を有する		*/
#define OFILL	0000100		/* 延滞動作に補墳文字を送る	*/
#define OFDEL	0000200		/* 補墳文字 : 1:DEL, 0:NUL	*/
#define NLDLY	0000400		/* NLの延滞を設定する		*/
#define   NL0	0000000		/*  0秒				*/
#define   NL1	0000400		/*  0.10秒			*/
#define CRDLY	0003000		/* CRの延滞を設定する		*/
#define   CR0	0000000		/*  0秒				*/
#define   CR1	0001000		/*  現在のｶﾗﾑ位置に依存		*/
#define   CR2	0002000		/*  0.10秒			*/
#define   CR3	0003000		/*  0.15秒			*/
#define TABDLY	0014000		/* 水平ﾀﾌﾞに延滞を設定する	*/
#define   TAB0	0000000		/*  0秒				*/
#define   TAB1	0004000		/*  現在のｶﾗﾑ位置に依存		*/
#define   TAB2	0010000		/*  0.10秒			*/
#define   TAB3	0014000		/*  ﾀﾌﾞ文字を空白文字に展開	*/
#define   XTABS	0014000		/*  ﾀﾌﾞﾓｼﾞを空白何個にするか	*/
#define BSDLY	0020000		/* 後退文字の延滞時間を取得する	*/
#define   BS0	0000000		/*  0秒				*/
#define   BS1	0020000		/*  0.05秒			*/
#define VTDLY	0040000		/* 垂直ﾀﾌﾞに延滞を設定する	*/
#define   VT0	0000000		/*  0秒				*/
#define   VT1	0040000		/*  2秒				*/
#define FFDLY	0040000		/* 頁送りに延滞を設定する		*/
#define   FF0	0000000		/*  0秒				*/
#define   FF1	0040000		/*  2秒				*/

/* c_cflag bit meaning */
/* ﾎﾟｰﾚｰﾄ(Baud rate) = [ﾋﾞｯﾄ/秒]と呼ぶべき歴史的用語である	*/
#define CBAUD	0000017		/* ﾎﾞｰﾚｰﾄﾏｽｸ	*/
#define  B0	0000000		/* 回線切断 */
#define  B50	0000001		/* 50ﾎﾟｰ    */
#define  B75	0000002		/* 75ﾎﾟｰ    */
#define  B110	0000003		/* 110ﾎﾟｰ   */
#define  B134	0000004		/* 134.5ﾎﾟｰ */
#define  B150	0000005		/* 150ﾎﾟｰ   */
#define  B200	0000006		/* 200ﾎﾟｰ   */
#define  B300	0000007		/* 300ﾎﾟｰ   */
#define  B600	0000010		/* 600ﾎﾟｰ   */
#define  B1200	0000011		/* 1200ﾎﾟｰ  */
#define  B1800	0000012		/* 1800ﾎﾟｰ  */
#define  B2400	0000013		/* 2400ﾎﾟｰ  */
#define  B4800	0000014		/* 4800ﾎﾟｰ  */
#define  B9600	0000015		/* 9600ﾎﾟｰ  */
#define  B19200	0000016		/* 19200ﾎﾟｰ */
#define  B38400	0000017		/* 38400ﾎﾟｰ */
#define CSIZE	0000060		/* 1ﾊﾞｲﾄあたりの転送ﾋﾞｯﾄ数の設定*/
#define   CS5	0000000		/*  5ﾋﾞｯﾄ			*/
#define   CS6	0000020		/*  6ﾋﾞｯﾄ			*/
#define   CS7	0000040		/*  7ﾋﾞｯﾄ			*/
#define   CS8	0000060		/*  8ﾋﾞｯﾄ			*/
#define CSTOPB	0000100		/* 一字あたりのｽﾄｯﾌﾟﾋﾞｯﾄ 1=2 0=1	*/
#define CREAD	0000200		/* 入力を受信可能にする		*/
#define CPARENB	0000400		/* 出力でﾊﾟﾘﾃｨ生成、入力で検査	*/
#define CPARODD	0001000		/* 1=奇数ﾊﾟﾘﾃｨ 0=偶数ﾊﾟﾘﾃｨ	*/
#define HUPCL	0002000		/* 最後のﾌﾟﾛｾｽがｸﾛｰｽﾞした後切断	*/
#define CLOCAL	0004000		/* ﾓﾃﾞﾑの制御信号を無視する	*/
#define CIBAUD	03600000	/* 入力速度のﾏｽｸ　		*/ /* input baud rate (not used) */
#define CRTSCTS	020000000000	/* ﾌﾛｰ制御を有効化		*/ /* flow control */

/* c_lflag bits */
#define ISIG	0000001		/* ｷｰﾎﾞｰﾄﾞからのｼｸﾞﾅﾙ送信を有効	*/
#define ICANON	0000002		/* ｶﾉﾆｶﾙﾓｰﾄﾞを有効		*/
#define XCASE	0000004		/* 大文字の直前に/を置き明示　大文字だけの端末用	*/
#define ECHO	0000010		/* 入力文字を出力する		*/
#define ECHOE	0000020		/* EARSE文字の入力で一文字削除	*/
#define ECHOK	0000040		/* KILL文字の後で改行文字出力	*/
#define ECHONL	0000100		/* ECHOでなくとも改行だけは出力	*/
#define NOFLSH	0000200		/* SIGINT,SIGQUIT,SIGSUSP時、入出力ﾌﾗｯｼｭを無効	*/
#define TOSTOP	0000400		/* SIGTO を送信する		*/
#define ECHOCTL	0001000		/* 制御文字を^Xとして可視化する	*/
#define ECHOPRT	0002000		/* 削除された文字も表示をする　ハードコピー端末用	*/
#define ECHOKE	0004000		/* KILL文字の入力で一行削除	*/
#define FLUSHO	0010000		/* 出力を全て破棄		*/
#define PENDIN	0040000		/* 次の文字の入力後、入力ｷｭｰの全文字を再表示をする　	*/
#define IEXTEN	0100000		/* 実装依存の入力処理を有効化 - EOL2,LNEXT,REPRINT..	*/ 

/* modem lines */
#define TIOCM_LE	0x001	/* 回線利用可能	- DSR (data set ready/line enable)	*/
#define TIOCM_DTR	0x002	/* ﾃﾞｰﾀ端末ﾚﾃﾞｨ	- DTR (data terminal ready)		*/
#define TIOCM_RTS	0x004	/* 送信要求	- RTS (request to send)			*/
#define TIOCM_ST	0x008	/* 従局送信	- Secondary TXD (transmit)		*/
#define TIOCM_SR	0x010	/* 従局受信	- Secondary RXD (receive)		*/
#define TIOCM_CTS	0x020	/* 送信可能	- CTS (clear to send)			*/
#define TIOCM_CAR	0x040	/* ｷｬﾘｱ検出	- DCD (data carrier detect)		*/
#define TIOCM_RNG	0x080	/* 被呼表示	- RNG (ring)				*/
#define TIOCM_DSR	0x100	/* ﾃﾞｰﾀﾚﾃﾞｨｾｯﾄ	- DSR (data set ready)			*/
#define TIOCM_CD	TIOCM_CAR	/* 別称	*/
#define TIOCM_RI	TIOCM_RNG	/* 別称	*/

/* tcflow() and TCXONC use these */
#define	TCOOFF		0	/* 出力を一時停止する		*/
#define	TCOON		1	/* 停止した出力を再開する　	*/
#define	TCIOFF		2	/* 回線上にSTOP文字を送信する	*/
#define	TCION		3	/* 回線上にSTART文字を送信する	*/

/* tcflush() and TCFLSH use these */
/* tcflush() は端末ドライバが受信後にプロセスがまだ読み取っていない
 * 入出力バッファをフラッシュ(破棄)する役割を担っている		*/
#define	TCIFLUSH	0	/* 未読の入力ﾃﾞｰﾀをﾌﾗｯｼｭする	*/
#define	TCOFLUSH	1	/* 未読の出力ﾃﾞｰﾀをﾌﾗｯｼｭする	*/
#define	TCIOFLUSH	2	/* 未読の入出力ﾃﾞｰﾀをﾌﾗｯｼｭする	*/

/* tcsetattr uses these */
#define	TCSANOW		0	/* 変更をただちに有効とする	*/
#define	TCSADRAIN	1	/* 全出力転送後、変更を有効化	*/
#define	TCSAFLUSH	2	/* 全出力転送後、全入力を破棄	*/

typedef int speed_t;

extern speed_t cfgetispeed(struct termios *termios_p);
extern speed_t cfgetospeed(struct termios *termios_p);
extern int cfsetispeed(struct termios *termios_p, speed_t speed);
extern int cfsetospeed(struct termios *termios_p, speed_t speed);
extern int tcdrain(int fildes);
extern int tcflow(int fildes, int action);
extern int tcflush(int fildes, int queue_selector);
extern int tcgetattr(int fildes, struct termios *termios_p);
extern int tcsendbreak(int fildes, int duration);
extern int tcsetattr(int fildes, int optional_actions,
	struct termios *termios_p);

#endif
