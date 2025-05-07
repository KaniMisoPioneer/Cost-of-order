#include <stdio.h>
#include <stdlib.h>
#include "input.h"

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    char *buffer = NULL;

    if (file) {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        rewind(file);

        buffer = (char *)malloc(size + 1);
        if (buffer) {
            fread(buffer, 1, size, file);
            buffer[size] = '\0';
        }
        fclose(file);
    }

    return buffer;
}