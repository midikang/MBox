#include "../include/claw.h"
#include "../include/platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char *fs_get_home(void) {
    return platform_get_home();
}

char *fs_config_path(void) {
    char *home = fs_get_home();
    if (!home) return NULL;

    char *path = (char *)malloc(strlen(home) + 64);
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

    char *path = (char *)malloc(strlen(home) + 64);
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

    char *path = (char *)malloc(strlen(home) + 64);
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

    struct _stat st;
    if (_stat(path, &st) == 0) {
        return (st.st_mode & _S_IFDIR) ? 0 : -1;
    }

    if (_mkdir(path) == 0) {
        return 0;
    }

    return -1;
}
