#include <asm.h>

.extern errno

/**
  if eax < 0
    errno = -eax
    return -1
  else
    return eax
*/
set_errno_eax:
  cmpl $0, %eax
  jge end
  negl %eax
  movl %eax, errno
  movl $-1, %eax
end:
  ret

// wrapper without parameters
#define WRAPPER(name, code) \
  ENTRY(name) \
    pushl %ebx; \
    /* parameters */ \
    movl 8(%esp), %edx; \
    movl 12(%esp), %ecx; \
    movl 16(%esp), %ebx; \
    movl $code, %eax; \
    pushl $after_ ## name; \
    pushl %ebp; \
    movl %esp, %ebp; \
    sysenter; \
  after_ ## name: \
    popl %ebp; \
    addl $4, %esp; \
    popl %ebx; \
    call set_errno_eax; \
    ret;

WRAPPER(exit, 1)
WRAPPER(fork, 2)
WRAPPER(write, 4)
WRAPPER(block, 5)
WRAPPER(unblock, 6)
WRAPPER(gettime, 10)
WRAPPER(getpid, 20)
WRAPPER(getkey, 14)
WRAPPER(gotoXY, 17)
WRAPPER(changeColor,18)
WRAPPER(clrscr,19)
