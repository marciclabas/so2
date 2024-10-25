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

ENTRY(gettime)
  movl $10, %eax

  pushl %ecx
  pushl %edx

  pushl $after_gettime

  pushl %ebp
  movl %esp, %ebp
  sysenter

after_gettime:
  popl %ebp
  addl $4, %esp // popl $after
  popl %edx
  popl %ecx
  call set_errno_eax

  ret



ENTRY(getpid)
  movl $10, %eax

  pushl %ecx
  pushl %edx

  pushl $after_getpid

  pushl %ebp
  movl %esp, %ebp
  sysenter

after_getpid:
  popl %ebp
  addl $4, %esp // popl $after
  popl %edx
  popl %ecx
  call set_errno_eax

  ret


ENTRY(fork)
  movl $10, %eax

  pushl %ecx
  pushl %edx

  pushl $after_fork

  pushl %ebp
  movl %esp, %ebp
  sysenter

after_fork:
  popl %ebp
  addl $4, %esp // popl $after
  popl %edx
  popl %ecx
  call set_errno_eax

  ret