#ifndef CLAW_H
#define CLAW_H

#include <stddef.h>
#include <time.h>

/* Core message structure */
typedef struct {
    char role[16];
    char *content;
} Message;

/* Provider configuration */
typedef struct {
    char base_url[256];
    char api_key[256];
    char model[64];
    float temperature;
} Provider;

/* Cron task structure */
typedef struct {
    int id;
    int interval_sec;
    time_t next_run;
    int enabled;
    char prompt[512];
} CronTask;

/* Configuration structure */
typedef struct {
    Provider provider;
    int gateway_port;
    int max_history;
} Config;

/* CLI commands */
int cli_main(int argc, char **argv);
int cmd_onboard(void);
int cmd_chat(const char *prompt);
int cmd_ask(const char *prompt);
int cmd_gateway(int port);
int cmd_cron_add(int interval, const char *prompt);
int cmd_daemon(void);

/* Provider API */
int provider_init(const Provider *cfg);
char *provider_chat(Message *msgs, int count);
void provider_cleanup(void);

/* Gateway HTTP server */
int gateway_start(int port);
void gateway_stop(void);

/* Config management */
int config_load(Config *cfg);
int config_save(const Config *cfg);

/* Memory/conversation log */
int memory_append(const char *role, const char *content);
Message *memory_load(int *count);
void memory_free(Message *msgs, int count);

/* Cron task engine */
int cron_load(CronTask **tasks, int *count);
int cron_save(CronTask *tasks, int count);
int cron_add(int interval, const char *prompt);
void cron_tick(void);

/* File system utilities */
char *fs_get_home(void);
char *fs_config_path(void);
char *fs_memory_path(void);
char *fs_cron_path(void);
int fs_ensure_dir(const char *path);

/* Daemon service */
int daemon_start(void);

/* Utility functions */
char *str_escape_json(const char *str);
char *str_extract_field(const char *json, const char *field);

#endif /* CLAW_H */
