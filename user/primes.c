#include "kernel/types.h"
#include "user/user.h"


int prime(int *p);

int main() {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        printf("pipe error\n");
        return -1;
    }
    for(int i = 2; i <= 35; i++) {
        write(pipefd[1], &i, sizeof(i));
    }

    int pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        return -1;
    }

    if (pid > 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        wait(0);
        exit(0);
    } else {
        close(pipefd[1]);
        prime(pipefd);
        close(pipefd[0]);
        exit(0);
    }

    return 0;
}

int prime(int *p) {
    int prime_num;
    if (read(p[0], &prime_num, sizeof(prime_num)) == 0) {
        close(p[0]);
        exit(0);
    } else {
        printf("prime %d\n", prime_num);
    }

    int num;
    int p_next[2];
    pipe(p_next);
    int pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        close(p[0]);
        close(p_next[0]);
        close(p_next[1]);
        exit(1);
    }

    if (pid > 0) {
        close(p_next[0]);
        while (read(p[0], &num, sizeof(num))) {
            if (num % prime_num != 0) {
                write(p_next[1], &num, sizeof(num));
            }
        }
        close(p[0]);
        close(p_next[1]);
        wait(0);
        exit(0);
    } else {
        close(p[0]);
        close(p_next[1]);
        prime(p_next);
        close(p_next[0]);
        exit(0);
    }
    return 0;
}