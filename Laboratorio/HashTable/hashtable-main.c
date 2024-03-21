/****************************************************************************
 *
 * hashtable-main.c -- dimostrazione hash table
 *
 * Copyright (C) 2021--2024 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"

#define MAX_KEY_SIZE 100

int randab(int a, int b)
{
    return a + rand() % (b-a+1);
}

void inputgen( int nops )
{
    const char* keys[] = {"hello", "ciao", "algoritmi", "strutture", "dati", "hash", "table", "a", "abcdabcd", "bcdabcda", "oaic", "algoritmi1", "1algoritmi", "algo1ritmi", "Al-Kwarismi", "al-kwarismi", "1234567890", "0987654321", "1023456789", "1230987654", "123", "321", "132", "312", "213", "231", "567"};
    const size_t NKEYS = sizeof(keys) / sizeof(keys[0]);
    const int PROB_INS = 50;
    const int PROB_DEL = 20;
    const int PROB_SEA = 29;
    int i;

    assert(PROB_INS + PROB_DEL + PROB_SEA <= 100);
    srand(nops);
    printf("%d\n", randab(1000, 2000));
    for (i=0; i<nops; i++) {
        const int coin = randab(0, 99);
        const int idx = randab(0, NKEYS-1);
        const int val = randab(0, 99);
        if (coin < PROB_INS) {
            printf("+ %s %d\n", keys[idx], val);
        } else if (coin < PROB_INS + PROB_DEL) {
            printf("- %s\n", keys[idx]);
        } else if (coin < PROB_INS + PROB_DEL + PROB_SEA) {
            printf("? %s\n", keys[idx]);
        } else {
            printf("c\n");
        }
    }
    printf("s\n");
}

int main( int argc, char *argv[] )
{
    int table_size = 10;
    char key[MAX_KEY_SIZE];
    char op;
    int val, retval;
    HashNode *item;
    HashTable *table;
    FILE *filein = stdin;

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "inputgen") == 0) {
        inputgen(atoi(argv[2]));
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-") != 0) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    if (1 != fscanf(filein, "%d", &table_size)) {
        fprintf(stderr, "Missing size\n");
        return EXIT_FAILURE;
    }
    printf("ht_create(%d)\n", table_size);
    table = ht_create(table_size);

    while (1 == fscanf(filein, " %c", &op)) {
        switch (op) {
        case '+': /* insert */
            fscanf(filein, "%s %d", key, &val);
            printf("ht_insert(table, \"%s\", %d)\n", key, val);
            ht_insert(table, key, val);
            break;
        case '-': /* delete */
            fscanf(filein, "%s", key);
            printf("ht_delete(table, \"%s\") = ", key);
            fflush(stdout);
            retval = ht_delete(table, key);
            printf("%d ", retval);
            if (retval) {
                printf("(OK)\n");
            } else {
                printf("(NOT FOUND)\n");
            }
            break;
        case '?': /* search */
            fscanf(filein, "%s", key);
            printf("ht_search(table, \"%s\") = ", key);
            fflush(stdout);
            item = ht_search(table, key);
            if (item == NULL) {
                printf("NOT FOUND\n");
            } else {
                printf("FOUND value %d\n", item->value);
            }
            break;
        case 'p': /* print */
            ht_print(table);
            break;
        case 'c': /* clear */
            printf("ht_clear(table)\n");
            ht_clear(table);
            break;
        case 's': /* count */
            printf("ht_count(table) = %d\n", ht_count(table));
            break;
        default:
            fprintf(stderr, "Unknown command %c\n", op);
            return EXIT_FAILURE;
        }
    }

    ht_destroy(table);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
