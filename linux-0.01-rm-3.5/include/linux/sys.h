/* 実装されていないシステムコールも含めて	*/
extern int sys_setup();		/* システム起動時:初期化関数 - kernel/hd.c	*/
extern int sys_exit();		/* プロセスを終了します	- kernel/exit.c		*/
extern int sys_fork();		/* プロセスを生成します	- kernel/system_call.c	*/
extern int sys_read();		/* ファイルを読み取ります	- fs/read_write.c	*/
extern int sys_write();		/* ファイルを書き込みます	- fs/read_write.c	*/
extern int sys_open();		/* ファイルを開けましょう	- fs/open.c		*/
extern int sys_close();		/* ファイルを閉じましょう	- fs/close.c		*/
extern int sys_waitpid();	/* プロセスを待機させます	- kernel/exit.c		*/
extern int sys_creat();		/* ファイルを作りましょう	- fs/open.c		*/
extern int sys_link();		/* ハードリンクを張ります	- fs/namei.c		*/
extern int sys_unlink();	/* ハードリンクを消します	- fs/namei.c		*/
extern int sys_execve();	/* ファイルを実行させます	- kernel/system_call.c	*/
extern int sys_chdir();		/* カレントディレクトリ変更 - fs.open.c		*/
extern int sys_time();		/* 現在の時刻を取得します	- kernel/sys.c		*/
extern int sys_mknod();		/* スペシャルファイル作成	- fs/namei.c		*/
extern int sys_chmod();		/* パーミッションから変更	- fs/open.c		*/
extern int sys_chown();		/* ファイルの所有者を変更	- fs/open.c		*/
extern int sys_break();		/* kernel/sys.c					*/
extern int sys_oldstat();	/* ファイル状態を取得する	- fs/stat.h (互換性)	*/
extern int sys_lseek();		/* ファイルオフセット変更	- fs/read_write.c	*/
extern int sys_getpid();	/* プロセスＩＤを取得する	- kernel/sched.c	*/
extern int sys_mount();		/* ファイルシステムマウント	- fs/super.c	*/
extern int sys_umount();	/* ファイルシステムアウマウント	- fs/super.c	*/
extern int sys_setuid();	/* プロセスユーザＩＤ設定	- kernel/sys.c		*/
extern int sys_getuid();	/* プロセスユーザＩＤ取得	- kernel/sys.c		*/
extern int sys_stime();		/* システムの日付時刻設定	- kernel/sys.c		*/
extern int sys_ptrace();	/* プログラムのトレース用	- kernel/sys.c		*/
extern int sys_alarm();		/* アラームタイーマを設定	- kernel/sched.c	*/
extern int sys_oldfstat();	/* ファイル状態を取得する	- fs/stat.c		*/
extern int sys_pause();		/* プロセスを一時中断する	- kernel/sched.c	*/
extern int sys_utime();		/* ファイルの時刻修正する	- fs/open.c		*/
extern int sys_stty();		/* 端末の回線を設定します	- kernel/sys.c		*/
extern int sys_gtty();		/* 端末の回線を取得します	- kernel/sys.c		*/
extern int sys_access();	/* パーミッションから取得	- fs/open.c		*/
extern int sys_nice();		/* プロセスの優先度を設定	- kernel/sched.c	*/
extern int sys_ftime();		/* 日付時刻を取得している	- kernel/sys.c		*/
extern int sys_sync();		/* キャッシュを書き出そう	- fs/buffer.c		*/
extern int sys_kill();		/* プロセスを終了させます	- kernel/exit.c		*/
extern int sys_rename();	/* ファイル名を変更します	- kernel/sys.c		*/
extern int sys_mkdir();		/* ディレクトリを作成する	- fs/namei.c		*/
extern int sys_rmdir();		/* ディレクトリを消去する	- fs/namei.c		*/
extern int sys_dup();		/* ファイルハンドルの複製	- fs/fcntl.c		*/
extern int sys_pipe();		/* パイプを作成しています	- fs/pipe.c		*/
extern int sys_times();		/* 実行時間を取得している	- kernel/sys.c		*/
extern int sys_prof();		/* 実行時間を計測している	- kernel/sys.c		*/
extern int sys_brk();		/* メモリブロックから取得	- kernel/sys.c		*/
extern int sys_setgid();	/* プロセスグループID設定	- kernel/sys.c		*/
extern int sys_getgid();	/* プロセスグループID取得	- kernel/sys.c		*/
extern int sys_signal();	/* シグナルを設定させます	- kernel/signal.c	*/
extern int sys_geteuid();	/* 実効ユーザIDを取得する	- kernel/sched.c	*/
extern int sys_getegid();	/* 実効ｸﾞﾙｰﾌﾟIDを取得する- kernel/sched.c	*/
extern int sys_acct();		/* プロセスアカウント設定	- kernel/sys.c		*/
extern int sys_phys();		/* 将来においても実装はされないだろう		*/
extern int sys_lock();		/* 標準カーネルには実装されないだろう		*/
extern int sys_ioctl();		/* デバイスを制御している	- fs/ioctl.c		*/
extern int sys_fcntl();		/* ファイルを制御している	- fs/fcntl.c		*/
extern int sys_mpx();		/* 標準カーネルには実装されないだろう		*/
extern int sys_setpgid();	/* プロセスグループID設定	- kernel/sys.c		*/
extern int sys_ulimit();	/* 標準カーネルには実装されないだろう		*/
extern int sys_oldolduname();	/* 廃止されているシステムコール			*/
extern int sys_umask();		/* ファイル作成時のマスク	- kernel/sys.c		*/
extern int sys_chroot();	/* ルートディレクトリ変更	- fs/open.c		*/
extern int sys_ustat();		/* ファイルシステムの情報	- fs/open.c		*/
extern int sys_dup2();		/* ファイルハンドルの複製	- fs/fcntl.c		*/
extern int sys_getppid();	/* 親プロセスIDを取得する	- kernel/sched.c	*/
extern int sys_getpgrp();	/* プロセスグループの取得	- kernel/sys.c		*/
extern int sys_setsid();	/* 新しいセッションの作成	- kernel/sys.c		*/

