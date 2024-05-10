#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MIN_DIMENSION 5
#define MAX_DIMENSION 250

#define END_OUTPUT_VAL -1

/* STRUCTS */

/*
Graph node
*/
typedef struct Node
{
    int row; /* y position in matrix */
    int col; /* x position in matrix */
    int val; /* height value */
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
    struct Edge *head; /* pointer to the head of the list */
    struct Edge *e;    /* pointer for list loops */
} AdjacencyList;

/*
Directed Weigthed Graph
*/
typedef struct Graph
{
    int n;               /* number of rows */
    int m;               /* number of columns */
    Node **nodes;        /* n x m matrix of nodes */
    AdjacencyList **adj; /* n x m matrix of adjacency lists (adj[x][y] = adjacents to node x,y) */
} Graph;

/* DEBUG */
void print_graph(Graph *g)
{
    int i, j;
    AdjacencyList *adj;

    printf("GRAPH (%d x %d):\n-------------------\n", g->n, g->m);
    for (i = 0; i < g->n; i++)
    {
        for (j = 0; j < g->m; j++)
        {
            printf("%d, ", g->nodes[i][j].val);
        }
        putchar('\n');
    }

    printf("EDGES:\n");
    for (i = 0; i < g->n; i++)
    {
        for (j = 0; j < g->m; j++)
        {
            printf("[%d, %d]-> ", i, j);

            adj = &(g->adj[i][j]);
            adj->e = adj->head;
            while (adj->e != NULL)
            {
                printf("[%d, %d], ", adj->e->dst->row, adj->e->dst->col);

                adj->e = adj->e->next;
            }
            putchar('\n');
        }
    }
}

/* UTILS */

/*
Try to allocate nxsize bytes of memory and assert it is allocated (!= NULL)
*/
void *safe_malloc(int n, size_t size)
{
    void *p = NULL;
    p = (void *)malloc(n * size);
    assert(p != NULL);
    return p;
}

/*
Checks if coordinates i,j are inside of a n x m matrix
*/
int matrix_in_bounds(int i, int j, int n, int m)
{
    return (i >= 0 &&
            i < n &&
            j >= 0 &&
            j < m);
}

/* Return the square of an integer */
int square_int(int x)
{
    return x * x;
}

/* GRAPH */

/*
Create node
*/
Node new_node(int row, int col, int val)
{
    Node n;
    n.row = row;
    n.col = col;
    n.val = val;

    return n;
}

/*
Create edge between two nodes
*/
Edge *new_edge(Node *src, Node *dst, int weight)
{
    Edge *e;
    assert(src != NULL);
    assert(dst != NULL);

    e = (Edge *)safe_malloc(1, sizeof(Edge));
    e->src = src;
    e->dst = dst;
    e->weight = weight;
    e->next = NULL;

    return e;
}

/*
Calculate height difference weight
*/
int height_difference(int x, int y, int C_height)
{
    return C_height * square_int(x - y);
}

/*
Create an empty adjacency list
*/
AdjacencyList new_adjacency_list()
{
    AdjacencyList adj;
    adj.e = NULL;
    adj.head = NULL;

    return adj;
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

/*
Create edges between src and the 4 adjacent nodes
while inserting them in the corresponding adjacency list
*/
void connect_adjacents(Graph *graph, Node *src, int C_height)
{
    int i, adj_row, adj_col, w;
    Node *dst = NULL;
    Edge *e = NULL;

    /* prepare x,y movements to select 4 adjacents */
    int mov_row[4] = {0, -1, 0, 1};
    int mov_col[4] = {-1, 0, 1, 0};

    /* loop 4 adjacent nodes */
    for (i = 0; i <= 4; i++)
    {
        adj_row = src->row + mov_row[i];
        adj_col = src->col + mov_col[i];

        if (matrix_in_bounds(adj_row, adj_col, graph->n, graph->m) == 1) /* adjacent node in bounds */
        {
            /* create edge */
            dst = &(graph->nodes[adj_row][adj_col]);
            w = height_difference(src->val, dst->val, C_height);
            e = new_edge(src, dst, w);

            /* add to adjacency list */
            push_adjacent(e, &(graph->adj[src->row][src->col]));
        }
    }
}

/*
Create new empty graph with n x m nodes
*/
Graph new_graph(int n, int m)
{
    Graph graph;
    int i, j;
    graph.n = n;
    graph.m = m;

    /* init nodes */
    graph.nodes = (Node **)safe_malloc(n, sizeof(Node *));
    for (i = 0; i < n; i++)
    {
        graph.nodes[i] = (Node *)safe_malloc(m, sizeof(Node));
    }

    /* init adjacency lists */
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

/* MAIN PROGRAM */

/*
Read the file and extract the input values

Returns: pointer to the H matrix
Output params:
- C_cell: cell movement weight ant
- C_height: cell height difference weight ant
- n: rows of H
- m: columns of H
*/
int **parse_file(FILE *filein,
                 int *C_cell,
                 int *C_height,
                 int *n,
                 int *m)
{
    int **H;
    int i, j;

    /* read parameters */
    fscanf(filein, "%d", C_cell);
    fscanf(filein, "%d", C_height);
    fscanf(filein, "%d", n);
    fscanf(filein, "%d", m);

    /* read matrix */
    H = (int **)safe_malloc(*n, sizeof(int *));
    for (i = 0; i < *n; i++)
    {
        H[i] = (int *)safe_malloc(*m, sizeof(int));
        for (j = 0; j < *m; j++)
        {
            fscanf(filein, "%d", &(H[i][j]));
        }
    }

    return H;
}

/*
Convert the H matrix to a graph
*/
Graph matrix_to_graph(int **H,
                      int C_cell,
                      int C_height,
                      int n,
                      int m)
{
    Graph graph;
    int i, j;

    graph = new_graph(n, m);

    /* add nodes */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            graph.nodes[i][j] = new_node(i, j, H[i][j]);
        }
    }

    /* add edges */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            connect_adjacents(&graph, &(graph.nodes[i][j]), C_height);
        }
    }

    print_graph(&graph);

    return graph;
}

void find_path()
{
}

void print_output()
{
}

int main(int argc, char *argv[])
{
    char *filename = "test/test0.in";
    FILE *filein = stdin;
    int **H;
    int n, m, C_cell, C_height;
    Graph graph;

    /* get file name from command arguments */

    /*
    if (argc != 2)
    {
        fprintf(stderr, "Invocare il programma con: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }
    filename = argv[1];
    */

    filein = fopen(filename, "r");
    if (filein == NULL)
    {
        fprintf(stderr, "Can not open %s\n", argv[1]);
        return EXIT_FAILURE;
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