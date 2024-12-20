#include <asm.h>
#include <segment.h>

ENTRY(sys_call_table)
	.long sys_ni_syscall // 0
	.long sys_exit // 1
	.long sys_fork // 2
	.long sys_ni_syscall // 3
	.long sys_write // 4
	.long sys_block // 5
	.long sys_unblock // 6
	.long sys_semCreate // 7
	.long sys_semWait // 8
	.long sys_semSignal // 9
	.long sys_gettime // 10
	.long sys_semDestroy // 11
	.long printc_xy // 12
	.long sys_gettid // 13
	.long sys_getkey // 14
	.long sys_ni_syscall // 15
	.long sys_threadCreateWithStack // 16
	.long sys_gotoXY // 17
	.long sys_changeColor // 18
	.long sys_clrscr // 19
	.long sys_getpid // 20
	.long sys_memRegGet // 21
	.long sys_memRegDel // 22

.globl MAX_SYSCALL
MAX_SYSCALL = (. - sys_call_table)/4
