#ifndef _FCNTL_H
#define _FCNTL_H

#include <sys/types.h>

/* open/fcntl - NOCTTY, NDELAY isn't implemented yet */
/* open/fcntl - NOCTTY, NDELAY はまだ実装していないよ  */

/* 三個のアクセスモードフラグは古いプログラムと互換性の為
   各ビットは独立せずビットテストが出来ない。しかし各値は
   排他的に扱う為、まずQ_ACCMODEでマスクして順に比較する */
#define O_ACCMODE	00003	/* O_RDONLY.O_RDONLY.O_WRONLYのマスク用 */
#define O_RDONLY	   00	/* 読み込み専用	*/
#define O_WRONLY	   01	/* 書き込み専用	*/
#define O_RDWR		   02	/* 読み書き両用	*/

/* 四個のアプローチモードフラグは併用で省略可能 */
#define O_CREAT		00100	/* not fcntl */	/* ファイルが無ければ作る	*/
#define O_EXCL		00200	/* not fcntl */ /* ファイルが在るなら失敗	*/
#define O_NOCTTY	00400	/* not fcntl */	/* ファイルを端末制御に切り離す(分離) */
#define O_TRUNC		01000	/* not fcntl */	/* ファイルを長さ０に切り詰める(消去) */

/* 三個のファイルステータスフラグは併用で省略可能*/
#define O_APPEND	02000			/* ファイルを末尾へ書き込んでく(追記) */
#define O_NONBLOCK	04000	/* not fcntl */	/* ファイルを開く際はブロックさせない */
#define O_NDELAY	O_NONBLOCK		/* 即ち処理が遅ければ中断させ戻らせる */

/* Defines for fcntl-commands. Note that currently
 * locking isn't supported, and other things aren't really
 * tested.
 */
/* fcntl-commandsを定義する。ロックコマンドを現在サポート
 * しておらず、その他のコマンドも実際にはテストしていない
 * ことに注意してほしい。
 */

/* ファイル・ディスクリプタの複製 */
#define F_DUPFD		0	/* dup */

/* ファイル・ディスクリプタ・CLOEXECフラグの取得・設定 */
#define F_GETFD		1	/* get f_flags */
#define F_SETFD		2	/* set f_flags */

/* ファイル・ディスクリプタ・filp[f_flags]フラグの取得・設定 */
#define F_GETFL		3	/* more flags (cloexec) */
#define F_SETFL		4	

/* ファイル・ロックの設定・取得 (未実装なので全て-1が返される)*/
#define F_GETLK		5	/* not implemented */		/* 実装されていない	*/
#define F_SETLK		6	/* ロック要求を実行する - ロック出来ない場合は-1を返却	*/
#define F_SETLKW	7	/* ロック要求を実行する - ロック出来ない場合は待機する	*/
#define F_GETLK64	12	/*							*/
#define F_SETLK64	13	/* 機能はF_GETLK, F_SETLK, FSETLKWと同じだが、flock64は	*/
#define F_SETLKW64	14	/* 64 bit version でオリジナルには存在していない事に注意	*/

/* for F_[GET|SET]FL */
#define FD_CLOEXEC	1	/* actually anything with low bit set goes */

/* Ok, these are locking features, and aren't implemented at any
 * level. POSIX wants them.
 */
/* Ok, これらのロックする為の機能は、任意のレベルで実装されていない
 * POSIXは望んでいるんだけど。
 */
#define F_RDLCK		0	/* 共有ロック : ロックを解除するまで書き込み操作をブロックする。読み出しは可能 */
#define F_WRLCK		1	/* 排他ロック : ロックを解除するまで書き込み／読み出し操作の両方をブロックする */
#define F_UNLCK		2	/* アンロック */

/* Once again - not implemented, but ... */
/* まただけど - 実装していない, しかし ...	 */
struct flock {
	short l_type;	/* 種類: {F_RDLCK, F_WRLCK, F_UNLCK}	*/
	short l_whence;	/* 基底: {SEEK_SET, SEEK_CUR, SEEK_END}	*/
	off_t l_start;	/* ロック領域の開始オフセット		*/
	off_t l_len;	/* ロック領域の末尾オフセット 0=EOF 終端	*/
	pid_t l_pid;	/* ロック所有プロセスのPID		*/
};

extern int creat(const char * filename,mode_t mode);
extern int fcntl(int fildes,int cmd, ...);
extern int open(const char * filename, int flags, ...);

#endif
