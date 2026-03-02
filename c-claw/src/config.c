#include "../include/claw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *trim(char *str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = 0;

    return str;
}

int config_load(Config *cfg) {
    if (!cfg) return -1;

    char *path = fs_config_path();
    if (!path) return -1;

    FILE *f = fopen(path, "r");
    free(path);

    if (!f) return -1;

    memset(cfg, 0, sizeof(Config));
    cfg->provider.temperature = 0.7f;
    cfg->gateway_port = 8080;
    cfg->max_history = 20;

    char line[512];
    char section[64] = "";

    while (fgets(line, sizeof(line), f)) {
        char *trimmed = trim(line);
        if (!*trimmed || *trimmed == '#' || *trimmed == ';') continue;

        if (*trimmed == '[') {
            char *end = strchr(trimmed, ']');
            if (end) {
                *end = 0;
                strncpy(section, trimmed + 1, sizeof(section) - 1);
            }
            continue;
        }

        char *eq = strchr(trimmed, '=');
        if (!eq) continue;

        *eq = 0;
        char *key = trim(trimmed);
        char *val = trim(eq + 1);

        if (strcmp(section, "provider") == 0) {
            if (strcmp(key, "base_url") == 0) {
                strncpy(cfg->provider.base_url, val, sizeof(cfg->provider.base_url) - 1);
            } else if (strcmp(key, "api_key") == 0) {
                strncpy(cfg->provider.api_key, val, sizeof(cfg->provider.api_key) - 1);
            } else if (strcmp(key, "model") == 0) {
                strncpy(cfg->provider.model, val, sizeof(cfg->provider.model) - 1);
            } else if (strcmp(key, "temperature") == 0) {
                cfg->provider.temperature = atof(val);
            }
        } else if (strcmp(section, "gateway") == 0) {
            if (strcmp(key, "port") == 0) {
                cfg->gateway_port = atoi(val);
            }
        } else if (strcmp(section, "general") == 0) {
            if (strcmp(key, "max_history") == 0) {
                cfg->max_history = atoi(val);
            }
        }
    }

    fclose(f);
    return 0;
}

int config_save(const Config *cfg) {
    if (!cfg) return -1;

    char *dir = fs_get_home();
    if (dir) {
        char path[512];
        snprintf(path, sizeof(path), "%s/.cclaw", dir);
        fs_ensure_dir(path);
        free(dir);
    }

    char *path = fs_config_path();
    if (!path) return -1;

    FILE *f = fopen(path, "w");
    free(path);

    if (!f) return -1;

    fprintf(f, "[provider]\n");
    fprintf(f, "base_url = %s\n", cfg->provider.base_url);
    fprintf(f, "api_key = %s\n", cfg->provider.api_key);
    fprintf(f, "model = %s\n", cfg->provider.model);
    fprintf(f, "temperature = %.1f\n", cfg->provider.temperature);
    fprintf(f, "\n");

    fprintf(f, "[gateway]\n");
    fprintf(f, "port = %d\n", cfg->gateway_port);
    fprintf(f, "\n");

    fprintf(f, "[general]\n");
    fprintf(f, "max_history = %d\n", cfg->max_history);

    fclose(f);
    return 0;
}
