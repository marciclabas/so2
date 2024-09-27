#include <asm.h>

.extern errno

/**
  if eax < 0
    errno = -eax
    return -1
  else
    return eax
*/
return:
  cmpl $0, %eax
  jge end
  negl %eax
  movl %eax, errno
  movl $-1, %eax
end:
  ret

ENTRY(write)
  // parameters: fd, buffer, size -> %edx, %ecx, %ebx
  pushl %ebx // C convention: only %ebx, %esi, %edi are callee-saved if used
  movl 8(%ebp), %edx
  movl 12(%ebp), %ecx
  movl 16(%ebp), %ebx
  movl $4, %eax 
  int $0x80
  popl %ebx
  call return
  ret

ENTRY(gettime)
  movl $10, %eax
  int $0x80
  call return
  ret
