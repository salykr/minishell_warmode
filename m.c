#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Prepare arguments for execve
    char *args[] = {"/bin/bash", "-c", "declare -p", NULL};  // The command to run in bash
    char *envp[] = {NULL};  // Use the current environment (you can pass specific env variables here)

    // Execute the command using execve
    if (execve("/bin/bash", args, envp) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }

    // Should never reach here if execve is successful
    return 0;
}
