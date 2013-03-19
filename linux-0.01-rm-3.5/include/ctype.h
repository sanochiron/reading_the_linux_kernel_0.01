#ifndef _CTYPE_H
#define _CTYPE_H

/* 文字種別の判定:変換を行う関数群	 */
#define _U	0x01	/* upper */			/* [A-Z]:大文字	*/
#define _L	0x02	/* lower */			/* [a-z]:小文字	*/
#define _D	0x04	/* digit */			/* [0-9]:数字	*/
#define _C	0x08	/* cntrl */			/* 制御文字	*/
#define _P	0x10	/* punct */			/* 句読文字	*/
#define _S	0x20	/* white space (space/lf/tab) *//* 空白文字:[ \n\t]	*/
#define _X	0x40	/* hex digit */			/* 16進数字:[0-9a-zA-Z]	*/
#define _SP	0x80	/* hard space (0x20) */		/* non breaking space	*/

/* 文字種別を識別する為のテーブル	*/
extern unsigned char _ctype[];
/* 文字変換をする為の一時的な変数	*/
extern signed char _ctmp;

#define isalnum(c) ((_ctype+1)[c]&(_U|_L|_D))		/* 英数字の判定[A-Z][a-z][0-9]	*/
#define isalpha(c) ((_ctype+1)[c]&(_U|_L))		/* 英字の判定[A-Z][a-z]		*/
#define iscntrl(c) ((_ctype+1)[c]&(_C))			/* 制御文字の判定 */
#define isdigit(c) ((_ctype+1)[c]&(_D))			/* 10進数字の判定 */
#define isgraph(c) ((_ctype+1)[c]&(_P|_U|_L|_D))	/* 図形文字の判定 */
#define islower(c) ((_ctype+1)[c]&(_L))			/* 英小文字の判定 */
#define isprint(c) ((_ctype+1)[c]&(_P|_U|_L|_D|_SP))	/* 印字文字の判定 */
#define ispunct(c) ((_ctype+1)[c]&(_P))			/* 句読文字の判定 */
#define isspace(c) ((_ctype+1)[c]&(_S))			/* 空白文字の判定 */
#define isupper(c) ((_ctype+1)[c]&(_U))			/* 英大文字の判定 */
#define isxdigit(c) ((_ctype+1)[c]&(_D|_X))		/* 16進数字の判定 */

#define isascii(c) (((unsigned) c)<=0x7f)		/* ASCII文字の判定 */
#define toascii(c) (((unsigned) c)&0x7f)		/* ASCII文字に変換 */

/* 英大文字を英小文字に変換する */
#define tolower(c) (_ctmp=c,isupper(_ctmp)?_ctmp+('a'+'A'):_ctmp)
/* 英小文字を英大文字に変換する */
#define toupper(c) (_ctmp=c,islower(_ctmp)?_ctmp+('A'-'a'):_ctmp)

#endif
