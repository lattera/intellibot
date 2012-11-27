#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "intellibot.h"

SQL_CTX *Initialize_DB(const char *path)
{
    SQL_CTX *ctx;

    ctx = calloc(1, sizeof(SQL_CTX));
    if (!(ctx))
        return NULL;

    ctx->path = strdup(path);
    if (!(ctx->path)) {
        free(ctx);
        return NULL;
    }

    pthread_mutex_init(&(ctx->execute_mutex), NULL);

    return ctx;
}

void Deinitialize_DB(INTELLIBOT *bot)
{
    sqlite3_close(bot->db->db);
}

void Free_Column(COLUMN *column)
{
    free(column->data);
    free(column->name);
    free(column);
}

void Free_Rows(ROW *rows)
{
    COLUMN *column, *next;

    if (!(rows))
        return;

    column = rows->columns;
    while (column) {
        next = column->next;

        Free_Column(column);

        column = next;
    }
}

int sqlite3_query_callback(void *ctx, int ncols, char **coldata, char **colnames)
{
    ROW *row = (ROW *)ctx;
    COLUMN *column;
    int i;

    if (row->columns) {
        while (row->next != NULL)
            row = row->next;

        row->next = calloc(1, sizeof(ROW));
        if (!(row->next))
            return -1;

        row->next->prev = row;
        row = row->next;
    };

    column = row->columns = calloc(1, sizeof(COLUMN));
    for (i=0; i < ncols; i++) {
        column->name = strdup(colnames[i]);
        column->data = strdup(coldata[i]);
    }

    return 0;
}

ROW *DB_Query(INTELLIBOT *bot, const char *query, int needs_lock)
{
    ROW *rows;

    if (needs_lock)
        pthread_mutex_lock(&(bot->db->execute_mutex));

    rows = calloc(1, sizeof(ROW));

    if (sqlite3_exec(bot->db->db, query, sqlite3_query_callback, rows, NULL)) {
        free(rows);

        if (needs_lock)
            pthread_mutex_unlock(&(bot->db->execute_mutex));

        return NULL;
    }

    if (needs_lock)
        pthread_mutex_unlock(&(bot->db->execute_mutex));

    return rows;
}

void DB_Execute(INTELLIBOT *bot, const char *query)
{
    Free_Rows(DB_Query(bot, query, 1));
}
