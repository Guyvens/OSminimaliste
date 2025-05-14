#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
    char command[256];

    while (1) {
        write(1, "# ", 2);

        int count = read(0, command, sizeof(command) - 1);
        if (count <= 1) {
            continue;
        }

        command[count - 1] = '\0';  // Remove newline

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // Enfant
            char *argv[] = {command, NULL};
            char *envp[] = {NULL};

            execve(command, argv, envp);

            // Si execve échoue
            perror("execve");
            _exit(1);
        } else {
            // Parent
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
