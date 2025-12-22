# Technical Report: Evaluation & Enhancement of xv6 Priority Scheduler

## 1. Introduction
This report evaluates the **Priority-Based Scheduler** implemented in the xv6 operating system (Assignment 02). It analyzes the standardized performance behavior under various loads, identifies a critical limitation regarding process starvation, and proposes a mechanism—**Process Aging**—to enhance fairness without compromising the priority-based architecture.

## 2. OS Feature Overview
The implemented feature is a **Non-Preemptive Priority Scheduler**.
-   **Mechanism**: The standard Round-Robin scheduler was replaced with a search algorithm that iterates through the process table (`proc[]`) to identify the `RUNNABLE` process with the highest priority value.
-   **Structure**: A `priority` integer field (range 0-20) was added to `struct proc`.
-   **Control**: A system call `setpriority(pid, priority)` allows users to dynamically adjust process importance.
-   **Default**: All new processes are initialized with a default priority of 10.

## 3. Evaluation & Limitation Analysis

### Observed Behavior
1.  **Normal Load**:
    -   Under standard conditions (e.g., shell commands, simple user programs), the system behaves identically to the stock scheduler because default priorities are equal (10).
    -   When explicit priorities are set (as in `test_basic`), the strict hierarchy is observed: processes with priority 15 consistently finish before those with priority 5.

2.  **Stress & Edge Cases**:
    -   **Stress**: In `test_stress` (20 concurrent processes with random priorities), the system remained stable. The scheduler overhead from scanning the loop ($O(N)$ where $N=64$) is negligible for the default process table size.
    -   **Edge Cases**: Invalid inputs (<0 or >20) are correctly rejected, preventing memory corruption or undefined behavior.

### Critical Limitation: Starvation
**Inefficiency**: The strict priority logic guarantees **Indefinite Blocking (Starvation)** for low-priority processes.
**Evidence**: If a high-priority process (e.g., Priority 20) enters an infinite CPU-bound loop, no `RUNNABLE` process with Priority < 20 will ever be selected by the scheduler. The scheduler loop will always find the Priority 20 process first.
-   In a real-world scenario, a rogue high-priority background service could render the interactive shell (default priority 10) unresponsive.

## 4. Enhancement Proposal: Process Aging

To mitigate starvation, I propose implementing **Process Aging**.

### High-Level Design
The concept is to gradually increase the effective priority of waiting processes over time. If a low-priority process stays `RUNNABLE` but is not chosen for a long duration, its priority should temporarily boost until it runs.

1.  **Wait Time Tracking**: Add a `ticks_waiting` field to `struct proc`.
2.  **Aging Threshold**: Define a `STARVATION_LIMIT`. If `ticks_waiting` exceeds this, increment priority.
3.  **Reset**: Once a process runs, reset its priority to the user-defined base priority.

### Pseudocode / Logic Flow

**Modified Scheduler Loop:**

```c
// Constants
#define STARVATION_LIMIT 100 // Ticks before boosting
#define MAX_PRIORITY 20

void scheduler(void) {
  struct proc *p;
  
  for(;;) {
    intr_on(); // Enable interrupts

    // 1. Aging Pass (Prevention)
    // Run periodically or on every tick
    for(p = proc; p < &proc[NPROC]; p++) {
        if(p->state == RUNNABLE) {
            p->wait_ticks++;
            // If starved, boost priority
            if(p->wait_ticks > STARVATION_LIMIT) {
                if(p->priority < MAX_PRIORITY) {
                    p->priority++; 
                }
                p->wait_ticks = 0; // Reset counter for next boost
            }
        }
    }

    // 2. Selection Pass (Existing Logic)
    struct proc *highest_p = find_highest_priority_runnable();

    if(highest_p) {
        // Switch to process
        swtch(&c->context, &highest_p->context);
        
        // Reset waiting ticks after running
        highest_p->wait_ticks = 0;
    }
  }
}
```

## 5. Impact & Trade-offs Analysis

| Metric | Impact | Trade-offs |
| :--- | :--- | :--- |
| **Performance** | Slight Overhead | The scheduler now performs an additional "Aging Pass" or check. In xv6 ($N=64$), this is trivial. In systems with thousands of processes, iterating simply to age them is expensive ($O(N)$). |
| **Stability** | Improved | Prevents complete system lockup caused by runaway high-priority processes. Ensures system remains interactive even under load. |
| **Complexity** | Increased | Requires tracking wait times (or `ticks` at arrival) and managing dynamic priority shifts. We must distinguish between "user-set priority" and "current boosted priority" to un-boost correctly. |
| **Fairness** | Improved | Guarantees that even the lowest priority process will eventually get CPU time, satisfying the liveness property. |

### Conclusion
While the strict priority scheduler meets the functional requirements of favoring important tasks, it introduces the risk of starvation. The proposed **Aging** mechanism effectively balances this trade-off, converting the system from a strictly unfair hierarchy to a *mostly* hierarchical but ultimately fair scheduler, significantly improving system robustness for a minimal performance cost.
