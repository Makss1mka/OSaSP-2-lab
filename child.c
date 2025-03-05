#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void readEnvFile(const char *filename) {
    FILE* file = open(filename, "r");
    if (!file) {
        perror("Failed to open env file");
        return;
    }

    char var[128];
    while (fgets(var, sizeof(var), file)) {
        var[strcspn(var, "\n")] = '\0';
        char *value = getenv(var);

        printf("%s=%s\n", var, value ? value : "(null)");
    }

    fclose(file);
}

void printEnvVars(char* envp[]) {
    for (char** env = envp; *env; env++) {
        printf("%s\n", *env);
    }
}

int main(int argc, char* argv[]) {
    printf("Child process: Name=%s, PID=%d, PPID=%d\n", argv[0], getpid(), getppid());

    if (argc > 1) {
        printf("Reading environment from file: %s\n", argv[1]);
        readEnvFile(argv[1]);
    } else {
        printf("Reading environment from envp[]\n");
        readEnvFile(envp);
    }

    return 0;
}
