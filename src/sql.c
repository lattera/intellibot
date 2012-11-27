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

    if (sqlite3_open(path, &(ctx->db)) != SQLITE_OK) {
        pthread_mutex_destroy(&(ctx->execute_mutex));
        free(ctx->path);
        free(ctx);

        return NULL;
    }

    return ctx;
}

void Deinitialize_DB(INTELLIBOT *bot)
{
    sqlite3_close(bot->db->db);
}

char *Get_Column(ROW *row, char *name)
{
    COLUMN *column;

    for (column = row->columns; column != NULL; column = column->next)
        if (!strcmp(column->name, name))
            return column->data;

    return NULL;
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
    ROW *row, *nrow;

    if (!(rows))
        return;

    row = rows;
    while (row) {
        nrow = row->next;

        column = row->columns;
        while (column) {
            next = column->next;

            Free_Column(column);

            column = next;
        }

        free(row);
        row = nrow;
    }
}

int sqlite3_query_callback(void *ctx, int ncols, char **coldata, char **colnames)
{
    ROW *row = (ROW *)ctx;
    COLUMN *column;
    int i=0;

    if (row->columns) {
        while (row->next != NULL)
            row = row->next;

        row->next = calloc(1, sizeof(ROW));
        if (!(row->next))
            return -1;

        row->next->prev = row;
        row = row->next;
    };

    for (i = 0; i < ncols; i++) {
        if (!(row->columns)) {
            column = row->columns = calloc(1, sizeof(COLUMN));
            if (!(column))
                return -1;
        } else {
            column->next = calloc(1, sizeof(COLUMN));
            if (!(column->next))
                return -1;

            column->next->prev = column;
            column = column->next;
        }

        column->name = strdup(colnames[i]);
        if (coldata[i])
            column->data = strdup(coldata[i]);
    } while (++i < ncols);

    return 0;
}

ROW *DB_Query(INTELLIBOT *bot, const char *query, int needs_lock)
{
    ROW *rows;
    char *error;
    int e;

    if (needs_lock)
        pthread_mutex_lock(&(bot->db->execute_mutex));

    rows = calloc(1, sizeof(ROW));

    if ((e = sqlite3_exec(bot->db->db, query, sqlite3_query_callback, rows, &error)) != SQLITE_OK) {
        if (error) {
            fprintf(stderr, "[-] sqlite3_exec:%d: %s\n", e, error);
            sqlite3_free(error);
        }

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
