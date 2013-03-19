#ifndef __DIRENT_H
#define __DIRENT_H

#include <linux/fs.h>
#include <stdint.h>

#define NAME_MAX NAME_LEN

/* struct directory entry */
struct dirent{
	long d_ino;		 /* i-node 番号	*/
	off_t d_off;		 /* offset 基底から次エントリへ	*/
	unsigned short d_reclen; /* offset 現在から次エントリへ	*/
	char d_name[256];	 /* ファイル名	*/
};

/* 64 bit interface */
struct dirent64 {
  uint64_t	d_ino;		 /* i-node number */
  int64_t	d_off;		 /* offset to next dirent   */
  uint16_t	d_reclen;	 /* length of this record   */
  unsigned char	d_type;		 /* filetype */
  char		d_name[256];	 /* filename */
};

/* ディレクトリ・ストリーム */
struct __DIR{
	struct dirent64 d_dent;
	int d_pos;	/* record position */
	int d_fd;	/* file descriptor */
};

#define DIR struct __DIR 

DIR *opendir (const char *name);
int closedir (DIR *dir);
struct dirent *readdir (DIR *dir);

#endif 
