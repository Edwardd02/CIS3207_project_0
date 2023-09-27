#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUF_SIZE 4096

void copy_file(const char* src, const char* dst) {
    int src_fd, dst_fd, n, err;
    unsigned char buffer[BUF_SIZE];

    src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("tucp: cannot open source file");
        exit(1);
    }

    dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dst_fd < 0) {
        perror("tucp: cannot open/create destination file");
        close(src_fd);
        exit(1);
    }

    while (1) {
        err = read(src_fd, buffer, BUF_SIZE);
        if (err == -1) {
            perror("tucp: read failed");
            close(src_fd);
            close(dst_fd);
            exit(1);
        }
        n = err;

        if (n == 0) break;

        err = write(dst_fd, buffer, n);
        if (err == -1) {
            perror("tucp: write failed");
            close(src_fd);
            close(dst_fd);
            exit(1);
        }
    }

    close(src_fd);
    close(dst_fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "tucp: missing file operand\n");
        exit(EXIT_FAILURE);
    }

    const char* dest = argv[argc - 1];
    struct stat st;

    if (stat(dest, &st) < 0) {
        if (argc == 3) { // Attempt to copy file if only source and destination are provided
            copy_file(argv[1], argv[2]);
            return 0;
        } else {
            fprintf(stderr, "tucp: destination does not exist\n");
            exit(EXIT_FAILURE);
        }
    }

    if (S_ISDIR(st.st_mode)) {
        for (int i = 1; i < argc - 1; ++i) {
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s/%s", dest, argv[i]);
            copy_file(argv[i], path);
        }
    } else {
        if (argc > 3) {
            fprintf(stderr, "tucp: extra operand '%s'\n", argv[3]);
            exit(EXIT_FAILURE);
        }
        copy_file(argv[1], argv[2]);
    }

    return 0;
}
