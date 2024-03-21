/****************************************************************************
 *
 * list-main.c -- Demo lista
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
#include "list.h"

int randab(int a, int b)
{
    return a + rand() % (b-a+1);
}

void inputgen( int nops )
{
    const int PROB_INS = 55;
    const int PROB_DEL = 5;
    const int PROB_SEA = 20;
    const int PROB_NTH = 5;
    int i;

    assert(PROB_INS + PROB_DEL + PROB_SEA + PROB_NTH <= 100);
    srand(nops);
    for (i=0; i<nops; i++) {
        const int coin = randab(0, 99);
        const int val = randab(0, 99);
        if (coin < PROB_INS) {
            printf("+ %d\n", val);
        } else if (coin < PROB_INS + PROB_DEL) {
            printf("- %d\n", val);
        } else if (coin < PROB_INS + PROB_DEL + PROB_SEA) {
            printf("? %d\n", val);
        } else if (coin < PROB_INS + PROB_DEL + PROB_SEA + PROB_NTH) {
            printf("n %d\n", randab(0, 99));
        } else {
            printf("c\n");
        }
    }
    printf("l\n");
}

/* Nota: si assume che ListInfo sia il tipo "int" */
int main( int argc, char *argv[] )
{
    char op;
    ListInfo k;
    List *L;
    ListNode *n;
    FILE *filein = stdin;
    int idx;

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

    printf("list_create()\n");
    L = list_create();

    while (1 == fscanf(filein, " %c", &op)) {
        switch (op) {
        case '+': /* insert */
            fscanf(filein, "%d", &k);
            printf("list_add_first(L, %d)\n", k);
            list_add_first(L, k);
            break;
        case '-': /* remove */
            fscanf(filein, "%d", &k);
            printf("list_remove(L, %d) = ", k);
            fflush(stdout);
            n = list_search(L, k);
            if (n != list_end(L)) {
                list_remove(L, n);
                printf("OK\n");
            } else {
                printf("NOT FOUND\n");
            }
            break;
        case '?': /* search */
            fscanf(filein, "%d", &k);
            printf("list_search(L, %d) = ", k);
            fflush(stdout);
            n = list_search(L, k);
            if (n != list_end(L)) {
                printf("FOUND\n");
            } else {
                printf("NOT FOUND\n");
            }
            break;
        case 'n': /* nth_element */
            fscanf(filein, "%d", &idx);
            printf("list_nth_element(L, %d) = ", idx);
            fflush(stdout);
            n = list_nth_element(L, idx);
            if (n == list_end(L)) {
                printf("NOT FOUND\n");
            } else {
                printf("%d\n", n->val);
            }
            break;
        case 'l': /* length */
            printf("list_length(L) = %d\n", list_length(L));
            break;
        case 'p': /* print */
            list_print(L);
            break;
        case 'c': /* clear */
            printf("list_clear(L)\n");
            list_clear(L);
            break;
        default:
            fprintf(stderr, "Unknown command %c\n", op);
            return EXIT_FAILURE;
        }
    }

    list_destroy(L);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
