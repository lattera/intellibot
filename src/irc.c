#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/select.h>

#include "intellibot.h"

DATA *parse_data(INTELLIBOT *, char *);

void Server_Loop(INTELLIBOT *bot)
{
    SERVER *server;
    int maxfd, i;
    fd_set readfds;
    struct timeval tv;
    ssize_t retval;
    char *line, *p1, *p2;

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    while (1) {
        maxfd = 0;
        FD_ZERO(&readfds);

        for (server = bot->servers; server != NULL; server = server->next) {
            if (server->sock->sockfd > maxfd)
                maxfd = server->sock->sockfd;

            FD_SET(server->sock->sockfd, &readfds);
        }

        /* Reuse maxfd to check select()'s return val */
        maxfd = select(maxfd+1, &readfds, NULL, NULL, &tv);
        if (maxfd < 0 && errno == EAGAIN)
            continue;
        if (maxfd < 0)
            perror("select");

        if (maxfd == 0) {
            /* Perform ping */
            for (server = bot->servers; server != NULL; server = server->next)
                SocketWrite(server->sock, "JOIN #test\n", strlen("JOIN #test\n"));
            continue;
        }

        for (server = bot->servers; server != NULL; server = server->next) {
            if (FD_ISSET(server->sock->sockfd, &readfds)) {
                memset(server->buf, 0x00, server->bufsz);
                retval = SocketRead(server->sock, server->buf, server->bufsz);

                line = p1 = server->buf;
                while ((p1 = strchr(line, '\n'))) {
                    *p1 = 0x00;
                    p2 = strchr(line, '\r');
                    if ((p2))
                        *p2 = 0x00;

                    parse_data(bot, line);
                    line = ++p1;
                }
            }
        }
    }
}

DATA *parse_data(INTELLIBOT *bot, char *buf)
{
    DATA *data;
    char *p1, *p2;

    p1 = strchr(buf, ' ');
    if (!(p1))
        return NULL;

    *p1 = 0x00;

    p2 = strchr(++p1, ' ');
    if (!(p2))
        return NULL;

    *p2 = 0x00;

    data = calloc(1, sizeof(DATA));
    data->hostmask = strdup(buf+1);
    data->action = strdup(p1);

    if (!strcmp(data->action, "PRIVMSG")) {
        p1 = strchr(++p2, ' ');
        if (!(p1)) {
            free(data->hostmask);
            free(data->action);
            free(data);
            return NULL;
        }

        *p1 = 0x00;
        data->chan = strdup(p2);
        data->data = strdup(p1+2);

        if (data->chan[0] == '#') {
            data->replyTo = strdup(data->chan);
        } else {
            p1 = strchr(data->hostmask, '!');
            data->replyTo = malloc((size_t)(p1 - data->hostmask) + 1);
            memcpy(data->replyTo, data->hostmask, (size_t)(p1 - data->hostmask));
            data->replyTo[(size_t)(p1 - data->hostmask)] = 0x00;

        }
        fprintf(stderr, "reply to: %s\n", data->replyTo);
    }

    return data;
}
