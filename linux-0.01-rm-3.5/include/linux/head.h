#ifndef _HEAD_H
#define _HEAD_H

/*
 * Kernel Setup Code(head.s)で定義された
 * pg_dir, gdt, ldtを利用する (extern)
 */

/* defined segment descriptor */
typedef struct desc_struct {
	unsigned long a,b;
} desc_table[256];

/* page directory table */
extern unsigned long pg_dir[1024];
/* interrupt descriptor table 
   global descriptor table	*/
extern desc_table idt,gdt;

#define GDT_NUL 0	/* GDT[0] = NULL Segment	*/
#define GDT_CODE 1	/* GDT[1] = Kernel Code Segment	*/
#define GDT_DATA 2	/* GDT[2] = Kernel Data Segment	*/
#define GDT_TMP 3	/* GDT[3] = System Segment	*/

#define LDT_NUL 0	/* LDT[0] = NULL Segmentj	*/
#define LDT_CODE 1	/* LDT[1] = User Code Segment	*/
#define LDT_DATA 2	/* LDT[2] = User Data Segment	*/

#endif
