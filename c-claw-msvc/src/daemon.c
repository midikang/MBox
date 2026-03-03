#include "../include/claw.h"
#include "../include/platform.h"
#include <stdio.h>
#include <stdlib.h>

static void *gateway_thread(void *arg) {
    int port = *(int *)arg;
    gateway_start(port);
    return NULL;
}

int daemon_start(void) {
    Config cfg;
    if (config_load(&cfg) != 0) {
        fprintf(stderr, "Error loading configuration\n");
        return -1;
    }

    if (provider_init(&cfg.provider) != 0) {
        fprintf(stderr, "Error initializing provider\n");
        return -1;
    }

    pthread_t thread;
    if (pthread_create(&thread, NULL, gateway_thread, &cfg.gateway_port) != 0) {
        fprintf(stderr, "Error creating gateway thread\n");
        provider_cleanup();
        return -1;
    }

    printf("Daemon started: gateway on port %d, cron engine running\n", cfg.gateway_port);

    while (1) {
        sleep(1);
        cron_tick();
    }

    pthread_join(thread, NULL);
    provider_cleanup();
    return 0;
}
