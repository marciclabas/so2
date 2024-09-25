# 3. Enter the System

## Conventions

e.g. `<name> = write`

### Handler
- Assembly code: `entry.S:<name>_handler`, e.g. `syscall_handler`
- Header: `include/interrupt.h`
- Onto the IDT: `setInterruptHandler(<num>, <name>_handler, priv_level)` in `interrupt.c`

```assembly
ENTRY(<name>_handler)
  SAVE_ALL
  EOI
  call <name>_routine;
  RESTORE_ALL
  iret;
```

- Masked interrupt enable: change mask in `hardware.c:enable_int`

### Interrupt Service Routines, ISRs - Interrupt only
- C code: `interrupt.c:<name>_routine`, e.g. `clock_routine`
- Header: `include/zeos_interrupt.h`
- Called from `<name>_handler`

### Service Routines - System calls
- C code: `sys.c:sys_<name>`, e.g. `sys_write`
- Called from `syscall_handler`, using the syscall table

### Wrappers
- Assembly code: `wrappers.S:<name>`, e.g. `write`
- Header: `include/libc.h`

## TODO

#### 3.10.1
- RESTORE_ALL: ok
- EOI: ok

#### 3.10.2 Keyboard mgmt
- ISR: `ìnterrupt.c/zeos_interrupt.h: keyboard_routine` ok
- Handler: `entry.S/interrupt.h: keyboard_handler` ok
- IDT <- handler: `interrupt.c: setInterruptHandler(33, keyboard_handler, 0);`ok
- Enable interrupt: `hardware.c: enable_int, 0xfd` ok

#### 3.10.3 write syscall

- `sys_write` (ISR): `sys.c` ok
- sys_call_table <- sys_write: `sys_call_table.S` ok
- `write` wrapper: `wrappers.S` ok
- `system_call_handler` : `entry.S` ok
- IDT <- handler: `interrupt.c` ok
- MSR <- handler
- `errno`, `perror`

#### 3.10.4 Clock

- service routine?: `sys.c`
- handler: `entry.S`
- IDT <- handler