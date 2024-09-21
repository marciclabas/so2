#include <asm.h>

ENTRY(addAsm)
  movl 8(%esp), %eax
  addl 12(%esp), %eax
  ret