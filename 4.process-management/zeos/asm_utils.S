#include <asm.h>
#include <segment.h>

// write_msr(msr, value)
ENTRY(write_msr)
  movl 4(%esp), %ecx
  movl 8(%esp), %eax
  movl $0, %edx
  // MSR[%ecx] <- %edx:%eax
  wrmsr
  ret
  
ENTRY(read_ebp)
  movl %ebp, %eax
  ret