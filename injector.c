#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    struct user_regs_struct regs;

    // Attach
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);

    // Wait for stop
    waitpid(pid, NULL, 0);

    printf("Attached to PID %d\n", pid);

    // Read registers
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);

    printf("Original RIP: %llx\n", regs.rip);

    // Modify RIP
    regs.rip += 2;

    // Write registers
    ptrace(PTRACE_SETREGS, pid, NULL, &regs);

    printf("Modified RIP: %llx\n", regs.rip);

    // Detach
    ptrace(PTRACE_DETACH, pid, NULL, NULL);

    printf("Detached\n");

    return 0;
}