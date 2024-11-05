# ZeOS Part 4 - Process Management

## Changelog

### 4.2. Process data structures

- Added field `struct list_head list` to `task_struct` in `include/sched.h`

**Free queue**
- Added `struct list_head freequeue;` to `sched.c`
- Added all tasks to `freequeue` in `sched.c:init_sched`

**Ready queue**
- Added `struct list_head readyqueue;` to `sched.c`
- Initialized as empty in `sched.c:init_sched`

### 4.4. Process initialization

- Added field `unsigned int kernel_esp` to `task_struct` in `include/sched.h`

#### 4.4.1. Idle process

In `sched.c:init_idle`:
- Pop a task from `freequeue`
- Assign `pid = 0` and `allocate_DIR`
- Setup the stack and `task_struct.kernel_esp` for context switching
- Initialize `struct task_struct * idle_task` (declared as global in `sched.c`)


#### 4.4.2. Init process

In `sched.c:init_task1`:
- Init with `pid = 1`
- Init page entries `set_user_pages`
- Set `tss.esp0 = MSR[0x175] = kernel_esp`
- Set `cr3` to its page directory address

### 4.4.3. ZeOS shenanigans
- Delete `system.c:monoprocess_init_addr_space`


## 4.5. Process switch
- Added `switch.S:task_switch`, wrapping `inner_task_switch` (plus Makefile rules)
- Added `sched.c:inner_task_switch`
- Added `switch.S:ret_task_switch`

## 4.6, Process creation and ID
- Added `sys.C:sys_getpid`, `wrappers.S:getpid`
- Added `sys.C:sys_fork`, `wrappers.S:fork`
- Added both to `sys_call_table.S`

## 4.7. Process scheduling
- Added `int quatum` and `int ticks` to `task_struct` in `include/sched.h`
- Added `sched.c:get_quantum,set_quantum`

## 4.8. Process destruction
- Added `sys.C:sys_exit`, `wrappers.S:exit`
- Added to `sys_call_table.S`

## 4.9. Process blocking
- Added `sys.C:sys_block`, `wrappers.S:block`
- Added to `sys.C:sys_unblock`, `wrappers.S:unblock`
- Added both to `sys_call_table.S`: `table[5] = sys_block`, `table[6] = sys_unblock`
- Added entries to `sched.h:task_struct`:
  
  ```C
  int pending_unblocks;
  list_head children;
  list_head child_anchor;
  task_struct *parent;
  ```