#include <asm.h>

ENTRY(write)
  // parameters: fd, buffer, size -> %edx, %ecx, %ebx
  movl 4(%esp), %edx
  movl 8(%esp), %ecx
  movl 12(%esp), %ebx
  movl $4, %eax
  int $0x80
  // if result >= 0, return as is; else errno <- |result|, return -1
  cmpl $0, %eax
  jge end
  movl $-1, %eax
end:
  ret