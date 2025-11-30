#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void work(int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 1000000; j++) {
            asm volatile("nop");
        }
    }
}

int main(int argc, char *argv[]) {
    int pid1, pid2, pid3;

    printf("Starting basic priority test\n");

    pid1 = fork();
    if (pid1 == 0) {
        setpriority(getpid(), 5); // Low priority
        printf("Child 1 (low priority) starting\n");
        work(500);
        printf("Child 1 (low priority) finished\n");
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        setpriority(getpid(), 10); // Medium priority
        printf("Child 2 (medium priority) starting\n");
        work(500);
        printf("Child 2 (medium priority) finished\n");
        exit(0);
    }

    pid3 = fork();
    if (pid3 == 0) {
        setpriority(getpid(), 15); // High priority
        printf("Child 3 (high priority) starting\n");
        work(500);
        printf("Child 3 (high priority) finished\n");
        exit(0);
    }

    wait(0);
    wait(0);
    wait(0);

    printf("Basic priority test finished\n");
    exit(0);
}
