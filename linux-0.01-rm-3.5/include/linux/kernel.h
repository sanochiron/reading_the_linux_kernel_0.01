/*
 * 'kernel.h' contains some often-used function prototypes etc
 */
/*
 * 'kernel.h' 幾つか頻繁に使われる関数のプロトタイプ等が含まれる
 */

/* kernel/fork.c	- ユーザ空間のアドレス範囲チェックする	*/
void verify_area(void * addr,int count);
/* kernel/panic.c	- エラーメッセージを表示して無限ループ	*/
void panic(const char * str);
/* init/main.c		- 標準出力に対して書式文字列を出力する	*/
int printf(const char * fmt, ...);
/* kernel/printk.c	- 直接端末に対して書式文字列を出力する	*/
int printk(const char * fmt, ...);
/* kernel/tty_io.c	- 指定した長さの文字列を端末に出力する	*/
int tty_write(unsigned ch,char * buf,int count);
