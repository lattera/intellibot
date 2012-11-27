#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "intellibot.h"

INTELLIBOT *Initialize_Bot(const char *dbpath)
{
    INTELLIBOT *bot;

    bot = calloc(1, sizeof(INTELLIBOT));
    if (!(bot))
        return NULL;

    bot->db = Initialize_DB(dbpath);
    if (!(bot->db)) {
        free(bot);
        return NULL;
    }

    return bot;
}

void Deinitialize_Bot(INTELLIBOT *bot)
{
    Deinitialize_DB(bot);
}
