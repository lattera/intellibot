#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "intellibot.h"

INTELLIBOT *init_bot(void)
{
    INTELLIBOT *bot;

    bot = calloc(1, sizeof(INTELLIBOT));
    if (!(bot))
        return NULL;

    return bot;
}

void fini_bot(INTELLIBOT *bot)
{
}
