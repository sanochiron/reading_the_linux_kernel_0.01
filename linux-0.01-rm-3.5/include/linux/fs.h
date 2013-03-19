/*
 * This file has definitions for some important file table
 * structures etc.
 */

#ifndef _FS_H
#define _FS_H

#include <sys/types.h>

/* devices are as follows: (same as minix, so we can use the minix
 * file system. These are major numbers.)
 * デバイスは次の通りでせう。(Minixと同様に、我々はMinixのファイルシス
 * テムを扱えます。以下はメジャー番号です。)
 *
 * 0 - unused (nodev)
 * 1 - /dev/mem		Memory device	   (character device)
 * 2 - /dev/fd		Floppy disk drive  (block device)
 * 3 - /dev/hd		Hard disk drive	   (block device)
 * 4 - /dev/ttyx	TTY serial device  (character device)
 * 5 - /dev/tty		TTY current device (character device)
 * 6 - /dev/lp		Printer device	   (character device)
 * 7 - unnamed pipes
 */

/* ブロックデバイスの判定 */
#define IS_BLOCKDEV(x) ((x)==2 || (x)==3)

#define READ 0
#define WRITE 1

void buffer_init(void);

#define MAJOR(a) (((unsigned)(a))>>8)	/* メジャー番号の算出 */
#define MINOR(a) ((a)&0xff)		/* マイナー番号の算出 */

#define NAME_LEN 14			/* ファイル名の最大長 */

#define I_MAP_SLOTS 8		/* index-node bitmap 8個 */
#define Z_MAP_SLOTS 8		/* data-block bitmap 8個 */
#define SUPER_MAGIC 0x137F	/* Super Block Magic Num */

#define NR_OPEN 20		/* プロセスの開けるファイル・スロット数 */
#define NR_INODE 32		/* システムの平稀有ファイル・スロット数 */
#define NR_FILE 64		/* システムの開けるファイルシステム総数 */
#define NR_SUPER 8		/* システムの保てるi-nodeキャッシュ総数 */

/* 素数:307が何を基準に定義されたのか解らない。dev:0x307を元にしたのか？ */
#define NR_HASH 307		/* ハッシュテーブル・エントリ数		*/
#define NR_BUFFERS nr_buffers	/* バッファキャッシュ・ノード数		*/
#define BLOCK_SIZE 1024		/* バッファキャッシュ・ブロックサイズ	*/
#ifndef NULL
#define NULL ((void *) 0)
#endif

/* １ブロック当たりのd_inode(disk-index-node)の保持数   */
#define INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct d_inode)))
/* １ブロック当たりのdir_entry(directory-entry)の保持数 */
#define DIR_ENTRIES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct dir_entry)))

typedef char buffer_block[BLOCK_SIZE];	/* charをbuffer_block[BLOCK_SIZE]に置換 */

struct buffer_head {
	char * b_data;			/* pointer to data block (1024 bytes) */	/* 読み取れたデータブロック */
	unsigned short b_dev;		/* device (0 = free) */				/* 対応しているデバイス番号 */
	unsigned short b_blocknr;	/* block number */				/* 対応しているブロック番号 */
	unsigned char b_uptodate;	/* update flog  */				/* 更新されているのかどうか */
	unsigned char b_dirt;		/* 0-clean,1-dirty */				/* 変更されているのかどうか */
	unsigned char b_count;		/* users using this block */			/* ＬＲＵで比較する参照回数 */
	unsigned char b_lock;		/* 0 - ok, 1 -locked */				/* 占有されているのかどうか */
	struct task_struct * b_wait;							/* 解除を待ち合わせるタスク */
	struct buffer_head * b_prev;							/* NR_HASHハッシュ待ち行列 */
	struct buffer_head * b_next;
	struct buffer_head * b_prev_free;
	struct buffer_head * b_next_free;
};

/* ディスク・インデックス・ノード */
struct d_inode {
	unsigned short i_mode;		/* ファイルの種類とアクセス権限	*/
	unsigned short i_uid;		/* 所有者のユーザＩＤ		*/
	unsigned long i_size;		/* ファイルサイズ:byte		*/
	unsigned long i_time;		/* 修正時刻:1970:1:1:0:0:0~	*/
	unsigned char i_gid;		/* グループＩＤ			*/
	unsigned char i_nlinks;		/* 自身にハードリンクされた数	*/
	unsigned short i_zone[9];	/* 0-6:直接 7:間接 8:二重間接	*/
};

