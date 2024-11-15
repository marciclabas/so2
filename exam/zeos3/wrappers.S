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

ENTRY(exit)
  pushl %ebx;
  movl 8(%esp), %edx; // parametre
  movl $1, %eax;
  pushl $after_exit;
  pushl %ebp;
  movl %esp, %ebp;
  sysenter;
after_exit:
  popl %ebp;
  addl $4, %esp;
  popl %ebx;
  call set_errno_eax;
  ret;

ENTRY(waitpid)
  pushl %ebx
  movl 8(%esp), %edx
  movl 12(%esp), %ecx
  int $130
  
  popl %ebx
  call set_errno_eax
  ret

WRAPPER(fork, 2)
WRAPPER(write, 4)
WRAPPER(block, 5)
WRAPPER(unblock, 6)
WRAPPER(gettime, 10)
WRAPPER(getpid, 20)