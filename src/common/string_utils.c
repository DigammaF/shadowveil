
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "string_utils.h"

char** splitString(const char* str, unsigned* count) {
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

void freeSplit(char** result, unsigned count) {
    for (unsigned i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

char* joinString(char** str, const char* joiner) {
	if (!str || !joiner) {
		return NULL;
	}

	size_t joinerLen = strlen(joiner);
	size_t totalLen = 0;
	int count = 0;

	// Calculate the total length of the resulting string
	while (str[count]) {
		totalLen += strlen(str[count]);
		if (str[count + 1]) {
			totalLen += joinerLen;
		}
		count++;
	}

	char* result = malloc(totalLen + 1);
	if (!result) {
		perror("joinString: malloc");
		exit(EXIT_FAILURE);
	}

	result[0] = '\0';

	// Concatenate the strings with the joiner
	for (int i = 0; i < count; i++) {
		strcat(result, str[i]);
		if (i < count - 1) {
			strcat(result, joiner);
		}
	}

	return result;
}

void freeJoin(char* str) {
	free(str);
}
