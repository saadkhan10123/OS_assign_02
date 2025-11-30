# Process Scheduling Assignment - xv6 Priority Scheduler

## Setup & Run Instructions

1.  **Build the kernel**:
    ```bash
    make
    ```

2.  **Run QEMU**:
    ```bash
    make qemu
    ```

3.  **Run Tests**:
    Inside the QEMU shell, run the following commands:
    - `test_basic`: Verifies that higher priority processes finish earlier.
    - `test_stress`: Runs multiple processes with random priorities to ensure stability.
    - `test_edge`: Tests edge cases like invalid priority values.

## Feature Implemented
**Priority-Based Scheduler**
- Added `priority` field to `struct proc`.
- Modified `scheduler()` to select the `RUNNABLE` process with the highest priority (0-20, 20 is highest).
- Implemented `setpriority(pid, priority)` system call.

## Files Modified
- `kernel/proc.h`: Added `priority` to `struct proc`.
- `kernel/proc.c`: Initialized priority, implemented scheduling logic, added `set_priority` helper.
- `kernel/syscall.h`: Added `SYS_setpriority`.
- `kernel/syscall.c`: Registered `sys_setpriority`.
- `kernel/sysproc.c`: Implemented `sys_setpriority`.
- `kernel/defs.h`: Added prototype for `set_priority`.
- `user/user.h`: Added user-space prototype.
- `user/usys.pl`: Added user-space entry.
- `Makefile`: Added test programs.
