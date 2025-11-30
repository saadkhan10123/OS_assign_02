# Implementation Report: Priority-Based Scheduler in xv6

## 1. Setup Guide

**Tools Used:**
- **OS**: Linux (Ubuntu)
- **Kernel**: xv6-riscv
- **Emulator**: QEMU (qemu-system-riscv64)
- **Compiler**: gcc-riscv64-linux-gnu

**How to Compile and Run:**
1.  Navigate to the `xv6-riscv` directory.
2.  Run `make` to compile the kernel and user programs.
3.  Run `make qemu` to start the xv6 operating system in QEMU.
4.  To exit QEMU, press `Ctrl-A` followed by `x`.

## 2. Implementation Summary

**Selected Feature:** Priority-Based Scheduler

**Changes Made:**
I implemented a non-preemptive priority scheduler (though xv6 is preemptive via timer interrupts, the selection logic happens when `scheduler()` runs).

1.  **Data Structures**:
    - Modified `struct proc` in `kernel/proc.h` to include an integer `priority`.
    - Priority range: 0 (lowest) to 20 (highest). Default priority is 10.

2.  **Scheduling Logic**:
    - Modified `scheduler()` in `kernel/proc.c`.
    - Instead of the original Round-Robin loop, the scheduler now iterates through the entire process table to find the `RUNNABLE` process with the highest `priority`.
    - If multiple processes have the same highest priority, the first one found is selected.

3.  **System Call**:
    - Implemented `sys_setpriority` to allow changing a process's priority.
    - Added `setpriority(int pid, int priority)` system call.
    - Validates that priority is within the 0-20 range.

**Challenges Faced:**
- **Scheduler Loop**: Replacing the simple Round-Robin loop required careful handling of locks. I had to ensure that I scan all processes to find the best candidate without holding locks unnecessarily long, and then acquire the lock of the chosen process to switch to it.
- **System Call Integration**: Adding a new system call requires touching multiple files (`syscall.h`, `syscall.c`, `sysproc.c`, `user.h`, `usys.pl`). Missing any one of them causes build errors.
- **Testing**: Verifying the scheduler required writing CPU-bound loops, as simple print statements might execute too fast to show scheduling differences.

## 3. Testing

Three test programs were created:
1.  `test_basic.c`: Forks 3 children with low, medium, and high priorities. Confirms that high priority processes get more CPU time/finish earlier.
2.  `test_stress.c`: Forks 20 processes with random priorities to ensure the kernel remains stable under load.
3.  `test_edge.c`: Tests invalid inputs to `setpriority` (e.g., -1, 21) to ensure error handling works.
