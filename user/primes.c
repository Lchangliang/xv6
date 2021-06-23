#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
prime_fork(int *p) {
    close(p[1]);
    int p1[2];
    pipe(p1);
    int first = 0;
    int read_from;
    int base;
    int child;
    while (read(p[0], &read_from, 4) != 0) {
        if (first == 0) {
            first = 1;
            base = read_from;
            child = fork();
            if (child == 0) {
                prime_fork(p1);
            } else {
                close(p1[0]);
            }
            printf("prime %d\n", base);
        } else {
            if (read_from % base != 0) {
                write(p1[1], &read_from, 4);
            }
        }
    }
    close(p[0]);
    close(p1[1]);
    wait(&child);
    exit(0);
}

int 
main(int argc, char *argv[]) 
{
    int base = 2;
    printf("prime 2\n");
    int p[2];
    pipe(p);
    int child = fork();
    if (child == 0) { // child
        prime_fork(p);
    }
    close(p[0]);
    for (int i = 3; i <= 35; i++) {
        if (i % base != 0) {
            write(p[1], &i, 4);
        }
    }
    close(p[1]);
    wait(&child);
    exit(0);
}