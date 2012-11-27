#if !defined(_INTELLIBOT_H)
#define _INTELLIBOT_H

#include <sys/types.h>
#include <time.h>

#include <sqlite3.h>

#include "queue.h"
#include "sock.h"

struct _intellibot;
struct _server;
struct _plugin;
struct _plugin_ctx;
struct _queue;

#define SERVER_FLAG_DISCONNECTED  0x00000000
#define SERVER_FLAG_CONNECTED     0x00000001
#define SERVER_FLAG_REGISTERED    0x00000002

typedef struct _user {
    char nick[51];
    char password[51];
    char **roles;
    unsigned long nroles;
    unsigned int flags;
} USER;

typedef struct _server {
    SOCK *sock;
    unsigned int flags;
    time_t last_ping;

    struct _server *prev, *next;
} SERVER;

typedef struct _plugin_ctx {
    struct _plugin *plugin;
    struct _intellibot *bot;
    struct _server *server;
    char *action;
    void *ctx;
} PLUGIN_CTX;

#define SUBSCRIPTION_JOIN       0x00000001
#define SUBSCRIPTION_PART       0x00000002
#define SUBSCRIPTION_CONNECT    0x00000004
#define SUBSCRIPTION_DISCONNECT 0x00000008
#define SUBSCRIPTION_PRIVMSG    0x00000010

#define SUBSCRIBE(o, s) do { o->subscriptions |= SUBSCRIPTION_##s; } while(0);
#define SUBSCRIBED(o, s) ((o->subscriptions & SUBSCRIPTION_##s) == SUBSCRIPTION_##s)

typedef struct _plugin {
    pthread_t tid;
    void *handle;
    void *ctx;
    unsigned int subscriptions;

    QUEUE *queue;
    int (*run)(struct _plugin *, char *);

    struct _plugin *prev, *next;
} PLUGIN;

typedef struct _column {
    char *name;
    char *data;

    struct _column *prev, *next;
} COLUMN;

typedef struct _row {
    COLUMN *columns;

    struct _row *prev, *next;
} ROW;

typedef struct _sql_ctx {
    char *path;
    pthread_mutex_t execute_mutex;

    sqlite3 *db;
} SQL_CTX;

typedef struct _intellibot {
    PLUGIN *plugins;
    SERVER *servers;
    
    SQL_CTX *db;
} INTELLIBOT;

/* sql.c */
SQL_CTX *Initialize_DB(const char *);
void Deinitialize_DB(INTELLIBOT *);
char *Get_Column(ROW *, char *);
void Free_Column(COLUMN *);
void Free_Rows(ROW *);
ROW *DB_Query(INTELLIBOT *, const char *, int);
void DB_Execute(INTELLIBOT *, const char *);

/* bot.c */
INTELLIBOT *Initialize_Bot(const char *);
void Deinitialize_Bot(INTELLIBOT *);

/* irc.c */
void Server_Loop(INTELLIBOT *);

#endif
