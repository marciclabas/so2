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

// wrapper withtout parameters
#define WRAPPER(name, code) \
  ENTRY(name) \
    movl $code, %eax; \
    pushl %ecx; \
    pushl %edx; \
    pushl $after_ ## name; \
    pushl %ebp; \
    movl %esp, %ebp; \
    sysenter; \
  after_ ## name: \
    popl %ebp; \
    addl $4, %esp; \
    popl %edx; \
    popl %ecx; \
    call set_errno_eax; \
    ret;

ENTRY(write)
  pushl %ebx
  // parameters: fd, buffer, size -> %edx, %ecx, %ebx
  // 0(esp) is @ret
  movl 8(%esp), %edx
  movl 12(%esp), %ecx
  movl 16(%esp), %ebx

  movl $4, %eax

  pushl %ecx // why would we need to save these though?
  pushl %edx

  pushl $after_write

  pushl %ebp
  movl %esp, %ebp
  sysenter

after_write:
  popl %ebp
  addl $4, %esp // popl $after
  popl %edx
  popl %ecx
  popl %ebx

  call set_errno_eax

  ret

WRAPPER(gettime, 10)
WRAPPER(getpid, 20)
WRAPPER(fork, 2)
WRAPPER(exit, 1)