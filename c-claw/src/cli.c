#define _POSIX_C_SOURCE 200809L
#include "../include/claw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void print_usage(const char *prog) {
    printf("Usage: %s <command> [options]\n\n", prog);
    printf("Commands:\n");
    printf("  onboard                    Initialize configuration\n");
    printf("  chat <prompt>              Interactive chat (with history)\n");
    printf("  ask <prompt>               Single question (no history)\n");
    printf("  gateway [port]             Start HTTP gateway (default: 8080)\n");
    printf("  cron add <interval> <prompt>  Add scheduled task\n");
    printf("  daemon                     Run background service\n");
    printf("\nExamples:\n");
    printf("  %s onboard\n", prog);
    printf("  %s chat \"Hello, how are you?\"\n", prog);
    printf("  %s ask \"What is 2+2?\"\n", prog);
    printf("  %s gateway 9000\n", prog);
    printf("  %s cron add 3600 \"Daily summary\"\n", prog);
    printf("  %s daemon\n", prog);
}

int cli_main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *cmd = argv[1];

    if (strcmp(cmd, "onboard") == 0) {
        return cmd_onboard();
    } else if (strcmp(cmd, "chat") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Error: chat requires a prompt\n");
            return 1;
        }
        return cmd_chat(argv[2]);
    } else if (strcmp(cmd, "ask") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Error: ask requires a prompt\n");
            return 1;
        }
        return cmd_ask(argv[2]);
    } else if (strcmp(cmd, "gateway") == 0) {
        int port = 8080;
        if (argc >= 3) {
            port = atoi(argv[2]);
            if (port <= 0 || port > 65535) {
                fprintf(stderr, "Error: invalid port number\n");
                return 1;
            }
        }
        return cmd_gateway(port);
    } else if (strcmp(cmd, "cron") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Error: cron requires a subcommand\n");
            return 1;
        }
        if (strcmp(argv[2], "add") == 0) {
            if (argc < 5) {
                fprintf(stderr, "Error: cron add requires <interval> <prompt>\n");
                return 1;
            }
            int interval = atoi(argv[3]);
            if (interval <= 0) {
                fprintf(stderr, "Error: invalid interval\n");
                return 1;
            }
            return cmd_cron_add(interval, argv[4]);
        } else {
            fprintf(stderr, "Error: unknown cron subcommand\n");
            return 1;
        }
    } else if (strcmp(cmd, "daemon") == 0) {
        return cmd_daemon();
    } else {
        fprintf(stderr, "Error: unknown command '%s'\n", cmd);
        print_usage(argv[0]);
        return 1;
    }
}

int cmd_onboard(void) {
    Config cfg = {0};

    printf("=== c-claw Onboarding ===\n\n");

    printf("Enter API base URL (e.g., https://api.openai.com/v1): ");
    if (!fgets(cfg.provider.base_url, sizeof(cfg.provider.base_url), stdin)) {
        fprintf(stderr, "Error reading base URL\n");
        return 1;
    }
    cfg.provider.base_url[strcspn(cfg.provider.base_url, "\n")] = 0;

    printf("Enter API key: ");
    if (!fgets(cfg.provider.api_key, sizeof(cfg.provider.api_key), stdin)) {
        fprintf(stderr, "Error reading API key\n");
        return 1;
    }
    cfg.provider.api_key[strcspn(cfg.provider.api_key, "\n")] = 0;

    printf("Enter model name (e.g., gpt-3.5-turbo): ");
    if (!fgets(cfg.provider.model, sizeof(cfg.provider.model), stdin)) {
        fprintf(stderr, "Error reading model name\n");
        return 1;
    }
    cfg.provider.model[strcspn(cfg.provider.model, "\n")] = 0;

    cfg.provider.temperature = 0.7f;
    cfg.gateway_port = 8080;
    cfg.max_history = 20;

    if (config_save(&cfg) != 0) {
        fprintf(stderr, "Error saving configuration\n");
        return 1;
    }

    printf("\nConfiguration saved successfully!\n");
    printf("Config location: %s\n", fs_config_path());
    return 0;
}

