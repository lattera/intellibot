#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "intellibot.h"

void Add_Server(INTELLIBOT *, ROW *);

INTELLIBOT *Initialize_Bot(const char *dbpath)
{
    INTELLIBOT *bot;
    ROW *rows, *row;

    bot = calloc(1, sizeof(INTELLIBOT));
    if (!(bot))
        return NULL;

    bot->db = Initialize_DB(dbpath);
    if (!(bot->db)) {
        free(bot);
        return NULL;
    }

    rows = DB_Query(bot, "SELECT * FROM server WHERE active = 1", 0);
    for (row = rows; row != NULL; row = row->next) {
        Add_Server(bot, row);
    }

    Free_Rows(rows);

    return bot;
}

void Deinitialize_Bot(INTELLIBOT *bot)
{
    Deinitialize_DB(bot);
}

void Add_Server(INTELLIBOT *bot, ROW *row)
{
    SERVER *server;

    if (bot->servers) {
        server = bot->servers;
        while (server->next != NULL)
            server = server->next;

        server->next = calloc(1, sizeof(SERVER));
        if (!(server->next))
            return;

        server->next->prev = server;
        server = server->next;
    } else {
        server = bot->servers = calloc(1, sizeof(SERVER));
        if (!(server))
            return;
    }

    server->buf = calloc(1, BUFSZ+1);
    if (!(server->buf)) {
        if ((server->prev))
            server->prev->next = NULL;

        if (server == bot->servers)
            bot->servers = NULL;

        free(server);
        return;
    }
    server->bufsz = BUFSZ;

    server->host = strdup(Get_Column(row, "host"));
    server->port = strdup(Get_Column(row, "port"));
    server->nick = strdup(Get_Column(row, "nick"));

    server->sock = SocketClient(Get_Column(row, "host"), Get_Column(row, "port"), SOCK_TCP);
    if (!(server->sock)) {
        if ((server->prev))
            server->prev->next = NULL;

        if (server == bot->servers)
            bot->servers = NULL;

        free(server);

        return;
    }

    snprintf(server->buf, server->bufsz, "USER %s %s %s :%s\n", server->nick, server->nick, server->host, server->nick);
    SocketWrite(server->sock, server->buf, strlen(server->buf));
    snprintf(server->buf, server->bufsz, "NICK %s\n", server->nick);
    SocketWrite(server->sock, server->buf, strlen(server->buf));

    server->last_ping = time(NULL);
    server->flags |= SERVER_FLAG_CONNECTED;
}
