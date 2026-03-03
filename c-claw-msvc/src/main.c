#include "../include/claw.h"
#include "../include/platform.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (platform_init() != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    int rc = cli_main(argc, argv);
    platform_cleanup();
    return rc;
}
