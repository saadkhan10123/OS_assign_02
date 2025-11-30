#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void work(int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 100000; j++) {
            asm volatile("nop");
        }
    }
}

int main(int argc, char *argv[]) {
    int i, pid;
    int n = 20;

    printf("Starting stress test with %d processes\n", n);

    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {
            int prio = (i * 37) % 21; // Random-ish priority 0-20
            setpriority(getpid(), prio);
            // printf("Child %d (prio %d) starting\n", getpid(), prio);
            work(100);
            // printf("Child %d (prio %d) finished\n", getpid(), prio);
            exit(0);
        }
    }

    for (i = 0; i < n; i++) {
        wait(0);
    }

    printf("Stress test finished\n");
    exit(0);
}
