#include "../include/claw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static Provider g_provider;
static CURL *g_curl = NULL;

typedef struct {
    char *data;
    size_t size;
} ResponseBuffer;

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    ResponseBuffer *buf = (ResponseBuffer *)userp;

    char *ptr = realloc(buf->data, buf->size + total + 1);
    if (!ptr) return 0;

    buf->data = ptr;
    memcpy(&(buf->data[buf->size]), contents, total);
    buf->size += total;
    buf->data[buf->size] = 0;

    return total;
}

char *str_escape_json(const char *str) {
    if (!str) return NULL;

    size_t len = strlen(str);
    char *escaped = malloc(len * 2 + 1);
    if (!escaped) return NULL;

    char *dst = escaped;
    for (const char *src = str; *src; src++) {
        switch (*src) {
            case '\\': *dst++ = '\\'; *dst++ = '\\'; break;
            case '"':  *dst++ = '\\'; *dst++ = '"'; break;
            case '\n': *dst++ = '\\'; *dst++ = 'n'; break;
            case '\r': *dst++ = '\\'; *dst++ = 'r'; break;
            case '\t': *dst++ = '\\'; *dst++ = 't'; break;
            default:   *dst++ = *src; break;
        }
    }
    *dst = 0;
    return escaped;
}

char *str_extract_field(const char *json, const char *field) {
    if (!json || !field) return NULL;

    char pattern[128];
    snprintf(pattern, sizeof(pattern), "\"%s\":", field);

    const char *pos = strstr(json, pattern);
    if (!pos) return NULL;

    pos += strlen(pattern);
    while (*pos == ' ' || *pos == '\t') pos++;

    if (*pos == '"') {
        pos++;
        const char *end = pos;
        while (*end && *end != '"') {
            if (*end == '\\' && *(end + 1)) end += 2;
            else end++;
        }
        if (*end != '"') return NULL;

        size_t len = end - pos;
        char *result = malloc(len + 1);
        if (!result) return NULL;

        char *dst = result;
        for (const char *src = pos; src < end; src++) {
            if (*src == '\\' && src + 1 < end) {
                src++;
                switch (*src) {
                    case 'n': *dst++ = '\n'; break;
                    case 'r': *dst++ = '\r'; break;
                    case 't': *dst++ = '\t'; break;
                    case '\\': *dst++ = '\\'; break;
                    case '"': *dst++ = '"'; break;
                    default: *dst++ = *src; break;
                }
            } else {
                *dst++ = *src;
            }
        }
        *dst = 0;
        return result;
    }

    return NULL;
}

int provider_init(const Provider *cfg) {
    if (!cfg) return -1;

    g_provider = *cfg;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    g_curl = curl_easy_init();

    return g_curl ? 0 : -1;
}

void provider_cleanup(void) {
    if (g_curl) {
        curl_easy_cleanup(g_curl);
        g_curl = NULL;
    }
    curl_global_cleanup();
}

char *provider_chat(Message *msgs, int count) {
    if (!g_curl || !msgs || count <= 0) return NULL;

    size_t json_size = 4096;
    for (int i = 0; i < count; i++) {
        if (msgs[i].content) {
            json_size += strlen(msgs[i].content) * 2 + 128;
        }
    }

    char *json = malloc(json_size);
    if (!json) return NULL;

    char *p = json;
    p += sprintf(p, "{\"model\":\"%s\",\"temperature\":%.1f,\"messages\":[",
                 g_provider.model, g_provider.temperature);

    for (int i = 0; i < count; i++) {
        if (i > 0) *p++ = ',';

        char *escaped = str_escape_json(msgs[i].content);
        if (!escaped) {
            free(json);
            return NULL;
        }

        p += sprintf(p, "{\"role\":\"%s\",\"content\":\"%s\"}",
                     msgs[i].role, escaped);
        free(escaped);
    }

    strcpy(p, "]}");

    char url[512];
    snprintf(url, sizeof(url), "%s/chat/completions", g_provider.base_url);

    ResponseBuffer response = {NULL, 0};

    struct curl_slist *headers = NULL;
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", g_provider.api_key);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(g_curl, CURLOPT_URL, url);
    curl_easy_setopt(g_curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(g_curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(g_curl);

    curl_slist_free_all(headers);
    free(json);

    if (res != CURLE_OK) {
        if (response.data) free(response.data);
        return NULL;
    }

    if (!response.data) return NULL;

    char *content = str_extract_field(response.data, "content");
    free(response.data);

    return content;
}
