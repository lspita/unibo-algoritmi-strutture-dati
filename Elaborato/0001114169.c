#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#define MIN_DIMENSION 5   /* minimum size of a matrix dimension */
#define MAX_DIMENSION 250 /* maximum size of a matrix dimension */
#define END_OUTPUT_VAL -1 /* value of x,y coordinates of last node in output */

#define REALLOC_JUMP 10 /* number of cell to add every time extending a dynamic vector */

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
    struct Node *parent; /* parent node to source */

    int h_index; /* corresponding index in the heap */
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

/* MEMORY */

/*
Try to allocate nxsize bytes of memory and assert it is allocated (!= NULL)
*/
void *safe_malloc(const int n, const size_t size)
{
    void *ptr = NULL;
    ptr = (void *)malloc(n * size);
    assert(ptr != NULL);
    return ptr;
}

/*
Try to extend a memory allocation to nxsize bytes and assert it is allocated (!= NULL)
*/
void *safe_realloc(void *ptr, const int n, const size_t size)
{
    ptr = (void *)realloc(ptr, n * size);
    assert(ptr != NULL);
    return ptr;
}

/*
Free memory and set pointer to NULL
*/
void safe_free(void **ptr)
{
    free(*ptr);
    *ptr = NULL;
}

/* UTILS */

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
    Node *node;
    node = (Node *)safe_malloc(1, sizeof(Node));
    node->row = row;
    node->col = col;
    node->val = val;

    node->parent = NULL;
    node->effort = UINT_MAX; /* simulate infinity */

    node->h_index = -1;

    return node;
}

/*
Create edge between two nodes
*/
Edge *new_edge(Node *const src, Node *const dst, const int weight)
{
    Edge *edge;

    edge = (Edge *)safe_malloc(1, sizeof(Edge));
    edge->src = src;
    edge->dst = dst;
    edge->weight = weight;
    edge->next = NULL;

    return edge;
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
void insert_adjacent(AdjacencyList *const list, Edge *const edge)
{
    edge->next = list->head;
    list->head = edge;
}

/*
Create edges between src and the 4 adjacent nodes
while inserting them in the corresponding adjacency list
*/
void connect_adjacents(Graph *const graph, Node *const src, const int C_height)
{
    int i, adj_row, adj_col, weight;
    Node *dst = NULL;
    Edge *edge = NULL;

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
            dst = graph->nodes[adj_row][adj_col];
            weight = height_difference(src->val, dst->val, C_height);
            edge = new_edge(src, dst, weight);

            /* add to adjacency list */
            insert_adjacent(graph->adj[src->row][src->col], edge);
        }
    }
}

/*
Create new empty graph with n x m nodes
*/
Graph *new_graph(const int n, const int m)
{
    int i, j;
    Graph *graph;
    graph = (Graph *)safe_malloc(1, sizeof(Graph));

    graph->n = n;
    graph->m = m;

    /* init nodes */
    graph->nodes = (Node ***)safe_malloc(n, sizeof(Node **));
    for (i = 0; i < n; i++)
    {
        graph->nodes[i] = (Node **)safe_malloc(m, sizeof(Node *));
    }

    /* init adjacency lists */
    graph->adj = (AdjacencyList ***)safe_malloc(n, sizeof(AdjacencyList **));
    for (i = 0; i < n; i++)
    {
        graph->adj[i] = (AdjacencyList **)safe_malloc(m, sizeof(AdjacencyList *));
        for (j = 0; j < m; j++)
        {
            graph->adj[i][j] = new_adjacency_list();
        }
    }

    return graph;
}

/* HEAP */

/*
Create empty min heap
*/
MinHeap *new_minheap()
{
    MinHeap *heap;
    heap = (MinHeap *)safe_malloc(1, sizeof(MinHeap));

    heap->data = NULL;
    heap->size = heap->n = 0;

    return heap;
}

