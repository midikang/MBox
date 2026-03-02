/*
 * c-claw - 极简C语言AI Agent运行时
 * Minimal C AI Agent Runtime
 *
 * 核心头文件，定义所有数据结构和API
 * Core header file defining all data structures and APIs
 */

#ifndef CLAW_H
#define CLAW_H

#include <stddef.h>
#include <time.h>

/* 消息结构体 / Core message structure */
typedef struct {
    char role[16];      /* 角色: user/assistant / Role: user/assistant */
    char *content;      /* 消息内容 / Message content */
} Message;

/* AI服务提供商配置 / Provider configuration */
typedef struct {
    char base_url[256];     /* API基础URL / API base URL */
    char api_key[256];      /* API密钥 / API key */
    char model[64];         /* 模型名称 / Model name */
    float temperature;      /* 温度参数 / Temperature parameter */
} Provider;

/* 定时任务结构体 / Cron task structure */
typedef struct {
    int id;                 /* 任务ID / Task ID */
    int interval_sec;       /* 执行间隔(秒) / Interval in seconds */
    time_t next_run;        /* 下次执行时间 / Next run time */
    int enabled;            /* 是否启用 / Enabled flag */
    char prompt[512];       /* 提示词 / Prompt text */
} CronTask;

/* 配置结构体 / Configuration structure */
typedef struct {
    Provider provider;      /* 服务提供商配置 / Provider configuration */
    int gateway_port;       /* HTTP网关端口 / Gateway port */
    int max_history;        /* 最大历史记录数 / Max history count */
} Config;

/* 命令行接口函数 / CLI command functions */
int cli_main(int argc, char **argv);                       /* 主入口 / Main entry */
int cmd_onboard(void);                                      /* 初始化配置 / Initialize config */
int cmd_chat(const char *prompt);                           /* 带历史的对话 / Chat with history */
int cmd_ask(const char *prompt);                            /* 单次提问 / Single question */
int cmd_gateway(int port);                                  /* 启动HTTP网关 / Start HTTP gateway */
int cmd_cron_add(int interval, const char *prompt);         /* 添加定时任务 / Add cron task */
int cmd_daemon(void);                                       /* 后台服务 / Daemon service */

/* AI服务提供商API / Provider API */
int provider_init(const Provider *cfg);                     /* 初始化提供商 / Initialize provider */
char *provider_chat(Message *msgs, int count);              /* 发送对话请求 / Send chat request */
void provider_cleanup(void);                                /* 清理资源 / Cleanup resources */

/* HTTP网关服务器 / Gateway HTTP server */
int gateway_start(int port);                                /* 启动网关 / Start gateway */
void gateway_stop(void);                                    /* 停止网关 / Stop gateway */

/* 配置管理 / Config management */
int config_load(Config *cfg);                               /* 加载配置 / Load config */
int config_save(const Config *cfg);                         /* 保存配置 / Save config */

/* 对话记忆/日志 / Memory/conversation log */
int memory_append(const char *role, const char *content);   /* 追加记录 / Append record */
Message *memory_load(int *count);                           /* 加载历史 / Load history */
void memory_free(Message *msgs, int count);                 /* 释放内存 / Free memory */

/* 定时任务引擎 / Cron task engine */
int cron_load(CronTask **tasks, int *count);                /* 加载任务 / Load tasks */
int cron_save(CronTask *tasks, int count);                  /* 保存任务 / Save tasks */
int cron_add(int interval, const char *prompt);             /* 添加任务 / Add task */
void cron_tick(void);                                       /* 执行检查 / Tick check */

/* 文件系统工具 / File system utilities */
char *fs_get_home(void);                                    /* 获取HOME目录 / Get home dir */
char *fs_config_path(void);                                 /* 配置文件路径 / Config path */
char *fs_memory_path(void);                                 /* 记忆文件路径 / Memory path */
char *fs_cron_path(void);                                   /* 定时任务文件路径 / Cron path */
int fs_ensure_dir(const char *path);                        /* 确保目录存在 / Ensure dir exists */

/* 后台服务 / Daemon service */
int daemon_start(void);                                     /* 启动后台服务 / Start daemon */

/* 工具函数 / Utility functions */
char *str_escape_json(const char *str);                     /* JSON转义 / JSON escape */
char *str_extract_field(const char *json, const char *field); /* 提取JSON字段 / Extract JSON field */

#endif /* CLAW_H */
