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