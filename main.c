#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

// A function to print the contents of a directory, non-recursively.
void printDir(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct dirent **namelist;
    int n;

    dir = opendir(path);
    if (!dir) {
        perror("tuls: cannot open directory");
        exit(1);
    }

    n = scandir(path, &namelist, NULL, alphasort); // corrected the filter function to NULL
    if (n == -1) {
        perror("tuls: cannot scan directory");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        entry = namelist[i];

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            free(namelist[i]);
            continue;
        }

        printf("%s\n", entry->d_name);
        free(namelist[i]);
    }

    free(namelist);
    closedir(dir);
}

// A function to recursively print the contents of a directory.
void recursivePrint(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct dirent **namelist;
    int n;

    dir = opendir(path);
    if (!dir) {
        perror("tuls: cannot open directory");
        exit(1);
    }

    n = scandir(path, &namelist, NULL, alphasort); // corrected the filter function to NULL
    if (n == -1) {
        perror("tuls: cannot scan directory");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        entry = namelist[i];

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            free(namelist[i]);
            continue;
        }

        char subPath[4096]; // You may need to adjust the size
        snprintf(subPath, sizeof(subPath), "%s/%s", path, entry->d_name);

        printf("%s\n", subPath);

        if (entry->d_type == DT_DIR) {
            recursivePrint(subPath);
        }

        free(namelist[i]);
    }

    free(namelist);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path;

    if (argc > 2) {
        fprintf(stderr, "tuls: too many arguments\n");
        exit(1);
    }

    path = (argc > 1) ? argv[1] : ".";

    if (argc > 1) {
        recursivePrint(path);
    } else {
        printDir(path);
    }

    return 0;
}
