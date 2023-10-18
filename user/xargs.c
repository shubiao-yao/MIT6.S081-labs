#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("xargs: The number of parameters must be >= 2!\n");
        exit(1);
    }
    if (argc > MAXARG) {
        printf("xargs: The number of args must be %d < !\n", MAXARG);
        exit(1);
    }

    char *x_args[MAXARG];
    for (int i = 1; i < argc; i++) {
        x_args[i - 1] = argv[i];
    }

    char buf[512];
    int i = 0;
    int index = argc - 1;
    int line_end = 0;
    while (read(0, &buf[i], sizeof(char))) { //example: echo world | xargs echo hello  
        if (buf[i] == '\n')
            line_end = 1;
        if (buf[i] == ' ' || buf[i] == '\n') {
            buf[i] = 0;          
            x_args[index] = buf;
            index++;
            i = 0;
        }
        i++;

        if (line_end) {
            line_end = 0;
            x_args[index] = 0;
            index = argc - 1;
            int pid = fork();
            if (pid < 0) {
                printf("fork error !\n");
                exit(1);
            } else if (pid == 0) {
                exec(x_args[0], x_args);
            } else {
                wait(0);
            }
        }
    }
    exit(0);
}