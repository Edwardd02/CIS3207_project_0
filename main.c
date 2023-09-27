#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void printDir(const char *path, int recursive) {
    DIR *dir;
    struct dirent *entry;
    struct dirent **namelist;
    int n;

    dir = opendir(path);
    if (!dir) {
        perror("tuls: cannot open directory");
        exit(1);
    }

    n = scandir(path, &namelist, NULL, alphasort);
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

        char subPath[4096];
        snprintf(subPath, sizeof(subPath), "%s/%s", path, entry->d_name);

        printf("%s\n", recursive ? subPath : entry->d_name);

        if (recursive && entry->d_type == DT_DIR) {
            printDir(subPath, 1);
        }

        free(namelist[i]);
    }

    free(namelist);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "tuls: too many arguments\n");
        exit(1);
    }

    const char *path = (argc > 1) ? argv[1] : ".";
    printDir(path, argc > 1);

    return 0;
}
