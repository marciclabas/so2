#include <asm.h>

.extern errno

/**
  if (eax < 0)
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
  movl 4(%esp), %edx
  movl 8(%esp), %ecx
  movl 12(%esp), %ebx
  movl $4, %eax 
  int $0x80
  call return
  ret

ENTRY(gettime)
  movl $10, %eax
  int $0x80
  call return
  ret
