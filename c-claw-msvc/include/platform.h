#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef HANDLE pthread_t;

typedef struct thread_start_ctx {
    void *(*fn)(void *);
    void *arg;
} thread_start_ctx;

static unsigned __stdcall platform_thread_start(void *p) {
    thread_start_ctx *ctx = (thread_start_ctx *)p;
    void *(*fn)(void *) = ctx->fn;
    void *arg = ctx->arg;
    free(ctx);
    (void)fn(arg);
    return 0;
}

static inline int pthread_create(pthread_t *thread, void *attr, void *(*start_routine)(void *), void *arg) {
    (void)attr;
    thread_start_ctx *ctx = (thread_start_ctx *)malloc(sizeof(thread_start_ctx));
    if (!ctx) return -1;
    ctx->fn = start_routine;
    ctx->arg = arg;

    uintptr_t h = _beginthreadex(NULL, 0, platform_thread_start, ctx, 0, NULL);
    if (h == 0) {
        free(ctx);
        return -1;
    }
    *thread = (HANDLE)h;
    return 0;
}

static inline int pthread_join(pthread_t thread, void **retval) {
    (void)retval;
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    return 0;
}

#ifndef strdup
#define strdup _strdup
#endif

#ifndef strtok_r
#define strtok_r(str, delim, saveptr) strtok_s((str), (delim), (saveptr))
#endif

#ifndef sleep
#define sleep(sec) Sleep((DWORD)((sec) * 1000))
#endif

int platform_init(void);
void platform_cleanup(void);
char *platform_get_home(void);

#endif /* _WIN32 */
#endif /* PLATFORM_H */
