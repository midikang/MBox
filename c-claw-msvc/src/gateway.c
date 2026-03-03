#include "../include/claw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <signal.h>

static volatile int g_running = 0;

static void signal_handler(int sig) {
    (void)sig;
    g_running = 0;
}

static char *read_request(SOCKET client_fd) {
    char *buffer = (char *)malloc(8192);
    if (!buffer) return NULL;

    int total = 0;
    int n = 0;

    while (total < 8191) {
        n = recv(client_fd, buffer + total, 8191 - total, 0);
        if (n <= 0) break;
        total += n;

        if (total >= 4 && strstr(buffer, "\r\n\r\n")) {
            const char *cl = strstr(buffer, "Content-Length:");
            if (cl) {
                int content_len = atoi(cl + 15);
                const char *body = strstr(buffer, "\r\n\r\n");
                if (body) {
                    body += 4;
                    int body_received = total - (int)(body - buffer);
                    while (body_received < content_len) {
                        n = recv(client_fd, buffer + total, 8191 - total, 0);
                        if (n <= 0) break;
                        total += n;
                        body_received += n;
                    }
                }
            }
            break;
        }
    }

    buffer[total] = 0;
    return buffer;
}

static void send_response(SOCKET client_fd, const char *status, const char *content_type, const char *body) {
    char header[512];
    int body_len = body ? (int)strlen(body) : 0;

    snprintf(header, sizeof(header),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n",
             status, content_type, body_len);

    send(client_fd, header, (int)strlen(header), 0);
    if (body) {
        send(client_fd, body, body_len, 0);
    }
}

static void handle_health(SOCKET client_fd) {
    const char *response = "{\"status\":\"ok\"}";
    send_response(client_fd, "200 OK", "application/json", response);
}

static void handle_chat(SOCKET client_fd, const char *request) {
    const char *body = strstr(request, "\r\n\r\n");
    if (!body) {
        send_response(client_fd, "400 Bad Request", "text/plain", "Missing request body");
        return;
    }
    body += 4;

    char *prompt = str_extract_field(body, "prompt");
    if (!prompt) {
        send_response(client_fd, "400 Bad Request", "text/plain", "Missing prompt field");
        return;
    }

    Config cfg;
    if (config_load(&cfg) != 0) {
        free(prompt);
        send_response(client_fd, "500 Internal Server Error", "text/plain", "Configuration error");
        return;
    }

    Message msg;
    strcpy(msg.role, "user");
    msg.content = prompt;

    char *reply = provider_chat(&msg, 1);
    free(prompt);

    if (!reply) {
        send_response(client_fd, "500 Internal Server Error", "text/plain", "Provider error");
        return;
    }

    char *escaped = str_escape_json(reply);
    free(reply);

    if (!escaped) {
        send_response(client_fd, "500 Internal Server Error", "text/plain", "Memory error");
        return;
    }

    char response[4096];
    snprintf(response, sizeof(response), "{\"response\":\"%s\"}", escaped);
    free(escaped);

    send_response(client_fd, "200 OK", "application/json", response);
}

static void handle_client(SOCKET client_fd) {
    char *request = read_request(client_fd);
    if (!request) {
        closesocket(client_fd);
        return;
    }

    if (strncmp(request, "GET /health", 11) == 0) {
        handle_health(client_fd);
    } else if (strncmp(request, "POST /chat", 10) == 0) {
        handle_chat(client_fd, request);
    } else {
        send_response(client_fd, "404 Not Found", "text/plain", "Not Found");
    }

    free(request);
    closesocket(client_fd);
}

int gateway_start(int port) {
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        closesocket(server_fd);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((u_short)port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        closesocket(server_fd);
        return -1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        closesocket(server_fd);
        return -1;
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    g_running = 1;

    printf("Gateway listening on port %d\n", port);

    while (g_running) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);

        SOCKET client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == INVALID_SOCKET) {
            if (g_running) perror("accept");
            continue;
        }

        handle_client(client_fd);
    }

    closesocket(server_fd);
    return 0;
}

void gateway_stop(void) {
    g_running = 0;
}
