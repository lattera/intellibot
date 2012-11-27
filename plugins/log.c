#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <dlfcn.h>

#include "intellibot.h"

int run(PLUGIN *me, const char *action)
{
    if (!strcmp(action, "init")) {
        SUBSCRIBE(me, JOIN);
        SUBSCRIBE(me, PART);
        SUBSCRIBE(me, PRIVMSG);
    }

    return 0;
}