int cmd_chat(const char *prompt) {
    Config cfg;
    if (config_load(&cfg) != 0) {
        fprintf(stderr, "Error: No configuration found. Run 'onboard' first.\n");
        return 1;
    }

    if (provider_init(&cfg.provider) != 0) {
        fprintf(stderr, "Error initializing provider\n");
        return 1;
    }

    int msg_count = 0;
    Message *history = memory_load(&msg_count);

    int total = msg_count + 1;
    if (total > cfg.max_history * 2) {
        total = cfg.max_history * 2;
    }

    Message *msgs = malloc(sizeof(Message) * (total + 1));
    if (!msgs) {
        fprintf(stderr, "Memory allocation failed\n");
        if (history) memory_free(history, msg_count);
        provider_cleanup();
        return 1;
    }

    int start = msg_count > total - 1 ? msg_count - total + 1 : 0;
    int copied = 0;
    for (int i = start; i < msg_count; i++) {
        msgs[copied] = history[i];
        msgs[copied].content = strdup(history[i].content);
        copied++;
    }

    strcpy(msgs[copied].role, "user");
    msgs[copied].content = strdup(prompt);
    copied++;

    char *response = provider_chat(msgs, copied);

    for (int i = 0; i < copied - 1; i++) {
        free(msgs[i].content);
    }
    free(msgs[copied - 1].content);
    free(msgs);

    if (history) memory_free(history, msg_count);

    if (!response) {
        fprintf(stderr, "Error getting response from provider\n");
        provider_cleanup();
        return 1;
    }

    printf("%s\n", response);

    memory_append("user", prompt);
    memory_append("assistant", response);

    free(response);
    provider_cleanup();
    return 0;
}

int cmd_ask(const char *prompt) {
    Config cfg;
    if (config_load(&cfg) != 0) {
        fprintf(stderr, "Error: No configuration found. Run 'onboard' first.\n");
        return 1;
    }

    if (provider_init(&cfg.provider) != 0) {
        fprintf(stderr, "Error initializing provider\n");
        return 1;
    }

    Message msg;
    strcpy(msg.role, "user");
    msg.content = strdup(prompt);

    char *response = provider_chat(&msg, 1);
    free(msg.content);

    if (!response) {
        fprintf(stderr, "Error getting response from provider\n");
        provider_cleanup();
        return 1;
    }

    printf("%s\n", response);
    free(response);
    provider_cleanup();
    return 0;
}

int cmd_gateway(int port) {
    Config cfg;
    if (config_load(&cfg) != 0) {
        fprintf(stderr, "Error: No configuration found. Run 'onboard' first.\n");
        return 1;
    }

    if (provider_init(&cfg.provider) != 0) {
        fprintf(stderr, "Error initializing provider\n");
        return 1;
    }

    printf("Starting HTTP gateway on port %d...\n", port);
    printf("Press Ctrl+C to stop\n");

    int ret = gateway_start(port);

    provider_cleanup();
    return ret;
}

int cmd_cron_add(int interval, const char *prompt) {
    if (cron_add(interval, prompt) != 0) {
        fprintf(stderr, "Error adding cron task\n");
        return 1;
    }

    printf("Cron task added: every %d seconds\n", interval);
    printf("Prompt: %s\n", prompt);
    return 0;
}

int cmd_daemon(void) {
    Config cfg;
    if (config_load(&cfg) != 0) {
        fprintf(stderr, "Error: No configuration found. Run 'onboard' first.\n");
        return 1;
    }

    printf("Starting daemon service...\n");
    printf("Gateway port: %d\n", cfg.gateway_port);
    printf("Press Ctrl+C to stop\n");

    return daemon_start();
}
