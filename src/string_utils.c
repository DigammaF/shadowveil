
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "string_utils.h"

char** splitString(const char* str, int* count) {
    if (!str) {
        *count = 0;
        return NULL;
    }

    int capacity = 10;
    char** result = malloc(capacity * sizeof(char*));
    if (!result) {
        perror("splitString: malloc");
        exit(EXIT_FAILURE);
    }

    char* strCopy = strdup(str);
    if (!strCopy) {
        perror("splitString: initial strdup");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(strCopy, " ");
    int index = 0;

    while (token) {
        if (index >= capacity) {
            capacity *= 2;
            result = realloc(result, capacity * sizeof(char*));
            if (!result) {
                perror("splitString: realloc");
                exit(EXIT_FAILURE);
            }
        }

        result[index++] = strdup(token);

        if (!result[index - 1]) {
            perror("splitString: strdup");
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL, " ");
    }

    result[index] = NULL;
    *count = index;
    free(strCopy);
    return result;
}

void freeSplit(char** result, int count) {
    for (int i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}
