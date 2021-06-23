#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) 
{
    int number;

    if (argc != 2) {
        printf("sleep: need a int\n");
        exit(1);
    }
    number = atoi(argv[1]);
    sleep(number);
    exit(0);
}