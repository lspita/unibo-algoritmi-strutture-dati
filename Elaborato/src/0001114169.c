#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MIN_DIMENSION 5
#define MAX_DIMENSION 250

#define END_OUTPUT_VAL -1

/* UTILS */

/*
Try to allocate nxsize bytes of memory to 0 (calloc) on the heap
and assert it is allocated (!= NULL)
*/
void *safe_malloc(int n, size_t size)
{
    void *p = NULL;
    p = (void *)calloc(n, size);
    assert(p != NULL);
    return p;
}

/*
Checks if coordinates i,j are inside of a nxm matrix
*/
int matrix_in_bounds(
    const int i,
    const int j,
    const int n,
    const int m)
{
    return (i >= 0 &&
            i < n &&
            j >= 0 &&
            j < m);
}

/* GRAPH IMPLEMENTATION */

/*
Graph node
*/
typedef struct Node
{
    int x;
    int y;
    int val;
} Node;

/*
Graph edge
*/
typedef struct Edge
{
    Node *src;  /* source node */
    Node *dst;  /* destination node */
    int weight; /* weight of the edge */

    struct Edge *next; /* next edge in the adjacent list */
} Edge;

/*
List of adjacent nodes
*/
typedef struct AdjacencyList
{
    Edge *e; /* list of edges to adjacent nodes of src */

    struct Edge *head;     /* pointer to the head of the list */
    struct Edge *sentinel; /* pointer for list loops */
} AdjacencyList;

typedef struct Graph
{
    int n;               /* number of nodes */
    int m;               /* number of edges */
    AdjacencyList **adj; /* nxm matrix of adjacency lists (adj[x][y] = adjacents to node x,y) */
} Graph;

/*
Create node
*/
Node *new_node(const int x, const int y, const int val)
{
    Node *n = (Node *)safe_malloc(1, sizeof(Node));
    n->x = x;
    n->y = y;
    n->val = val;

    return n;
}

/*
Create edge between two nodes
*/
Edge *new_edge(const Node *const src, const Node *const dst, const int weight)
{
    assert(src != NULL);
    assert(dst != NULL);

    Edge *e = (Edge *)safe_malloc(1, sizeof(Edge));
    e->src = src;
    e->dst = dst;
    e->weight = weight;
    e->next = NULL;

    return e;
}

/*
Create an empty adjacency list
*/
AdjacencyList new_adjacency_list()
{
    AdjacencyList adj;
    adj.e = NULL;
    adj.head = NULL;
    adj.sentinel = NULL;

    return adj;
}

Graph new_graph(const int n, const int m)
{
    Graph graph;
    int i, j;
    graph.n = n;
    graph.m = m;

    graph.adj = (AdjacencyList **)safe_malloc(n, sizeof(AdjacencyList *));
    for (i = 0; i < n; i++)
    {
        graph.adj[i] = (AdjacencyList *)safe_malloc(m, sizeof(AdjacencyList));
        for (j = 0; j < m; j++)
        {
            graph.adj[i][j] = new_adjacency_list();
        }
    }

    return graph;
}

/*
Head insert an edge to an adjacency list
*/
void push_adjacent(Edge *edge, AdjacencyList *list)
{
    assert(edge != NULL);
    assert(list != NULL);

    edge->next = list->head;
    list->head = edge;
}

/* MAIN PROGRAM */

/*
Calculate cost of cell step from value x to value y
*/
int edge_weight(const int x,
                const int y,
                const int C_cell,
                const int C_height)
{
    // return C_cell +
}

/*
Read the file and extract the input values

Returns: pointer to the H matrix
Output params:
- C_cell: cell movement weight constant
- C_height: cell height difference weight constant
- n: rows of H
- m: columns of H
*/
int **parse_file(FILE *filein,
                 int *const C_cell,
                 int *const C_height,
                 int *const n,
                 int *const m)
{
    int **H;
    int i, j;

    /* read parameters */
    fscanf(filein, "%d", C_cell);
    fscanf(filein, "%d", C_height);
    fscanf(filein, "%d", n);
    fscanf(filein, "%d", m);

    /* read matrix */
    H = (int **)safe_malloc(n, sizeof(int *));
    for (i = 0; i < n; i++)
    {
        H[i] = (int *)safe_malloc(m, sizeof(int));
        for (j = 0; j < m; j++)
        {
            fscanf(filein, "%d", &(H[i][j]));
        }
    }
}

/*
Convert the H matrix to a graph
*/
Graph matrix_to_graph(const int *const *const H,
                      const int C_cell,
                      const int C_height,
                      const int n,
                      const int m)
{
    Graph graph;
    int i, j;

    graph = new_graph(n, m);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
        }
    }
}

void find_path()
{
}

void print_output()
{
}

int main(int argc, char *argv[])
{
    FILE *filein = stdin;
    int **H;
    int n, m, C_cell, C_height;
    Graph graph;

    /* open file from command arguments*/
    if (argc != 2)
    {
        fprintf(stderr, "Invocare il programma con: %s input_file\n", argv[0]);
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

    /* parse input file */
    H = parse_file(filein, &n, &m, &C_cell, &C_height);

    /* close file */
    if (filein != stdin)
        fclose(filein);

    /* convert the H matrix to a graph */
    graph = matrix_to_graph(H, n, m, C_cell, C_height);

    /* find lightest path */
    find_path();

    /* print the path found */
    print_output();

    return EXIT_SUCCESS;
}