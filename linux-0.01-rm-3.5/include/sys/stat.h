#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <sys/types.h>
#include <stdint.h>

struct stat {
	uint16_t	st_dev;		/* ファイルのあるデバイスID	*/
	uint16_t	__pad1;	
	unsigned long	st_ino;		/* ファイルのあるi-node		*/
	uint16_t	st_mode;	/* アクセス保護モード		*/
	uint16_t	st_nlink;	/* ハードリンクの数		*/
	uint16_t	st_uid;		/* 所有者のユーザID		*/
	uint16_t	st_gid;		/* 所有者のグループID		*/
	uint16_t	st_rdev;	/* デバイスID(特殊ファイル専用)	*/
	uint16_t	__pad2;
	unsigned long	st_size;	/* 全体のサイズ(バイト単位)	*/
	unsigned long	st_blksize;	/* I/Oのブロックサイズ		*/
	unsigned long	st_blocks;	/* 割り当てられたブロック数	*/
	time_t		st_atime;	/* 最終アクセス時間		*/
	unsigned long	__unused1;
	time_t		st_mtime;	/* 最終修正時刻			*/
	unsigned long	__unused2;
	time_t		st_ctime;	/* 最終状態変更時刻		*/
	unsigned long	__unused3;
	unsigned long	__unused4;
	unsigned long	__unused5;
};

struct stat64 {
	uint16_t	st_dev;
	unsigned char	__pad0[10];

#define STAT64_HAS_BROKEN_ST_INO	1
	unsigned long	__st_ino;

	uint32_t	st_mode;
	uint32_t	st_nlink;

	unsigned long	st_uid;
	unsigned long	st_gid;

	uint16_t	st_rdev;
	unsigned char	__pad3[10];

__extension__	long long	st_size __attribute__((__packed__));
	unsigned long	st_blksize;

	unsigned long	st_blocks;	/* Number 512-byte blocks allocated. */
	unsigned long	__pad4;		/* future possible st_blocks high bits */

	time_t		st_atime;
	unsigned long	__pad5;

	time_t		st_mtime;
	unsigned long	__pad6;

	time_t		st_ctime;
	unsigned long	__pad7;		/* will be high 32 bits of ctime someday */

__extension__	unsigned long long	st_ino __attribute__((__packed__));
};

#define S_IFMT  00170000	/* use S_IS*()	*/
#define S_IFREG  0100000	/* 通常ファイル	*/
#define S_IFBLK  0060000	/* ﾌﾞﾛｯｸﾃﾞﾊﾞｲｽ	*/
#define S_IFDIR  0040000	/* ディレクトリ	*/
#define S_IFCHR  0020000	/* ｷｬﾗｸﾀﾃﾞﾊﾞｲｽ	*/
#define S_IFIFO  0010000	/* FIFO		*/
#define S_ISUID  0004000	/* 所有ﾕｰｻﾞID	*/
#define S_ISGID  0002000	/* 所有ｸﾞﾙｰﾌﾟID	*/
#define S_ISVTX  0001000	/* ｽﾃｨｰｷｨｰﾋﾞｯﾄ	*/

/* 判別に使う為のマクロ	*/
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)

#define S_IRWXU 00700	/* ファイルオーナーに対するマスク	*/
#define S_IRUSR 00400	/* オーナーがリード権限を持ってる	*/
#define S_IWUSR 00200	/* オーナーがライト権限を持ってる	*/
#define S_IXUSR 00100	/* オーナーが実行権限を持っている	*/

#define S_IRWXG 00070	/* グループの権限に対するマスク	*/
#define S_IRGRP 00040	/* グループにリード権限がある	*/
#define S_IWGRP 00020	/* グループにライト権限がある	*/
#define S_IXGRP 00010	/* グループに実行権限があるよ	*/

#define S_IRWXO 00007	/* グループ外の権限に対するマスク	*/
#define S_IROTH 00004	/* その他はリード権限を持っている	*/
#define S_IWOTH 00002	/* その他はライト権限を持っている	*/
#define S_IXOTH 00001	/* その他は実行権限を持っているよ	*/

extern int chmod(const char *_path, mode_t mode);
extern int fstat64(int fildes, struct stat64 *stat_buf);
extern int mkdir(const char *_path, mode_t mode);
extern int mkfifo(const char *_path, mode_t mode);
extern int stat64(const char *filename, struct stat64 *stat_buf);
extern mode_t umask(mode_t mask);

#endif
