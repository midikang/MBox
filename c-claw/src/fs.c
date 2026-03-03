#define _POSIX_C_SOURCE 200809L
#include "../include/claw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

char *fs_get_home(void) {
    const char *home = getenv("HOME");
    if (home) {
        return strdup(home);
    }

    struct passwd *pw = getpwuid(getuid());
    if (pw && pw->pw_dir) {
        return strdup(pw->pw_dir);
    }

    return NULL;
}

char *fs_config_path(void) {
    char *home = fs_get_home();
    if (!home) return NULL;

    char *path = malloc(strlen(home) + 64);
    if (!path) {
        free(home);
        return NULL;
    }

    sprintf(path, "%s/.cclaw/config.ini", home);
    free(home);

    return path;
}

char *fs_memory_path(void) {
    char *home = fs_get_home();
    if (!home) return NULL;

    char *path = malloc(strlen(home) + 64);
    if (!path) {
        free(home);
        return NULL;
    }

    sprintf(path, "%s/.cclaw/memory.log", home);
    free(home);

    return path;
}

char *fs_cron_path(void) {
    char *home = fs_get_home();
    if (!home) return NULL;

    char *path = malloc(strlen(home) + 64);
    if (!path) {
        free(home);
        return NULL;
    }

    sprintf(path, "%s/.cclaw/cron.txt", home);
    free(home);

    return path;
}

int fs_ensure_dir(const char *path) {
    if (!path) return -1;

    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode) ? 0 : -1;
    }

    if (mkdir(path, 0755) == 0) {
        return 0;
    }

    return -1;
}
