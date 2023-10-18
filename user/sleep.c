#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        fprintf(stderr, "usage: sleep TIME\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}
