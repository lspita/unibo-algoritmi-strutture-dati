/****************************************************************************
 *
 * dfs.c -- Visita in profondità di grafi orientati
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

/***
% LabASD - Visita in profondità (DFS)
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-04-17

[![DFS Demo](dfs-demo.png "Demo visita DFS")](dfs-demo.html)

Realizzare l'algoritmo di visita in profondità (_Depth-First Search_,
DFS) di un grafo orientato $G=(V,E)$ con $n$ nodi e $m$ archi. Si
chiede di realizzare la versione di DFS che **esplora l'intero
grafo** e non solo la componente raggiungibile da un nodo sorgente
dato. Cliccando sull'immagine in cima alla pagina è possibile vedere
l'algoritmo DFS in azione
([fonte](https://bl.ocks.org/mbostock/949c772b81296f8e4188)).

La visita in profondità parte da ogni nodo non ancora esplorato $v$, e
percorre i cammini "il più a lungo possibile". Si presti attenzione al
fatto che **la visita in profondità non trova i cammini di lunghezza
massima**. Per ogni nodo $v$ viene calcolato l'istante `discover[v]`
in cui il nodo viene incontrato per la prima volta, e l'istante
`finish[v]` in cui tutti i nodi raggiungibili da $v$ sono stati
esplorati. Si avrà sempre che `discover[v] < finish[v]`. L'istante di
apertura `discover[v]` del primo nodo visitato è 1, in conformità con
lo pseudocodice visto a lezione.

Durante la visita ogni nodo viene colorato di bianco, grigio o
nero. Tutti i nodi sono inizialmente bianchi, ad indicare che non sono
ancora stati visitati. Un nodo raggiunto per la prima volta viene
colorato di grigio; un nodo diventa nero quando sono stati esplorati
tutti i nodi da esso raggiungibili.

L'algoritmo DFS può essere espresso sia in forma iterativa che
ricorsiva.  Tuttavia, consiglio di realizzare la versione ricorsiva
perché è più semplice da scrivere.

Per compilare:

        gcc -std=c90 -Wall -Wpedantic graph.c dfs.c -o dfs

Per eseguire in ambiente Linux/MacOSX:

        ./dfs graph10.in

Per eseguire in ambiente Windows:

        .\dfs graph10.in

## File

- [dfs.c](dfs.c)
- [graph.c](graph.c)
- [graph.h](graph.h)
- [graph10.in](graph10.in) ([possibile output](graph10-dfs.out); l'output può essere diverso in base all'ordine con cui si visitano i nodi)
- [graph100.in](graph100.in)
- [graph1000.in](graph1000.in)

***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "graph.h"

/* Si può usare il simbolo NODE_UNDEF per indicare che il predecessore
   della lista dei padri non esiste. */
const int NODE_UNDEF = -1;

typedef enum
{
    WHITE,
    GREY,
    BLACK
} Color; /* colori dei nodi */

Graph *G;
int *p;        /* array di predecessori; `p[i]` è il predecessore di
                  `i` lungo il cammino dal nodo usato come
                  sorgente. Se `i` è un nodo sorgente, si pone `p[i]
                  = -1`. */
int t;         /* tempo corrente (per gli array discover[], finish[]) */
int *discover; /* `discover[v]` è l'istante di tempo in cui il nodo
                  `v` è stato visitato per la prima volta. */
int *finish;   /* `finish[v]` è l'istante di tempo in cui tutti i
                  discendenti di `v` sono stati visitati. */
Color *color;  /* `color[v]` è il colore del nodo `v` (vedi libro di
                  testo) */

/* Visita il grafo `G` (definito nell'omonima variabile globale)
   usando l'algoritmo di visita in profondità (DFS) partendo da tutti
   i nodi. Non viene passato un nodo sorgente, perché questa versione
   di DFS deve visitare l'intero grafo. */
void dfs(void)
{
    /* [TODO] */
}

/* Stampa i predecessori di ciascun nodo lungo l'albero di visita;
 stampa inoltre l'istante di apertura (`discover`) e chiusura
 (`finish`) di ciascun nodo. Come visto a lezione, questi valori
 servono tra le altre cose per classificare gli archi ("in avanti",
 "all'indietro"). */
void print_dfs(void)
{
    const int n = graph_n_nodes(G);
    int v;

    printf("     v |   p[v] | discover |   finish\n");
    printf("-------+--------+----------+----------\n");
    for (v = 0; v < n; v++)
    {
        printf("%6d | %6d | %8d | %8d\n", v, p[v], discover[v], finish[v]);
    }
}

int main(int argc, char *argv[])
{
    FILE *filein = stdin;
    int n;

    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s file_grafo\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-") != 0)
    {
        filein = fopen(argv[1], "r");
        if (filein == NULL)
        {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    G = graph_read_from_file(filein);
    n = graph_n_nodes(G);
    p = (int *)malloc(n * sizeof(*p));
    assert(p != NULL);
    discover = (int *)malloc(n * sizeof(*discover));
    assert(discover != NULL);
    finish = (int *)malloc(n * sizeof(*finish));
    assert(finish != NULL);
    color = (Color *)malloc(n * sizeof(*color));
    assert(color != NULL);
    dfs();
    print_dfs();
    graph_destroy(G);
    free(p);
    free(discover);
    free(finish);
    free(color);
    if (filein != stdin)
        fclose(filein);

    return EXIT_SUCCESS;
}
