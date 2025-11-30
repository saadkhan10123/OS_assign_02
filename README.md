# xv6 Priority Scheduler Assignment

This repository contains a modified version of the xv6-riscv kernel with a priority-based scheduler implementation.

## Prerequisites

To build and run this project, you need a Linux environment (Ubuntu recommended) with the following tools installed:
-   `git`
-   `build-essential` (make, gcc, etc.)
-   `gdb-multiarch` (for debugging)
-   `qemu-system-misc` (QEMU emulator for RISC-V)
-   `gcc-riscv64-linux-gnu` (C compiler for RISC-V)
-   `binutils-riscv64-linux-gnu` (Linker and assembler for RISC-V)

## Installation

1.  **Install Dependencies**:
    Run the following command in your terminal to install all necessary tools:
    ```bash
    sudo apt-get update && sudo apt-get install -y git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
    ```

2.  **Clone the Repository** (if you haven't already):
    ```bash
    git clone https://github.com/saadkhan10123/OS_assign_02.git
    cd OS_assign_02
    ```

## Building and Running

1.  **Build the Kernel**:
    Compile the kernel and user programs using `make`:
    ```bash
    make
    ```

2.  **Run in QEMU**:
    Start the xv6 operating system in the QEMU emulator:
    ```bash
    make qemu
    ```
    You should see the xv6 boot messages and eventually a shell prompt (`$`).

3.  **Exiting QEMU**:
    To exit QEMU, press `Ctrl-A` then release and press `x`.

## Running Tests

We have included three test programs to verify the priority scheduler:

1.  **`test_basic`**:
    -   **Description**: Forks 3 processes with Low (5), Medium (10), and High (15) priorities.
    -   **Expected Output**: The High priority process should finish first, followed by Medium, then Low.
    -   **Command**:
        ```bash
        $ test_basic
        ```

2.  **`test_stress`**:
    -   **Description**: Creates 20 processes with random priorities to ensure system stability.
    -   **Command**:
        ```bash
        $ test_stress
        ```

3.  **`test_edge`**:
    -   **Description**: Tests invalid inputs for the `setpriority` system call (e.g., negative values).
    -   **Command**:
        ```bash
        $ test_edge
        ```

## Feature Details

-   **Scheduler**: Replaced Round-Robin with a Priority-Based Scheduler (Highest Priority First).
-   **Priority Range**: 0 (Lowest) to 20 (Highest). Default is 10.
-   **System Call**: Added `setpriority(int pid, int priority)` to modify process priority.

For a detailed implementation report, please refer to [unified_report.md](unified_report.md).
