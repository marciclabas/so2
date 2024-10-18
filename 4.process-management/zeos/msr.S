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

.extern task

// MSR[0x174] <- KERNEL_CS
// MSR[0x175] <- INITIAL_ESP
// MSR[0x176] <- syscall_handler_sysenter
ENTRY(init_msrs)
  pushl $__KERNEL_CS
  pushl $0x174
  call write_msr
  addl $8, %esp

  movl $task + 1024, %eax
  pushl %eax
  pushl $0x175
  call write_msr
  addl $8, %esp

  pushl $syscall_handler_sysenter
  pushl $0x176
  call write_msr
  addl $8, %esp

  ret