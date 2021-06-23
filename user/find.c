#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find_file(const char *dir, const char *file) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(dir, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", dir);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", dir);
        close(fd);
        return;
    }
    if (st.type == T_FILE) {
        printf("command: find dir file\n");
        exit(1);
    }
    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        exit(1);
    }
    strcpy(buf, dir);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_FILE) {
            if (strcmp(de.name, file) == 0) {
                printf("%s\n", buf);
            }
        } else if (st.type == T_DIR) {
            find_file(buf, file);
        }
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        printf("command: find dir file\n");
        exit(1);
    }

    find_file(argv[1], argv[2]);
    exit(0);
}