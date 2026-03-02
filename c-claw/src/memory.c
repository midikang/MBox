#define _POSIX_C_SOURCE 200809L
#include "../include/claw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int memory_append(const char *role, const char *content) {
    if (!role || !content) return -1;

    char *path = fs_memory_path();
    if (!path) return -1;

    FILE *f = fopen(path, "a");
    free(path);

    if (!f) return -1;

    time_t now = time(NULL);
    fprintf(f, "[%ld] %s: %s\n", now, role, content);

    fclose(f);
    return 0;
}

Message *memory_load(int *count) {
    if (!count) return NULL;

    *count = 0;

    char *path = fs_memory_path();
    if (!path) return NULL;

    FILE *f = fopen(path, "r");
    free(path);

    if (!f) return NULL;

    Message *msgs = NULL;
    int capacity = 0;
    char line[4096];

    while (fgets(line, sizeof(line), f)) {
        if (*count >= capacity) {
            capacity = capacity == 0 ? 16 : capacity * 2;
            Message *new_msgs = realloc(msgs, capacity * sizeof(Message));
            if (!new_msgs) {
                if (msgs) {
                    for (int i = 0; i < *count; i++) {
                        free(msgs[i].content);
                    }
                    free(msgs);
                }
                fclose(f);
                return NULL;
            }
            msgs = new_msgs;
        }

        char *bracket = strchr(line, ']');
        if (!bracket) continue;

        char *role_start = bracket + 1;
        while (*role_start == ' ') role_start++;

        char *colon = strchr(role_start, ':');
        if (!colon) continue;

        size_t role_len = colon - role_start;
        if (role_len >= sizeof(msgs[*count].role)) continue;

        strncpy(msgs[*count].role, role_start, role_len);
        msgs[*count].role[role_len] = 0;

        char *content_start = colon + 1;
        while (*content_start == ' ') content_start++;

        size_t content_len = strlen(content_start);
        if (content_len > 0 && content_start[content_len - 1] == '\n') {
            content_start[content_len - 1] = 0;
        }

        msgs[*count].content = strdup(content_start);
        (*count)++;
    }

    fclose(f);
    return msgs;
}

void memory_free(Message *msgs, int count) {
    if (!msgs) return;

    for (int i = 0; i < count; i++) {
        if (msgs[i].content) {
            free(msgs[i].content);
        }
    }
    free(msgs);
}
