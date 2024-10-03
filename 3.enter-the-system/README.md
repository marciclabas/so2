# ZeOS Part 3 - Mechanisms to Enter the System

## Changelog

### 3.10.1 Complete ZeOS Code
- Added `RESTORE_ALL` to `entry.S`
- Added `EOI` to `entry.S`

### 3.10.2 Keyboard Interrupt
- Interrupt Service Routine (ISR): added `keyboard_routine` to `interrupt.c`, `zeos_interrupt.h`
- Interrupt handler: added `keyboard_handler` to `entry.S`, `interrupt.h`
- Initialize IDT: added `setInterruptHandler(33, keyboard_handler, 0);` to `interrupt.c`
- Enable interrupt: set flag to 0 in `hardware.c:enable_int`

### 3.10.3 Write Syscall

**A) Interrupt mechanism**
- Routine: added `sys_write` to `sys.c`
- Sycall table: `sys_call_table[4] = sys_write` in `sys_call_table.S`
- Wrapper: added `write_int` to `wrappers.S` (new file), `libc.h`
- Syscall handler: added `system_call_handler` to `entry.S`
- Initialize IDT: added `setTrapHandler(0x80, system_call_handler, 3);` to `interrupt.c`
- `errno`: added code to set `errno` in `wrappers.S` (per wrapper)
- `perror`: added `perror` to `lib.c/h`

**B) Fast syscall mechanism**
- Init MSRs: added `init_msrs` to `entry.S`, `system.h`. Called from `system.c:main`
- Added `syscall_handler_sysenter` to `entry.S`
- Added `write_msr`, `init_msrs` to `entry.S`
- Added `write` to `wrappers.S`, `libc.h`

### 3.10.4. Clock Interrupt
- ISR: added `clock_routine` to `interrupt.c`, `zeos_interrupt.h`
- Handler: added `clock_handler` to `entry.S`, `interrupt.h`
- IDT: added `setInterruptHandler(32, clock_handler, 0);` to `interrupt.c`
- Enable interrupt: set flag to 0 in `hardware.c:enable_int`

### 3.10.5 Gettime syscall
- Counter:
  - Added `int zeos_ticks = 0;` to `system.c`, `system.h`
  - Added `zeos_ticks++` to `interrupt.c:clock_routine`
- Routine: added `sys_gettime` to `sys.c`
- Syscall table: `sys_call_table[10] = sys_gettime` in `sys_call_table.S`
- Wrapper (fast syscall): added `gettime` to `wrappers.S`, `libc.h`

### 3.10.6 Page Faults

- ISR: added `page_fault_routine2(int addr)` to `interrupt.c`, `zeos_interrupt.h`
- Interrupt handler: added `page_fault_handler2` to `entry.S`, `interrupt.h`, passing `%cr2` to the ISR
- IDT: added `setInterruptHandler(14, page_fault_handler2, 0);` to `interrupt.c`



## Problems

Something weird happens with local variables.

### Example 1

```C
int last_time = -10000;

int main() {
  while (1) {
    int time = gettime();
    if (time > last_time + 100) {
      // print time
      last_time = time;
    }
  }
}
```

But if we move the declaration in, `last_time` doesn't get updated:

```C
int main() {
  int last_time = 0;
  while (1) {
    int time = gettime();
    if (time > last_time + 100) {
      // print time
      last_time = time;
    }
  }
}
```

### Example 2

This works fine:

```C
char buffer[10];

void print_int(int num) {
  itoa(num, buffer);
  write(1, buffer, strlen(buffer));
}
```

But moving the buffer in yiels page faults:


```C
void print_int(int num) {
  char buffer[10];
  itoa(num, buffer);
  write(1, buffer, strlen(buffer));
}
```