#ifndef _CONST_H
#define _CONST_H

/* 主記憶[&end ~ BUFFER_END] = バッファキャッシュで利用	*/
/* end = fs:buffer.c:extern = defined in linker		*/
#define BUFFER_END 0x200000 /* physical address		*/

/* i-node[mode] = ファイル種別とアクセス権限を8進数ﾋﾞｯﾄﾌﾗｸﾞで識別	*/
/*		  [4:type - 12:permission] stat.h = permission	*/
#define I_TYPE          0170000	/* ビットマスク用: 1111	*/
#define I_REGULAR       0100000	/* 通常ファイル	: 1000	*/
#define I_DIRECTORY	0040000	/* ディレクトリ	:  100	*/
#define I_BLOCK_SPECIAL 0060000	/* ブロック型	:  110	*/
#define I_CHAR_SPECIAL  0020000	/* キャラクタ型	:   10	*/
#define I_NAMED_PIPE	0010000	/* 名前付きパイプ:    1	*/

/* permission  = 3bit4組[sticky-user:group:others]で区画分けする	*/
/*               sticky = ファイル形式により挙動が変わることに注意	*/
#define I_SET_UID_BIT   0004000	/*	:    0000[100000000000]	*/
#define I_SET_GID_BIT   0002000	/* 	:    0000[010000000000]	*/

#endif
