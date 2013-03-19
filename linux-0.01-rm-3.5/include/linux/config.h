#ifndef _CONFIG_H
#define _CONFIG_H

/* #define LASU_HD */
#define LINUS_HD

/*
 * Amount of ram memory (in bytes, 640k-1M not discounted). Currently 8Mb.
 * Don't make this bigger without making sure that there are enough page
 * directory entries (boot/head.s)
 */
/* RAM Memoryの容量(in bytes, 640k-1Mは差し引かれない)。現在は8MBです。
 * 十分なページディレクトリエントリ (boot/head.s)を確認する事なく拡張は
 * しないでください。
 */

/* RAM Memory - 8MB (ﾊﾞｲﾄ単位) */
#define HIGH_MEMORY (0x800000)

/* End of buffer memory. Must be 0xA0000, or > 0x100000, 4096-byte aligned */
/* ﾊﾞｯﾌｧﾒﾓﾘの終端。0xA0000 or > 0x100000に4096ﾊﾞｲﾄ境界に揃える必要があります */
#if (HIGH_MEMORY>=0x600000)
#define BUFFER_END 0x200000	/* 6MB以上のメモリがあれば、&end~2MB境界まで */
#else
#define BUFFER_END 0xA0000	/* 6MB以上のメモリがなくば、&end~640K境界まで*/
#endif


/* ルートファイルシステムの存在するドライブ&パーティションを指定する
 * dev_no(major<<8)+minor = ﾃﾞﾊﾞｲｽ番号=ﾒｼﾞｬｰ番号*256+ﾏｲﾅｰ番号
 * major = 1:mem 2:fd 3:hd 4:ttyx 5:tty 6:lp 7:unnamed pipes
 * 0x300 = /dev/hd0:第一ハードドライブ (256*3+0 = 300)
 *  4 partitions
 * 0x305 = /dev/hd5:第二ハードドライブ (256*3+5 = 305)
 *  4 partitions
 */
/* Root device at bootup. */
#define ROOT_DEV 0x301		/* ルートファイルシステムは第一ドライブに存在 */

/*
 * HD type. If 2, put 2 structures with a comma. If just 1, put
 * only 1 struct. The structs are { HEAD, SECTOR, TRACKS, WPCOM, LZONE, CTL }
 *
 * NOTE. CTL is supposed to be 0 for drives with less than 8 heads, and
 * 8 if heads >= 8. Don't know why, and I haven't tested it on a drive with
 * more than 8 heads, but that is what the bios-listings seem to imply. I
 * just love not having a manual.
 */
/*
 * HD_type. もし二台あればコンマで二つ目の構造体を挿入してほしい。丁度一台なら
 * 一個の構造体を挿入するだけだ。構造体は{HEAD,SECTOR,TRACKS,WPCOM,LZONE,CTL}
 *
 * 注意。CTLは8個未満のヘッドのドライブの為に0であるべきだ。尚、heads >= 8なら
 * 知らない。というか、僕は8個以上のヘッドのドライブをテストしていない。けれど、
 * bios-listings には含まれているようだ。ただ、僕は丁度このマニュアルを持って
 * いないからわからない
 */

#define HD_TYPE { 4,20,1024,0,4,0 }

#endif
