#ifndef _STDARG_H
#define _STDARG_H

/* 可変長引数を持つ関数をポータブルに定義する為のマクロ一覧	*/

typedef char *va_list;	/* 可変長引数アクセス用の特別な型	*/

/* Amount of space required in an argument list for an arg of type TYPE.
   TYPE may alternatively be an expression whose type is used.  */
/* argパラメータリストに必要な領域の種類(TYPE)は以下の通りです。
   TYPEは代わりに型に使用される表現を指定することもできます。 */

/*
 * __va_rounded_size(TYPE) : TYPEはバイト長を丸めた型(int 4 倍数)にする
 *   va_start(AP, LASTARG) : LASTARGは最後の固定長引数(...直線の仮引数)で、APを可変長引数の先頭に初期する
 *      va_arg (AP, TYPE)　: APが現在指している可変長引数から引数の型(TYPE)と取り出し、次の引数に移動する
 *        va_end (va_list) : APを正常に終了する。
 */
#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

/* __builtin_saveregs() は GCCﾗｲﾌﾞﾗﾘﾌﾟﾛｸﾞﾗﾑのlibgcc2.cに定義されています	*/
/* GCC Manual:Target Description Macros->Implementing the varargs Macro	*/
#ifndef __sparc__
#define va_start(AP, LASTARG) 						\
 (AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
#else
#define va_start(AP, LASTARG) 						\
 (__builtin_saveregs (),						\
  AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
#endif

void va_end (va_list);		/* Defined in gnulib */
#define va_end(AP)

#define va_arg(AP, TYPE)						\
 (AP += __va_rounded_size (TYPE),					\
  *((TYPE *) (AP - __va_rounded_size (TYPE))))

#endif /* _STDARG_H */
