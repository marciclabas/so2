#include <asm.h>

ENTRY(task_switch)
  movl 4(%esp), %eax // arg
  pushl %edi
  pushl %esi
  pushl %ebx
  pushl %ebp
  pushl %eax // forward arg
  call inner_task_switch
  addl $4, %esp
  popl %ebp
  popl %ebx
  popl %esi
  popl %edi
  ret
