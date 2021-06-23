#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int get_line(char *buf, int len) {
    char read_from;
    int index = 0;
    do {
        int c = read(0, &read_from, 1);
        if (c == 0) {
            return 0;  // 读取结束
        }
        buf[index++] = read_from;
        if (index == len - 1) {
            printf("The arg string is too long");
            exit(1);
        }
    } while (read_from != '\n');
    buf[--index] = '\0';
    return index;  // 返回一行
}

int get_command_argv(char *command_argv[], char *buf, int base, int line_len)
{
    int index = base;
    command_argv[index++] = &buf[0];
    for (int i = 1; i < line_len; i++) {
        if (buf[i] == ' ') {
            buf[i] = '\0';
            command_argv[index++] = &buf[++i];
        }
    }
    return index;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("xargs: need a command");
        exit(1);
    }
    char *command = argv[1];
    char *command_argv[MAXARG];
    command_argv[0] = argv[1];
    int base = 1;
    for (int i = 2; i < argc; i++) {
        command_argv[base++] = argv[i];
    }

    for (;;) {
        char buf[512];
        int line_len = get_line(buf, 512);
        if (line_len == 0) {
            break;
        }
        int command_num = get_command_argv(command_argv, buf, base, line_len);
        int child = fork();
        if (child == 0) {
            command_argv[command_num] = 0;
            exec(command, command_argv);
        } else {
            wait(&child);
        }
    }
    exit(0);
}