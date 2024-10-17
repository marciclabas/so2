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
