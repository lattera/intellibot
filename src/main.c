#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "intellibot.h"

int main(int argc, char *argv[])
{
    INTELLIBOT *bot;

    bot = calloc(1, sizeof(INTELLIBOT));
    if (!(bot)) {
        fprintf(stderr, "[-] Unable to allocate bot's memory\n");
        return 1;
    }

    free(bot);

    return 0;
}
