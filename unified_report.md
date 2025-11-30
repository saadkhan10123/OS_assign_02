# Unified Report: Priority-Based Scheduler in xv6

## 1. Setup Guide

**Tools Used:**
- **OS**: Linux (Ubuntu)
- **Kernel**: xv6-riscv
- **Emulator**: QEMU (qemu-system-riscv64)
- **Compiler**: gcc-riscv64-linux-gnu

**How to Compile and Run:**
1.  **Build the kernel**:
    ```bash
    make
    ```
2.  **Run QEMU**:
    ```bash
    make qemu
    ```
3.  **Exit QEMU**:
    Press `Ctrl-A` followed by `x`.

## 2. Implementation Summary

**Selected Feature:** Priority-Based Scheduler

**Goal:**
Replace the default Round-Robin scheduler with a priority-based scheduler where processes with higher priority values are selected first.

**Changes Made:**
1.  **Data Structures**:
    -   Modified `struct proc` in `kernel/proc.h` to include an integer `priority`.
    -   **Priority Range**: 0 (lowest) to 20 (highest). Default priority is 10.

2.  **Scheduling Logic**:
    -   Modified `scheduler()` in `kernel/proc.c`.
    -   The scheduler now iterates through the entire process table to find the `RUNNABLE` process with the highest `priority`.
    -   If multiple processes share the highest priority, the first one found is selected (First-Come-First-Served among equals).

3.  **System Call**:
    -   Implemented `sys_setpriority` to allow changing a process's priority.
    -   Added `setpriority(int pid, int priority)` system call.
    -   Added validation to ensure priority is within the 0-20 range.

**Files Modified:**
-   `kernel/proc.h`: Added `priority` field.
-   `kernel/proc.c`: Scheduler logic, initialization, helper function.
-   `kernel/syscall.h`, `kernel/syscall.c`, `kernel/sysproc.c`: System call implementation.
-   `kernel/defs.h`: Helper prototype.
-   `user/user.h`, `user/usys.pl`: User-space interface.
-   `Makefile`: Added test programs.

**Challenges Faced:**
-   **Concurrency**: Ensuring thread safety while scanning the process table for the highest priority process.
-   **Integration**: Correctly wiring up the new system call across kernel and user space.
-   **Verification**: Designing tests that effectively demonstrate priority differences in a short-lived environment.

## 3. Testing

Three test programs were implemented to validate the feature:

1.  **`test_basic`**:
    -   Forks 3 children with Low (5), Medium (10), and High (15) priorities.
    -   **Result**: High priority process finishes first, followed by Medium, then Low.

2.  **`test_stress`**:
    -   Forks 20 processes with random priorities.
    -   **Result**: System remains stable and all processes complete.

3.  **`test_edge`**:
    -   Tests invalid priority values (e.g., -1, 21).
    -   **Result**: System call correctly returns -1 for invalid inputs.

**How to Run Tests:**
Inside the QEMU shell, execute:
```bash
test_basic
test_stress
test_edge
```
