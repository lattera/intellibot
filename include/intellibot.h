#if !defined(_INTELLIBOT_H)
#define _INTELLIBOT_H

#include <sys/types.h>
#include <time.h>

#include "queue.h"
#include "sock.h"

struct _intellibot;
struct _server;
struct _plugin;
struct _plugin_ctx;
struct _queue;

#define SOCK_FLAG_DISCONNECTED 0
#define SOCK_FLAG_CONNECTED 1
#define SOCK_FLAG_REGISTERED 2

typedef struct _server {
    SOCK *sock;

    struct _server *prev, *next;
} SERVER;

typedef struct _plugin_ctx {
    struct _plugin *plugin;
    struct _intellibot *bot;
    void *ctx;
} PLUGIN_CTX;

#define SUBSCRIPTION_JOIN       0x00000001
#define SUBSCRIPTION_PART       0x00000002
#define SUBSCRIPTION_CONNECT    0x00000004
#define SUBSCRIPTION_DISCONNECT 0x00000008
#define SUBSCRIPTION_PRIVMSG    0x00000010

typedef struct _plugin {
    pthread_t tid;
    void *handle;
    void *ctx;
    unsigned int subscriptions;

    QUEUE *queue;
    int (*run)(struct _plugin *, char *);

    struct _plugin *prev, *next;
} PLUGIN;

typedef struct _intellibot {
    PLUGIN *plugins;
    SERVER *servers;
} INTELLIBOT;

#endif
