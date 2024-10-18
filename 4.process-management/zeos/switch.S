#include <asm.h>

ENTRY(task_switch)
  movl %esp, %eax // arg
  pushl %edi
  pushl %esi
  pushl %ebx
  pushl %eax // forward arg
  call inner_task_switch
  addl $4, %esp
  popl %ebx
  popl %esi
  popl %edi
  ret
