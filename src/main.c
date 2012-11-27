#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "intellibot.h"

void usage(char *path)
{
    fprintf(stderr, "USAGE: %s <db>\n", path);
    exit(1);
}

int main(int argc, char *argv[])
{
    INTELLIBOT *bot;
    ROW *rows, *row;
    COLUMN *column;

    if (argc != 2)
        usage(argv[0]);

    bot = Initialize_Bot(argv[1]);
    if (!(bot)) {
        fprintf(stderr, "[-] Could not initialize bot\n");
        return 1;
    }

    rows = DB_Query(bot, "SELECT * FROM config", 0);
    if (!(rows)) {
        fprintf(stderr, "[-] Query failed\n");
    }

    for (row = rows; row != NULL; row = row->next) {
        for (column = row->columns; column != NULL; column = column->next) {
            fprintf(stderr, "config[%s]: %s\n", column->name, column->data);
        }
    }

    Free_Rows(rows);

    Deinitialize_Bot(bot);

    return 0;
}
