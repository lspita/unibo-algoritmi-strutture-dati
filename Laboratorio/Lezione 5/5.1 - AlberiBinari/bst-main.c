/****************************************************************************
 *
 * bst-main.c -- Demo alberi binari di ricerca
 *
 * Copyright (C) 2021, 2022, 2024 Moreno Marzolla
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
#include "bst.h"

int randab(int a, int b)
{
    return a + rand() % (b-a+1);
}

void inputgen( int nops )
{
    const int PROB_INS = 44;
    const int PROB_DEL = 24;
    const int PROB_SEA = 20;
    const int PROB_HEI = 5;
    const int PROB_SIZ = 5;
    int i;

    assert(PROB_INS + PROB_DEL + PROB_SEA + PROB_HEI + PROB_SIZ <= 100);
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
        } else if (coin < PROB_INS + PROB_DEL + PROB_SEA + PROB_HEI) {
            printf("h\n");
        } else if (coin < PROB_INS + PROB_DEL + PROB_SEA + PROB_HEI + PROB_SIZ) {
            printf("s\n");
        } else {
            printf("p\n");
        }
    }
    printf("s\n");
}

/* Nota: il main assume che BSTKey sia il tipo "int" */
int main( int argc, char *argv[] )
{
    char op;
    BSTKey k;
    BST *T;
    BSTNode *n;
    FILE *filein = stdin;
    int retval;

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

    T = bst_create();

    while (1 == fscanf(filein, " %c", &op)) {
        switch (op) {
        case '+': /* insert */
            fscanf(filein, "%d", &k);
            printf("bst_insert(T, %d) = ", k);
            retval = bst_insert(T, k);
            if (retval) {
                printf("OK\n");
            } else {
                printf("ALREADY PRESENT\n");
            }
            break;
        case '-': /* delete */
            fscanf(filein, "%d", &k);
            printf("bst_delete(T, %d) = ", k);
            n = bst_search(T, k);
            if (n) {
                bst_delete(T, n);
                printf("OK\n");
            } else {
                printf("NOT FOUND\n");
            }
            break;
        case '?': /* search */
            fscanf(filein, "%d", &k);
            printf("bst_search(T, %d) = ", k);
            n = bst_search(T, k);
            if (n != NULL) {
                printf("FOUND\n");
            } else {
                printf("NOT FOUND\n");
            }
            break;
        case 'h': /* height */
            printf("bst_height(T) = %d\n", bst_height(T));
            break;
        case 's': /* size */
            printf("bst_size(T) = %d\n", bst_size(T));
            break;
        case 'p': /* print */
            bst_print(T);
            break;
        default:
            printf("Unknown command %c\n", op);
            exit(EXIT_FAILURE);
        }
    }

    bst_destroy(T);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
