#if !defined(_INTELLIBOT_H)
#define _INTELLIBOT_H

struct _intellibot;
struct _sock;
struct _plugin;
struct _plugin_ctx;
struct _queue;

typedef struct _sock {
    int sockfd;
    void *buf;
    size_t bufsz;
    char *addr;
    char *port;
    unsigned int flags;
    char *nick;
} SOCK;

typedef struct _plugin_ctx {
    struct _plugin *plugin;
    struct _intellibot *bot;
    void *ctx;
} PLUGIN_CTX;

typedef int (*connect_cb)(PLUGIN_CTX *);
typedef int (*disconnect_cb)(PLUGIN_CTX *);
typedef int (*join_cb)(PLUGIN_CTX *, const char *, const char *, const char *); /* ctx, chan, nick, nickmask */
typedef int (*part_cb)(PLUGIN_CTX *, const char *, const char *); /* ctx, chan, nick */
typedef int (*privmsg_cb)(PLUGIN_CTX *, const char *, const char *, const char *); /* ctx, chan, nick, msg */

typedef struct _plugin {
    void *handle;
    void *ctx;

    connect_cb connect;
    disconnect_cb disconnect;
    join_cb join;
    part_cb part;
    privmsg_cb privmsg;

    struct _plugin *prev, *next;
} PLUGIN;

typedef struct _intellibot {
    PLUGIN *plugins;

} INTELLIBOT;

#endif
