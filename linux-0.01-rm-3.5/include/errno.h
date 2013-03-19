#ifndef _ERRNO_H
#define _ERRNO_H

/*
 * ok, as I hadn't got any other source of information about
 * possible error numbers, I was forced to use the same numbers
 * as minix.
 * Hopefully these are posix or something. I wouldn't know (and posix
 * isn't telling me - they want $$$ for their f***ing standard).
 *
 * We don't use the _SIGN cludge of minix, so kernel returns must
 * see to the sign by themselves.
 *
 * NOTE! Remember to change strerror() if you change this file!
 */
/*
 * ok, 僕はエラー番号に関する他の情報を持っていなかったから、やむを得ず
 * minixと同じ番号を使用しているんだ。
 * ある程度POSIXに準拠しているだろうね。僕は知らない (だってPOSIXは僕に
 * 教えてさえくれなかった。できそこないの標準に支払いが必要だなんて)
 * 
 * カーネルの戻り値は正負を区別する必要があるから、minixの_SIGNマクロは
 * 使用していない。
 * 
 * 注意! このファイルを変更したなら忘れずにstrerror()を変更してほしい。
 */

extern int errno;		/* lib/error.c グローバル変数	*/

#define ERROR		99						/* エラーの総称	*/
#define EPERM		 1	/* Operation not permitted	*/	/* 許可出来ない操作をした	*/
#define ENOENT		 2	/* No such file or directory	*/	/* 存在していないファイル	*/
#define ESRCH		 3	/* No such process		*/	/* 存在していないプロセス	*/
#define EINTR		 4	/* Interrupted system call	*/	/* シグナルに割り込まれた	*/
#define EIO		 5	/* I/O Error			*/	/* 入出力に問題が発生した	*/
#define ENXIO		 6	/* No such device or address	*/	/* 存在していないデバイス	*/
#define E2BIG		 7	/* Arg list too long		*/	/* 長過ぎている引数リスト	*/
#define ENOEXEC		 8	/* Exec format error		*/	/* 異常な実行ファイル形式	*/
#define EBADF		 9	/* Bad file number		*/	/* 不正確なファイル記述子	*/
#define ECHILD		10	/* No child processes		*/	/* 存在してない子プロセス	*/
#define EAGAIN		11	/* Try again			*/	/* リソースを確保出来ない	*/
#define ENOMEM		12	/* Out of memory		*/	/* メモリーを確保出来ない	*/
#define EACCES		13	/* Permission denied		*/	/* パーミッションが異なる	*/
#define EFAULT		14	/* Bad address			*/	/* アドレスが不正なものだ	*/
#define ENOTBLK		15	/* Block device required	*/	/* ブロックデバイスでない	*/
#define EBUSY		16	/* Device or resource busy	*/	/* リソースがビジー状態だ	*/
#define EEXIST		17	/* File exists			*/	/* ファイルが既に存在した	*/
#define EXDEV		18	/* Cross-device link		*/	/* クロスデバイスリンクだ	*/
#define ENODEV		19	/* No such device		*/	/* デバイスが存在してない	*/
#define ENOTDIR		20	/* Not a directory		*/	/* ディレクトリでない	*/
#define EISDIR		21	/* Is a directory		*/	/* ディレクトリである	*/
#define EINVAL		22	/* Invalid argument		*/	/* 引数が不適切である	*/
#define ENFILE		23	/* File table overflow		*/	/* システムが開けるﾌｧｲﾙ数は限界	*/
#define EMFILE		24	/* Too many open files		*/	/* プロセスが開けるﾌｧｲﾙ数は限界	*/
#define ENOTTY		25	/* Not a typewriter		*/	/* 不正な入出力操作が行われてる	*/
#define ETXTBSY		26	/* Text file busy		*/	/* テキストファイルがビジー状態	*/
#define EFBIG		27	/* File too large		*/	/* ファイルサイズが巨大過ぎてる	*/
#define ENOSPC		28	/* No space left on device	*/	/* デバイスには空き領域がないぞ	*/
#define ESPIPE		29	/* Illegal seek			*/	/* シークアドレスが無効なものだ	*/
#define EROFS		30	/* Read-only file system	*/	/* 読込専用のファイルシステムだ	*/
#define EMLINK		31	/* Too many links		*/	/* リンクが多すぎている		*/
#define EPIPE		32	/* Broken pipe			*/	/* パイプが切断されてる		*/
#define EDOM		33	/* Math argument out of domain of func */ /* 数値の引数が領域外 (ANSI C)	*/
#define ERANGE		34	/* Math result not representable       */ /* 計算の結果が範囲外 (ANSI C)	*/
#define EDEADLK		35	/* Resource deadlock would occur       */ /* リソースのデッドロックを回避	*/
#define ENAMETOOLONG	36	/* File name too long		*/	  /* ファイル名が長過ぎている	*/
#define ENOLCK		37	/* No record locks available	*/	  /* ロックを取得出来ていない	*/
#define ENOSYS		38	/* Function not implemented	*/	  /* 関数が実装されていない	*/
#define ENOTEMPTY	39	/* Directory not empty		*/	  /* ディレクトリが空でない	*/

#endif

