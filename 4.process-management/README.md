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
