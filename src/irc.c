#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/select.h>

#include "intellibot.h"

void Server_Loop(INTELLIBOT *bot)
{
    SERVER *server;
    int maxfd, i;
    fd_set readfds;
    struct timeval tv;
    char buf[1025];
    ssize_t retval;

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    while (1) {
        maxfd = 0;
        FD_ZERO(&readfds);
        memset(buf, 0x00, sizeof(buf));

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
            continue;
        }

        for (server = bot->servers; server != NULL; server = server->next) {
            if (FD_ISSET(server->sock->sockfd, &readfds)) {
                retval = SocketRead(server->sock, buf, sizeof(buf)-1);
                fprintf(stderr, "%s", buf);
                fprintf(stderr, "\n");
            }
        }
    }
}
