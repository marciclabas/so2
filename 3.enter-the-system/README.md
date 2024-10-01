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