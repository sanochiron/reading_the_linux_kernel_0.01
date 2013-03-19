#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef long ptrdiff_t;		/* difference between two pointers */ 
				/* ポインタ間の減算結果を示す為の型  */
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;	/* return sizeof()  返却値の戻り型  */
#endif

#undef NULL
#define NULL ((void *)0)	/* null pointer */

/* 構造体の基底からメンバのオフセット値を計算(バイト単位)	*/
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif