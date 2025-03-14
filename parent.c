#define _GBU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ENV_FILE "env"
#define MAX_CHILDS 16
#define CHILD_PATH "CHILD_PATH"

void readAndPrintEnv() {
    extern char** environ;

    printf("ENV:\n");
    for(char **env = environ; *env; env++){
        printf("  %s\n", *env);
    }
}

void createChild(int id, int mode) {
    char childPath[256];
    char childName[32];
    char *envPath = getenv(CHILD_PATH);

    if (!envPath) {
        fprintf(stderr, "Child Path does not set\n");
    } 

    snprintf(childPath, sizeof(childPath), "%s/child", envPath);
    snprintf(childName, sizeof(childName), "child_%02d", id);

    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return;
    } else if (pid == 0) {
        if (mode == 0) {
            execl(childPath, childName, ENV_FILE, NULL);
        } else {
            extern char** environ;
            execve(childPath, (char *[]){childName, NULL}, environ);
        }
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

int main(int argc, char* argv[]) {
    char command;
    int childCount = 0;

    readAndPrintEnv();

    printf("Commands:\n '+' - spawn child (execve with getenv)\n '*' - spawn child (execve with envp)\n 'q' - exit\n");

    while (1) {
        command = getchar();
        if (command == 'q') {
            printf("Exiting parent process...\n");
            break;
        } else if (command == '+') {
            if (childCount < MAX_CHILDS) {
                createChild(childCount++, 0);
            } else {
                printf("Max child processes reached.\n");
            }
        } else if (command == '*') {
            if (childCount < MAX_CHILDS) {
                createChild(childCount++, 1);
            } else {
                printf("Max child processes reached.\n");
            }
        }
    }
    
    return 0;
}

