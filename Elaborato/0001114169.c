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

    unsigned int effort; /* cost of path to this node */
    struct Node *p;      /* parent node to source */
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
    int n;                /* number of rows */
    int m;                /* number of columns */
    Node ***nodes;        /* n x m matrix of nodes */
    AdjacencyList ***adj; /* n x m matrix of adjacency lists (adj[x][y] = adjacents to node x,y) */
} Graph;

/*
Nodes Min Heap
*/
typedef struct MinHeap
{
    int n;       /* number of elements */
    int size;    /* real size of vector */
    Node **data; /* vector of nodes */
} MinHeap;

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
            printf("%d, ", g->nodes[i][j]->val);
        }
        putchar('\n');
    }

    printf("EDGES:\n");
    for (i = 0; i < g->n; i++)
    {
        for (j = 0; j < g->m; j++)
        {
            printf("[%d, %d]-> ", i, j);

            adj = g->adj[i][j];
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
void *safe_malloc(const int n, const size_t size)
{
    void *p = NULL;
    p = (void *)malloc(n * size);
    assert(p != NULL);
    return p;
}

/*
Free memory and set pointer to NULL
*/
void safe_free(void **p)
{
    free(*p);
    *p = NULL;
}

/*
Checks if coordinates i,j are inside of a n x m matrix
*/
int matrix_in_bounds(const int i, const int j, const int n, const int m)
{
    return (i >= 0 &&
            i < n &&
            j >= 0 &&
            j < m);
}

/* Return the square of an integer */
int square_int(const int x)
{
    return x * x;
}

/* GRAPH */

/*
Create node
*/
Node *new_node(const int row, const int col, const int val)
{
    Node *n;
    n = (Node *)safe_malloc(1, sizeof(Node));
    n->row = row;
    n->col = col;
    n->val = val;

    return n;
}

/*
Create edge between two nodes
*/
Edge *new_edge(Node *const src, Node *const dst, const int weight)
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
int height_difference(const int x, const int y, const int C_height)
{
    return C_height * square_int(x - y);
}

/*
Create an empty adjacency list
*/
AdjacencyList *new_adjacency_list()
{
    AdjacencyList *adj;
    adj = (AdjacencyList *)safe_malloc(1, sizeof(AdjacencyList));

    adj->e = NULL;
    adj->head = NULL;

    return adj;
}

/*
Head insert an edge to an adjacency list
*/
void push_adjacent(Edge *const edge, AdjacencyList *const list)
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
void connect_adjacents(Graph *const g, Node *const src, const int C_height)
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

        if (matrix_in_bounds(adj_row, adj_col, g->n, g->m) == 1) /* adjacent node in bounds */
        {
            /* create edge */
            dst = g->nodes[adj_row][adj_col];
            w = height_difference(src->val, dst->val, C_height);
            e = new_edge(src, dst, w);

            /* add to adjacency list */
            push_adjacent(e, g->adj[src->row][src->col]);
        }
    }
}

/*
Create new empty graph with n x m nodes
*/
Graph *new_graph(const int n, const int m)
{
    int i, j;
    Graph *g;
    g = (Graph *)safe_malloc(1, sizeof(Graph));

    g->n = n;
    g->m = m;

    /* init nodes */
    g->nodes = (Node ***)safe_malloc(n, sizeof(Node **));
    for (i = 0; i < n; i++)
    {
        g->nodes[i] = (Node **)safe_malloc(m, sizeof(Node *));
    }

    /* init adjacency lists */
    g->adj = (AdjacencyList ***)safe_malloc(n, sizeof(AdjacencyList **));
    for (i = 0; i < n; i++)
    {
        g->adj[i] = (AdjacencyList **)safe_malloc(m, sizeof(AdjacencyList *));
        for (j = 0; j < m; j++)
        {
            g->adj[i][j] = new_adjacency_list();
        }
    }

    return g;
}

/* HEAP */

/*
Create empty min heap
*/
MinHeap *new_minheap()
{
    MinHeap *h;
    h = (MinHeap *)safe_malloc(1, sizeof(MinHeap));

    h->data = NULL;
    h->size = h->n = 0;

    return h;
}

/*
Checks if i is a valid index in the h heap
*/
int valid(const MinHeap *const h, const int i)
{
    assert(h != NULL);

    return ((i >= 0) && (i < h->n));
}

/*
Returns parent node index of i
*/
int heap_parent(const int i)
{
    return (i + 1) / 2 - 1;
}

/*
Returns left child node index of i
*/
int heap_left(const int i)
{
    return 2 * i + 1;
}

/*
Returns right child node index of i
*/
int heap_right(const int i)
{
    return 2 * i + 2;
}

/*
Swap heap[i] and heap[j]
*/
void heap_swap(MinHeap *const h, const int i, const int j)
{
    Node *tmp = NULL;

    assert(valid(h, i));
    assert(valid(h, j));

    tmp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = tmp;
}

/*
Transform single position to follow the min heap structure
*/
void min_heapify(MinHeap *const h, const int i)
{
    int l, r, smallest;
    smallest = i;

    l = heap_left(i);
    r = heap_right(i);

    if (valid(h, l) && h->data[l]->effort < h->data[i]->effort)
    {
        smallest = l;
    }
    else if (valid(h, r) && h->data[r]->effort < h->data[i]->effort)
    {
        smallest = r;
    }

    if (smallest != i)
    {
        heap_swap(h, i, smallest);
        min_heapify(h, smallest);
    }
}

void push_node_heap(MinHeap *const h, Node *const n)
{
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
Graph *matrix_to_graph(int **H,
                       const int C_height,
                       const int n,
                       const int m)
{
    int i, j;
    Graph *g;

    g = new_graph(n, m);

    /* add nodes */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            g->nodes[i][j] = new_node(i, j, H[i][j]);
        }
    }

    /* add edges */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            connect_adjacents(g, g->nodes[i][j], C_height);
        }
    }

    print_graph(g);

    return g;
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
    Graph *g;

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
    H = parse_file(filein, &C_cell, &C_height, &n, &m);

    /* close file */
    if (filein != stdin)
        fclose(filein);

    /* convert the H matrix to a graph */
    g = matrix_to_graph(H, C_height, n, m);

    /* find lightest path */

    /* print the path found */
    print_output();

    return EXIT_SUCCESS;
}