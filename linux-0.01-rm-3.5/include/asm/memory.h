/*
 *  NOTE!!! memcpy(dest,src,n) assumes ds=es=normal data segment. This
 *  goes for all kernel functions (ds=es=kernel space, fs=local data,
 *  gs=null), as well as for all well-behaving user programs (ds=es=
 *  user data space). This is NOT a bug, as any user program that changes
 *  es deserves to die if it isn't careful.
 */
/*
 *  注意!!! memcpy(dest,src,n) はds=es=normal data segmentを前提としている
 *  これはカーネルで使われる全関数(ds=es=kernsl space, fs=local data, 
 *  gs=null)だけでなくユーザプログラムでも同様(ds=es=user data space)です。
 *  これはバクでなく、esを変更するどのユーザプログラムも慎重でないなら死ぬに
 *  値します。
 */
#define memcpy(dest,src,n) ({ \
void * _res = dest; \
__asm__ ("cld;rep;movsb" \
	::"D" ((long)(_res)),"S" ((long)(src)),"c" ((long) (n)) \
	:"di","si","cx"); \
_res; \
})
