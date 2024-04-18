/****************************************************************************
 *
 * graph-main.c -- Test implementazione struttura dati grafo
 *
 * Copyright (C) 2021 Moreno Marzolla
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
#include "graph.h"


int main( int argc, char *argv[] )
{
    Graph *G;
    FILE *filein = stdin;

    if (argc != 2) {
        fprintf(stderr, "Invocare il programma con: %s file_grafo\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-") != 0) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    G = graph_read_from_file(filein);
    graph_print(G);
    graph_destroy(G);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
