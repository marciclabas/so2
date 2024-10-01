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

ENTRY(write_int)
  // parameters: fd, buffer, size -> %edx, %ecx, %ebx
  // 0(esp) is @ret
  movl 4(%esp), %edx
  movl 8(%esp), %ecx
  movl 12(%esp), %ebx

  pushl %ebx

  movl $4, %eax
  int $0x80
  
  popl %ebx

  call set_errno_eax

  ret


ENTRY(write)
  // parameters: fd, buffer, size -> %edx, %ecx, %ebx
  // 0(esp) is @ret
  movl 4(%esp), %edx
  movl 8(%esp), %ecx
  movl 12(%esp), %ebx

  movl $4, %eax

  pushl %ecx // why would we need to save these though?
  pushl %edx

  pushl $after

  pushl %ebp
  movl %esp, %ebp
  sysenter

after:
  popl %ebp
  addl $4, %esp // popl $after
  popl %edx
  popl %ecx
  call set_errno_eax

  ret

ENTRY(gettime)
  // movl $10, %eax
  // int $0x80
  // call return
  // ret
