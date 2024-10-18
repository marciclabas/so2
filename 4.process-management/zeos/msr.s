# 0 "msr.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "msr.S"
# 1 "include/asm.h" 1
# 2 "msr.S" 2
# 1 "include/segment.h" 1
# 3 "msr.S" 2


.globl write_msr; .type write_msr, @function; .align 0; write_msr:
  movl 4(%esp), %ecx
  movl 8(%esp), %eax
  movl $0, %edx

  wrmsr

  ret

.extern task




.globl init_msrs; .type init_msrs, @function; .align 0; init_msrs:
  pushl $0x10
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