/*
Check if i is a valid index in the h heap
*/
int heap_valid(const MinHeap *const heap, const int i)
{
    return ((i >= 0) && (i < heap->n));
}

/*
Check if heap is empty
*/
int heap_empty(const MinHeap *const heap)
{
    return heap->n == 0;
}

/*
Return root of heap
*/
Node *heap_min(const MinHeap *const heap)
{
    if (heap_empty(heap))
    {
        return NULL;
    }

    return heap->data[0];
}

/*
Set heap[i] = node and update node.h_index
*/
void heap_set_node(MinHeap *const heap, const int i, Node *const node)
{
    assert(heap_valid(heap, i));

    heap->data[i] = node;
    node->h_index = i;
}

/*
Return parent node index of i
*/
int heap_parent(const int i)
{
    return (i + 1) / 2 - 1;
}

/*
Return left child node index of i
*/
int heap_left(const int i)
{
    return 2 * i + 1;
}

/*
Return right child node index of i
*/
int heap_right(const int i)
{
    return 2 * i + 2;
}

/*
Swap heap[i] and heap[j]
*/
void heap_swap(MinHeap *const heap, const int i, const int j)
{
    Node *tmp = NULL;

    tmp = heap->data[i];
    heap_set_node(heap, i, heap->data[j]);
    heap_set_node(heap, j, tmp);
}

/*
Transform single position to follow the min heap structure
*/
void min_heapify(MinHeap *const heap, const int i)
{
    int l, r, smallest;
    smallest = i;

    l = heap_left(i);
    r = heap_right(i);

    if (heap_valid(heap, l) && heap->data[l]->effort < heap->data[smallest]->effort)
    {
        smallest = l;
    }
    if (heap_valid(heap, r) && heap->data[r]->effort < heap->data[smallest]->effort)
    {
        smallest = r;
    }

    if (smallest != i)
    {
        heap_swap(heap, i, smallest);
        min_heapify(heap, smallest);
    }
}

/*
Extend size of heap vector
*/
void heap_extend(MinHeap *const heap)
{
    if (heap->n >= heap->size)
    {
        heap->size = heap->n + REALLOC_JUMP;
        heap->data = (Node **)safe_realloc(heap->data, heap->size, sizeof(Node *));
    }
    heap->n++;
}

/*
Changes the effort value and updates the heap accordingly
*/
void decrease_effort(MinHeap *const heap, int i, const int new_effort)
{
    int p;
    assert(new_effort <= heap->data[i]->effort);

    heap->data[i]->effort = new_effort;
    p = heap_parent(i);
    while (heap_valid(heap, p) && heap->data[p]->effort > heap->data[i]->effort)
    {
        heap_swap(heap, p, i);
        i = p;
        p = heap_parent(i);
    }
}

/*
Insert node in the heap with the specified effort value
*/
void heap_insert(MinHeap *const heap, Node *const node, const int effort)
{
    int i;

    i = heap->n;
    heap_extend(heap);
    heap_set_node(heap, i, node);
    node->effort = UINT_MAX;

    decrease_effort(heap, node->h_index, effort);
}

/*
Extract root of heap
*/
Node *heap_extract(MinHeap *const heap)
{
    Node *min;
    int i;

    min = heap_min(heap);
    if (min == NULL)
    {
        return NULL;
    }
    min->h_index = -1;

    i = heap->n - 1;
    heap_swap(heap, 0, i);
    heap->n--;
    min_heapify(heap, 0);

    return min;
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
    Graph *graph;

    graph = new_graph(n, m);

    /* add nodes */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            graph->nodes[i][j] = new_node(i, j, H[i][j]);
        }
    }

    /* add edges */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            connect_adjacents(graph, graph->nodes[i][j], C_height);
        }
    }

    return graph;
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
    Graph *graph;

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
    graph = matrix_to_graph(H, C_height, n, m);

    /* find lightest path */

    /* print the path found */
    print_output();

    return EXIT_SUCCESS;
}