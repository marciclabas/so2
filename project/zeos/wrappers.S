#include <asm.h>

.extern errno

/**
  if eax < 0
    errno = -eax
    return -1
  else
    return eax
*/
set_errno_eax:
  cmpl $0, %eax
  jge end
  negl %eax
  movl %eax, errno
  movl $-1, %eax
end:
  ret

// wrapper without parameters
#define WRAPPER0(name, code) \
  ENTRY(name) \
    pushl %ebx; \
    /* parameters */ \
    movl $code, %eax; \
    pushl $after_ ## name; \
    pushl %ebp; \
    movl %esp, %ebp; \
    sysenter; \
  after_ ## name: \
    popl %ebp; \
    addl $4, %esp; \
    popl %ebx; \
    call set_errno_eax; \
    ret;

// wrapper with 1 parameter
#define WRAPPER1(name, code) \
  ENTRY(name) \
    pushl %ebx; \
    /* parameters */ \
    movl 8(%esp), %edx; \
    movl $code, %eax; \
    pushl $after_ ## name; \
    pushl %ebp; \
    movl %esp, %ebp; \
    sysenter; \
  after_ ## name: \
    popl %ebp; \
    addl $4, %esp; \
    popl %ebx; \
    call set_errno_eax; \
    ret;

// wrapper with 2 parameters
#define WRAPPER2(name, code) \
  ENTRY(name) \
    pushl %ebx; \
    /* parameters */ \
    movl 8(%esp), %edx; \
    movl 12(%esp), %ecx; \
    movl $code, %eax; \
    pushl $after_ ## name; \
    pushl %ebp; \
    movl %esp, %ebp; \
    sysenter; \
  after_ ## name: \
    popl %ebp; \
    addl $4, %esp; \
    popl %ebx; \
    call set_errno_eax; \
    ret;

// wrapper with 3 parameters
#define WRAPPER3(name, code) \
  ENTRY(name) \
    pushl %ebx; \
    /* parameters */ \
    movl 8(%esp), %edx; \
    movl 12(%esp), %ecx; \
    movl 16(%esp), %ebx; \
    movl $code, %eax; \
    pushl $after_ ## name; \
    pushl %ebp; \
    movl %esp, %ebp; \
    sysenter; \
  after_ ## name: \
    popl %ebp; \
    addl $4, %esp; \
    popl %ebx; \
    call set_errno_eax; \
    ret;

WRAPPER0(exit, 1)
WRAPPER0(fork, 2)
WRAPPER3(write, 4)
WRAPPER0(block, 5)
WRAPPER1(unblock, 6)
WRAPPER1(semCreate, 7)
WRAPPER1(semWait, 8)
WRAPPER1(semSignal, 9)
WRAPPER0(gettime, 10)
WRAPPER1(semDestroy, 11)
WRAPPER3(printc_xy,12)
WRAPPER0(gettid,13)
WRAPPER2(getkey, 14)
WRAPPER2(gotoXY, 17)
WRAPPER2(changeColor,18)
WRAPPER1(clrscr,19)
WRAPPER0(getpid, 20)
WRAPPER1(memRegGet, 21)
WRAPPER1(memRegDel, 22)



// WRAPPER3(threadCreateWithStack, 16)
ENTRY(threadCreateWithStack)
  pushl %ebx
  // params
  movl 8(%esp), %edx
  movl 12(%esp), %ecx
  movl 16(%esp), %ebx
  // pass `call_and_exit`
  movl $call_and_exit, %esi

  movl $16, %eax
  pushl $after_threadCreateWithStack
  pushl %ebp
  movl %esp, %ebp
  sysenter

after_threadCreateWithStack:
  popl %ebp
  addl $4, %esp
  popl %ebx
  call set_errno_eax
  ret