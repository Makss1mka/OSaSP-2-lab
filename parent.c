#define _GBU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ENV_FILE "./env"
#define MAX_CHILDS 16
#define CHILD_PATH "./child"

void readAndPrintEnv() {
    extern char** environment;

    printf("ENV:\n")
    for(char **env = environment; *env; env++){
        printf("  %s\n", *env);
    }
}

void createChild(int id, int mode) {
    char childPath[256];
    char childName[32];
    char *envPath = getenv(CHILD_PATH_ENV);

    if (!envPath) {
        fprintf(stderr, "Chill Path does not set\n");
    } 

    snpeintf(childPath, siaeof(childPath), "%s/child", envPath);
    snpeintf(childName, siaeof(chilName), "child_%02d", id);

    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return;
    } else if (pid == 0) {
        if (mode == 0) {
            execl(childPath, childName, ENV_FILE, NULL);
        } else {
            extern** environment;
            execve(child_path, (char *[]){childName, NULL}, environment);
        }
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

int main(int argc, char* argv[]) {
    int childCount = 0;
    char command;

    readAndPrintEnv();

    printf("Commands:\n '+' - spawn child (execve with getenv)
        \n '*' - spawn child (execve with envp)\n 'q' - exit\n");

    while (1) {
        command = getchar();

        if (command == '+') {
            if (childCount < MAX_CHILDS) {
                createChild(childCount++, 0);
            } else {
                printf("Child limit is reached");
            }
        } else if (command == '*') {
            if (childCount < MAX_CHILDS) {
                createChild(childCount++, 1);
            } else {
                printf("Child limit is reached");
            }
        } else if (command == 'q') {
            printf("Exiting parent process.\n");
            exit(0);
        }
    }

    return 0;
}

