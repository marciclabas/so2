# 0 "wrappers.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "wrappers.S"
# 1 "include/asm.h" 1
# 2 "wrappers.S" 2

.extern errno
# 12 "wrappers.S"
set_errno_eax:
  cmpl $0, %eax
  jge end
  negl %eax
  movl %eax, errno
  movl $-1, %eax
end:
  ret
# 42 "wrappers.S"
.globl write; .type write, @function; .align 0; write:
  pushl %ebx



  movl 8(%esp), %edx
  movl 12(%esp), %ecx
  movl 16(%esp), %ebx

  movl $4, %eax

  pushl %ecx
  pushl %edx

  pushl $after_write

  pushl %ebp
  movl %esp, %ebp
  sysenter

after_write:
  popl %ebp
  addl $4, %esp
  popl %edx
  popl %ecx
  popl %ebx

  call set_errno_eax

  ret

.globl gettime; .type gettime, @function; .align 0; gettime:
  movl $10, %eax

  pushl %ecx
  pushl %edx

  pushl $after_gettime

  pushl %ebp
  movl %esp, %ebp
  sysenter

after_gettime:
  popl %ebp
  addl $4, %esp
  popl %edx
  popl %ecx
  call set_errno_eax

  ret
