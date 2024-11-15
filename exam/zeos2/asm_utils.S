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

ENTRY(read_ebx)
  movl %ebx, %eax
  ret

ENTRY(read_esi)
  movl %esi, %eax
  ret

ENTRY(read_edi)
  movl %edi, %eax
  ret

ENTRY(write_ebx)
  movl 4(%esp), %ebx
  ret

ENTRY(write_esi)
  movl 4(%esp), %esi
  ret

ENTRY(write_edi)
  movl 4(%esp), %edi
  ret