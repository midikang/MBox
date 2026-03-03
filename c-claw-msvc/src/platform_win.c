#include "../include/platform.h"
#include <stdio.h>

int platform_init(void) {
#ifdef _WIN32
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
#else
    return 0;
#endif
}

void platform_cleanup(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

char *platform_get_home(void) {
#ifdef _WIN32
    const char *p = getenv("USERPROFILE");
    if (p && *p) return _strdup(p);

    const char *d = getenv("HOMEDRIVE");
    const char *h = getenv("HOMEPATH");
    if (d && h) {
        size_t n = strlen(d) + strlen(h) + 1;
        char *buf = (char *)malloc(n);
        if (!buf) return NULL;
        snprintf(buf, n, "%s%s", d, h);
        return buf;
    }
    return _strdup("C:\\");
#else
    return NULL;
#endif
}
