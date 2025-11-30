#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int ret;

    printf("Starting edge case test\n");

    // Test invalid priority < 0
    ret = setpriority(getpid(), -1);
    if (ret == -1) {
        printf("Test passed: setpriority(-1) returned -1\n");
    } else {
        printf("Test failed: setpriority(-1) returned %d\n", ret);
    }

    // Test invalid priority > 20
    ret = setpriority(getpid(), 21);
    if (ret == -1) {
        printf("Test passed: setpriority(21) returned -1\n");
    } else {
        printf("Test failed: setpriority(21) returned %d\n", ret);
    }

    // Test valid priority 0
    ret = setpriority(getpid(), 0);
    if (ret == 0) {
        printf("Test passed: setpriority(0) returned 0\n");
    } else {
        printf("Test failed: setpriority(0) returned %d\n", ret);
    }

    // Test valid priority 20
    ret = setpriority(getpid(), 20);
    if (ret == 0) {
        printf("Test passed: setpriority(20) returned 0\n");
    } else {
        printf("Test failed: setpriority(20) returned %d\n", ret);
    }

    // Test setting priority for non-existent PID (not easily testable with current API as setpriority takes pid but my impl uses loop, wait, my impl takes pid)
    // Actually my impl iterates all procs. If I pass a random large PID it should fail.
    ret = setpriority(99999, 10);
    if (ret == -1) {
        printf("Test passed: setpriority(99999) returned -1\n");
    } else {
        printf("Test failed: setpriority(99999) returned %d\n", ret);
    }

    printf("Edge case test finished\n");
    exit(0);
}