/* 移植したときに追加されたシステムコール	*/
extern int sys_stat();
extern int sys_lstat();
extern int sys_fstat();
extern int sys_uname();
extern int sys_getdents();
extern int sys_stat64();
extern int sys_lstat64();
extern int sys_fstat64();
extern int sys_getdents64();
extern int sys_fcntl64();
extern int sys_null();	/* not implemented sys_call */

/* int 0x80によりソフトウェア割り込みをかけて、IDT[0x80]に対応する
   トラップゲート・ディスクリプタの参照で、以下のテーブルが使われる	*/
fn_ptr sys_call_table[] = { sys_setup, sys_exit, sys_fork, sys_read,
sys_write, sys_open, sys_close, sys_waitpid, sys_creat, sys_link,
sys_unlink, sys_execve, sys_chdir, sys_time, sys_mknod, sys_chmod,
sys_chown, sys_break, sys_oldstat, sys_lseek, sys_getpid, sys_mount,
sys_umount, sys_setuid, sys_getuid, sys_stime, sys_ptrace, sys_alarm,
sys_oldfstat, sys_pause, sys_utime, sys_stty, sys_gtty, sys_access,
sys_nice, sys_ftime, sys_sync, sys_kill, sys_rename, sys_mkdir,
sys_rmdir, sys_dup, sys_pipe, sys_times, sys_prof, sys_brk, sys_setgid,
sys_getgid, sys_signal, sys_geteuid, sys_getegid, sys_acct, sys_phys,
sys_lock, sys_ioctl, sys_fcntl, sys_mpx, sys_setpgid, sys_ulimit,
sys_oldolduname, sys_umask, sys_chroot, sys_ustat, sys_dup2, sys_getppid,
sys_getpgrp,sys_setsid, sys_null /*67*/,sys_null,sys_null,

/* 70 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 80 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 90 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 100 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_stat,sys_lstat,sys_fstat,sys_null,
/* 110 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 120 */sys_null,sys_null,sys_uname,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 130 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 140 */sys_null,sys_getdents,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 150 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 160 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 170 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 180 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 190 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_stat64,sys_lstat64,sys_fstat64,sys_null,sys_null,
/* 200 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 210 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 220 */sys_getdents64,sys_fcntl64,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 230 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 240 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 250 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 260 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 270 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 280 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 290 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 300 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 310 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null
};

