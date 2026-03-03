#define _POSIX_C_SOURCE 200809L
#include "../include/claw.h"
#include "../include/platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cron_load(CronTask **tasks, int *count) {
    if (!tasks || !count) return -1;

    *tasks = NULL;
    *count = 0;

    char *path = fs_cron_path();
    if (!path) return -1;

    FILE *f = fopen(path, "r");
    free(path);

    if (!f) return 0;

    int capacity = 0;
    char line[1024];

    while (fgets(line, sizeof(line), f)) {
        if (*count >= capacity) {
            capacity = capacity == 0 ? 8 : capacity * 2;
            CronTask *new_tasks = realloc(*tasks, capacity * sizeof(CronTask));
            if (!new_tasks) {
                if (*tasks) free(*tasks);
                fclose(f);
                return -1;
            }
            *tasks = new_tasks;
        }

        char *saveptr;
        char *id_str = strtok_r(line, "|", &saveptr);
        char *interval_str = strtok_r(NULL, "|", &saveptr);
        char *next_run_str = strtok_r(NULL, "|", &saveptr);
        char *enabled_str = strtok_r(NULL, "|", &saveptr);
        char *prompt = strtok_r(NULL, "|", &saveptr);

        if (!id_str || !interval_str || !next_run_str || !enabled_str || !prompt) {
            continue;
        }

        (*tasks)[*count].id = atoi(id_str);
        (*tasks)[*count].interval_sec = atoi(interval_str);
        (*tasks)[*count].next_run = (time_t)atoll(next_run_str);
        (*tasks)[*count].enabled = atoi(enabled_str);

        size_t plen = strlen(prompt);
        if (plen > 0 && prompt[plen - 1] == '\n') {
            prompt[plen - 1] = 0;
        }

        strncpy((*tasks)[*count].prompt, prompt, sizeof((*tasks)[*count].prompt) - 1);
        (*tasks)[*count].prompt[sizeof((*tasks)[*count].prompt) - 1] = 0;

        (*count)++;
    }

    fclose(f);
    return 0;
}

int cron_save(CronTask *tasks, int count) {
    if (!tasks && count > 0) return -1;

    char *path = fs_cron_path();
    if (!path) return -1;

    FILE *f = fopen(path, "w");
    free(path);

    if (!f) return -1;

    for (int i = 0; i < count; i++) {
        fprintf(f, "%d|%d|%ld|%d|%s\n",
                tasks[i].id,
                tasks[i].interval_sec,
                (long)tasks[i].next_run,
                tasks[i].enabled,
                tasks[i].prompt);
    }

    fclose(f);
    return 0;
}

int cron_add(int interval, const char *prompt) {
    if (interval <= 0 || !prompt) return -1;

    CronTask *tasks = NULL;
    int count = 0;

    cron_load(&tasks, &count);

    int max_id = 0;
    for (int i = 0; i < count; i++) {
        if (tasks[i].id > max_id) {
            max_id = tasks[i].id;
        }
    }

    CronTask *new_tasks = realloc(tasks, (count + 1) * sizeof(CronTask));
    if (!new_tasks) {
        if (tasks) free(tasks);
        return -1;
    }
    tasks = new_tasks;

    tasks[count].id = max_id + 1;
    tasks[count].interval_sec = interval;
    tasks[count].next_run = time(NULL) + interval;
    tasks[count].enabled = 1;
    strncpy(tasks[count].prompt, prompt, sizeof(tasks[count].prompt) - 1);
    tasks[count].prompt[sizeof(tasks[count].prompt) - 1] = 0;

    int ret = cron_save(tasks, count + 1);
    free(tasks);

    return ret;
}

void cron_tick(void) {
    CronTask *tasks = NULL;
    int count = 0;

    if (cron_load(&tasks, &count) != 0) {
        return;
    }

    time_t now = time(NULL);
    int changed = 0;

    Config cfg;
    int has_config = (config_load(&cfg) == 0);

    for (int i = 0; i < count; i++) {
        if (!tasks[i].enabled) continue;
        if (tasks[i].next_run > now) continue;

        if (has_config) {
            if (provider_init(&cfg.provider) == 0) {
                Message msg;
                strcpy(msg.role, "user");
                msg.content = tasks[i].prompt;

                char *response = provider_chat(&msg, 1);
                if (response) {
                    printf("[Cron Task %d] %s\n", tasks[i].id, response);
                    memory_append("user", tasks[i].prompt);
                    memory_append("assistant", response);
                    free(response);
                }

                provider_cleanup();
            }
        }

        tasks[i].next_run = now + tasks[i].interval_sec;
        changed = 1;
    }

    if (changed) {
        cron_save(tasks, count);
    }

    if (tasks) free(tasks);
}

