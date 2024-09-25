# 3. Enter the System

## Conventions

### Handlers
- Onto the IDT: `setInterruptHandler(<num>, <name>_handler, priv_level)` in `interrupt.c`
- Assembly code: `entry.S`, named `<name>_handler`

```assembly
ENTRY(<name>_handler)
  SAVE_ALL
  EOI
  call <name>_routine;
  RESTORE_ALL
  iret;
```

- Header: `include/zeos_interrupt.h`
- Enabling interrupts: change mask in `enable_int()`, in `hardware.c`

### Routines (Interrupt Service Routines, ISRs)
- Called from `<name>_handler`
- C code: `interrupt.c`, named `<name>_routine`
- Header: `include/zeos_interrupt.h`

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
