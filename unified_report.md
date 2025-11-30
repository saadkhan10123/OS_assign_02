# Process Management in xv6: A Developer's Journey

## 1. Introduction: The Choice of xv6
For this assignment, I chose **xv6** as the foundation for my work. I chose xv6 because it is lightweight compared to the full Linux kernel and designed specifically to help students tinker with OS fundamentals. Its clean, ANSI C implementation makes it an ideal sandbox for understanding how core kernel mechanisms—like scheduling—actually work under the hood.

## 2. Phase 1: Setting the Stage (Environment Setup)
My journey began with setting up a reliable development environment. I used **Ubuntu Linux** as my host OS, leveraging the standard GNU toolchain (`gcc-riscv64-linux-gnu`) to cross-compile the kernel for the RISC-V architecture.

To run the kernel, I used **QEMU**, a versatile emulator that allows for rapid iteration without the need for physical hardware.
-   **Build Command**: `make`
-   **Run Command**: `make qemu`

Once I saw the `init: starting sh` prompt, I knew I had a working system ready for modification.

## 3. Phase 2: Understanding the Core (Code Analysis)
Before writing any code, I needed to understand how xv6 currently handles processes. I dove into `kernel/proc.c` and `kernel/proc.h`.

I identified three key components that would be central to my task:
1.  **`struct proc`**: The process control block. It contained the state, PID, and context, but lacked any notion of "importance" or priority.
2.  **`scheduler()`**: The heart of the system. I found it running a simple Round-Robin loop—blindly picking the next `RUNNABLE` process in the list.
3.  **`allocproc()`**: The function responsible for birthing new processes. This is where I would need to initialize any new attributes I added.

## 4. Phase 3: The Implementation Journey
I decided to implement a **Priority-Based Scheduler**. The goal was simple but impactful: replace the egalitarian Round-Robin approach with one that respects a hierarchy of importance.

### Step 1: Defining Priority
First, I modified `struct proc` in `kernel/proc.h` to add an integer `priority`. I decided on a scale of **0 to 20**, where 20 represents the highest urgency. I updated `allocproc()` to assign a default priority of **10** to every new process, ensuring backward compatibility.

### Step 2: Rewriting the Scheduler
The most critical change was in `scheduler()` in `kernel/proc.c`. I replaced the simple iterator with a search loop. Now, instead of just taking the next available process, the scheduler scans the entire table to find the `RUNNABLE` process with the *highest* priority. If multiple processes share the top spot, it picks the first one it finds, preserving a degree of fairness among equals.

### Step 3: Empowering the User
A priority system is useless if users can't control it. I implemented a new system call, `sys_setpriority`. This involved a tour through the entire kernel-to-user interface:
-   **Kernel**: Added `sys_setpriority` in `sysproc.c` to validate inputs and update the target process.
-   **User Space**: Exposed the call via `user.h` and `usys.pl` so user programs could invoke it.

## 5. Phase 4: Validation (Testing)
To prove the system worked, I wrote three test programs, treating them as experiments to validate my hypothesis.

-   **The Basic Test (`test_basic`)**: I created three child processes with Low (5), Medium (10), and High (15) priorities. As expected, the High priority child finished its work first, followed by Medium, and then Low. This confirmed the scheduler was honoring the hierarchy.
-   **The Stress Test (`test_stress`)**: I spawned 20 processes with random priorities to ensure my search loop didn't introduce instability or deadlocks. The system handled the load gracefully.
-   **The Edge Case Test (`test_edge`)**: I tried to break the system with invalid priorities (like -1 or 21). The system call correctly rejected them, proving the robustness of the input validation.

## Conclusion
This project took me from a basic understanding of process scheduling to a hands-on implementation of a priority-based system. By modifying xv6, I gained a deeper appreciation for the delicate balance of performance and fairness that operating systems must maintain.
