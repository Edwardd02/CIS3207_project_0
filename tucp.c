#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Function to copy file
void copyFile(const char *sourcePath, const char *destPath) {
    FILE *sourceFile, *destFile;
    char ch;

    sourceFile = fopen(sourcePath, "r");
    if (sourceFile == NULL) {
        perror("tucp: cannot open source file");
        exit(EXIT_FAILURE);
    }

    destFile = fopen(destPath, "w");
    if (destFile == NULL) {
        perror("tucp: cannot open destination file");
        fclose(sourceFile);
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(sourceFile)) != EOF)
        fputc(ch, destFile);

    fclose(sourceFile);
    fclose(destFile);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "tucp: missing file operand\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 3) {
        // tucp Sourcefile Destinationfile or tucp Sourcefile Directory
        copyFile(argv[1], argv[2]);
    } else {
        // tucp Sourcefile-1 Sourcefile-2 Sourcefile-3 Sourcefile-n Directory
        for (int i = 1; i < argc - 1; i++) {
            char destPath[4096];
            snprintf(destPath, sizeof(destPath), "%s/%s", argv[argc - 1], strrchr(argv[i], '/') + 1);
            copyFile(argv[i], destPath);
        }
    }

    return 0;
}