/* メモリ・インデックス・ノード */
struct m_inode {
/* modify, access, change */
	unsigned short i_mode;		/* d_inode をそのまま保持している */
	unsigned short i_uid;
	unsigned long i_size;
	unsigned long i_mtime;
	unsigned char i_gid;
	unsigned char i_nlinks;
	unsigned short i_zone[9];
/* these are in memory also */		/* これらはメモリ内に存在している */
	struct task_struct * i_wait;	/* 保持しているプロセス		*/
	unsigned long i_atime;		/* i-node access time		*/
	unsigned long i_ctime;		/* i-node change time		*/
	unsigned short i_dev;		/* i-node device number		*/
	unsigned short i_num;		/* i-node block number		*/
	unsigned short i_count;		/* i-node使用番号,0:idle		*/
	unsigned char i_lock;		/* 保護されているかどうか		*/
	unsigned char i_dirt;		/* 変更(汚れた)のかどうか		*/
	unsigned char i_pipe;		/* パイプファイルかどうか		*/
	unsigned char i_mount;		/* マウントされたかどうか		*/
	unsigned char i_seek;		/* シークされたのかどうか		*/
	unsigned char i_update;		/* 更新済みであるかどうか		*/
};

/* プロセスによるパイプ機能を実装するには、i_zone[n]の物理アドレスを取得する */
#define PIPE_HEAD(inode) (((long *)((inode).i_zone))[0])
#define PIPE_TAIL(inode) (((long *)((inode).i_zone))[1])
#define PIPE_SIZE(inode) ((PIPE_HEAD(inode)-PIPE_TAIL(inode))&(PAGE_SIZE-1))
#define PIPE_EMPTY(inode) (PIPE_HEAD(inode)==PIPE_TAIL(inode))
#define PIPE_FULL(inode) (PIPE_SIZE(inode)==(PAGE_SIZE-1))
#define INC_PIPE(head) \
__asm__("incl %0\n\tandl $4095,%0"::"m" (head))

/* ファイル・ディスクリプタ */
struct file {
	unsigned short f_mode;		/* ファイルの種別 */
	unsigned short f_flags;		/* アクセスモード */
	unsigned short f_count;		/* 参照カウント数 */
	struct m_inode * f_inode;	/* pointer:inode */
	off_t f_pos;			/* offset:seek	 */
};

/* スーパー・ブロック */
struct super_block {
	unsigned short s_ninodes;	/* usable inodes	*/
	unsigned short s_nzones;	/* total device size	*/
	unsigned short s_imap_blocks;	/* used by inode bitmap	*/
	unsigned short s_zmap_blocks;	/* used by zone bitmap	*/
	unsigned short s_firstdatazone;	/* number of first data	*/
	unsigned short s_log_zone_size;	/* log2 of block zone	*/
	unsigned long s_max_size;	/* maxinum file size	*/
	unsigned short s_magic;		/* magic number		*/

/* These are only in memory */	/* これらはメモリ内に存在している	*/
	struct buffer_head * s_imap[8];	/* inode bitmap = 64MB */
	struct buffer_head * s_zmap[8];	/* block bitmap = 64MB */
	unsigned short s_dev;		/* whose super block is this ?	*/
	struct m_inode * s_isup;	/* inode for root directory	*/
	struct m_inode * s_imount;	/* inode mounted on		*/
	unsigned long s_time;		/* changed time			*/
	unsigned char s_rd_only;	/* 1:mounted read only		*/
	unsigned char s_dirt;		/* 1:dirty super_block		*/
};

/* ディレクトリ・エントリ */
struct dir_entry {
	unsigned short inode;		/* inode number   */
	char name[NAME_LEN];		/* directory name */
};

extern struct m_inode inode_table[NR_INODE];
extern struct file file_table[NR_FILE];
extern struct super_block super_block[NR_SUPER];
extern struct buffer_head * start_buffer;
extern int nr_buffers;

extern void truncate(struct m_inode * inode);
extern void sync_inodes(void);
extern void wait_on(struct m_inode * inode);
extern int bmap(struct m_inode * inode,int block);
extern int create_block(struct m_inode * inode,int block);
extern struct m_inode * namei(const char * pathname);
extern int open_namei(const char * pathname, int flag, int mode,
	struct m_inode ** res_inode);
extern void iput(struct m_inode * inode);
extern struct m_inode * iget(int dev,int nr);
extern struct m_inode * get_empty_inode(void);
extern struct m_inode * get_pipe_inode(void);
extern struct buffer_head * get_hash_table(int dev, int block);
extern struct buffer_head * getblk(int dev, int block);
extern void ll_rw_block(int rw, struct buffer_head * bh);
extern void brelse(struct buffer_head * buf);
extern struct buffer_head * bread(int dev,int block);
extern int new_block(int dev);
extern void free_block(int dev, int block);
extern struct m_inode * new_inode(int dev);
extern void free_inode(struct m_inode * inode);

extern void mount_root(void);

extern inline struct super_block * get_super(int dev)
{
	struct super_block * s;

	for(s = 0+super_block;s < NR_SUPER+super_block; s++)
		if (s->s_dev == dev)
			return s;
	return NULL;
}

#endif
