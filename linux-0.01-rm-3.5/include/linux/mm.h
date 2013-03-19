#ifndef _MM_H
#define _MM_H

#define PAGE_SIZE 4096	/* １ページ=4KB	*/

/* mem_map[p]の終端より空ページを取得する	*/
extern unsigned long get_free_page(void);
/* mem_map[p]の任意より埋ページを挿入する	*/
extern unsigned long put_page(unsigned long page,unsigned long address);
/* mem_map[p]の任意より空ページを設定する	*/
extern void free_page(unsigned long addr);

#endif
