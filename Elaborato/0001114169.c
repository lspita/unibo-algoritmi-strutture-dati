/*
Ludovico Maria Spitaleri 0001114169
Classe A
ludovico.spitaleri@studio.unibo.it
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#define MIN_DIMENSION 5   /* minimum size of a matrix dimension */
#define MAX_DIMENSION 250 /* maximum size of a matrix dimension */
#define END_OUTPUT_VAL -1 /* value of x,y coordinates of last node in output */

#define REALLOC_JUMP 5 /* number of cell to add every time extending a dynamic vector */

/* STRUCTS */

/*
Graph node
*/
typedef struct Node
{
    int row; /* y position in matrix */
    int col; /* x position in matrix */
    int val; /* height value */

    long int effort;     /* total effort to reach this node in the path (dijkstra) */
    struct Node *parent; /* parent node in the path (dijkstra) */

    int h_index; /* corresponding index in the heap */

    struct Node *next; /* next node in the path */
} Node;

/*
Graph edge
*/
typedef struct Edge
{
    Node *src;       /* source node */
    Node *dst;       /* destination node */
    long int weight; /* weight of the edge */

    struct Edge *next; /* next edge in the adjacency list */
} Edge;

/*
List of edges from same source
*/
typedef struct AdjacencyList
{
    struct Edge *head; /* head of edges list */
    struct Edge *e;    /* loop sentinel */
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

/*
Output path of nodes
*/
typedef struct Path
{
    Node *head;      /* head of nodes list */
    Node *n;         /* loop sentinel*/
    long int effort; /* total cost of the path */
} Path;

/* UTILS */

/*
Check if i,j are between 0..n,m
*/
int in_bounds(const int i, const int j, const int n, const int m)
{
    return (i >= 0 &&
            i < n &&
            j >= 0 &&
            j < m);
}

/* MEMORY */

/*
Try to allocate nxsize bytes of memory to 0 and assert it is allocated (!= NULL)
*/
void *safe_malloc(const int n, const size_t size)
{
    void *ptr = NULL;

    ptr = (void *)calloc(n, size);
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
    node->effort = INT_MAX;
    node->parent = NULL;
    node->h_index = -1;
    node->next = NULL;

    return node;
}

/*
Create edge between two nodes
*/
Edge *new_edge(Node *const src, Node *const dst, const long int weight)
{
    Edge *edge;

    assert(src != NULL);
    assert(dst != NULL);

    edge = (Edge *)safe_malloc(1, sizeof(Edge));

    edge->src = src;
    edge->dst = dst;
    edge->weight = weight;
    edge->next = NULL;

    return edge;
}

/*
Calculate height difference between x and y
*/
int height_difference(const int x, const int y)
{
    return (x - y) * (x - y);
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
Deallocate adjacency list memory
*/
void free_adjacency_list(AdjacencyList *adj)
{
    Edge *next;

    assert(adj != NULL);

    adj->e = adj->head;

    while (adj->e != NULL)
    {
        next = adj->e->next;
        free(adj->e);
        adj->e = next;
    }

    free(adj);
}

/*
Head insert an edge to an adjacency list
*/
void insert_adjacent(AdjacencyList *const list, Edge *const edge)
{
    assert(list != NULL);
    assert(edge != NULL);

    edge->next = list->head;
    list->head = edge;
}

/*
Create edges between src and the 4 adjacent nodes
while inserting them in the corresponding adjacency list
*/
void connect_adjacents(Graph *const graph, Node *const src)
{
    int i, adj_row, adj_col;
    long int weight;
    Node *dst = NULL;
    Edge *edge = NULL;

    /* prepare x,y movements to select 4 adjacents */
    int mov_row[4] = {0, -1, 0, 1};
    int mov_col[4] = {-1, 0, 1, 0};

    assert(graph != NULL);
    assert(src != NULL);

    /* loop 4 adjacent nodes */
    for (i = 0; i <= 4; i++)
    {
        adj_row = src->row + mov_row[i];
        adj_col = src->col + mov_col[i];

        if (in_bounds(adj_row, adj_col, graph->n, graph->m) == 1) /* adjacent node in bounds */
        {
            /* create edge */
            dst = graph->nodes[adj_row][adj_col];
            weight = height_difference(src->val, dst->val);
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
    graph->nodes = (Node ***)safe_malloc(graph->n, sizeof(Node **));
    for (i = 0; i < graph->n; i++)
    {
        graph->nodes[i] = (Node **)safe_malloc(graph->m, sizeof(Node *));
    }

    /* init adjacency lists */
    graph->adj = (AdjacencyList ***)safe_malloc(graph->n, sizeof(AdjacencyList **));
    for (i = 0; i < graph->n; i++)
    {
        graph->adj[i] = (AdjacencyList **)safe_malloc(graph->m, sizeof(AdjacencyList *));
        for (j = 0; j < graph->m; j++)
        {
            graph->adj[i][j] = new_adjacency_list();
        }
    }

    return graph;
}

/*
Deallocate graph memory
*/
void free_graph(Graph *graph)
{
    int i, j;

    assert(graph != NULL);

    for (i = 0; i < graph->n; i++)
    {
        for (j = 0; j < graph->m; j++)
        {
            free(graph->nodes[i][j]);
            free_adjacency_list(graph->adj[i][j]);
        }
        free(graph->nodes[i]);
        free(graph->adj[i]);
    }

    free(graph->nodes);
    free(graph->adj);

    free(graph);
}

/* MATRIX */

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
                 int *const out_C_cell,
                 int *const out_C_height,
                 int *const out_n,
                 int *const out_m)
{
    int **H;
    int i, j, C_cell, C_height, n, m;

    assert(filein != NULL);
    assert(out_C_cell != NULL);
    assert(out_C_height != NULL);
    assert(out_n != NULL);
    assert(out_m != NULL);

    /* read parameters */
    fscanf(filein, "%d", &C_cell);
    fscanf(filein, "%d", &C_height);
    fscanf(filein, "%d", &n);
    fscanf(filein, "%d", &m);

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

    /* set out parameters */
    *out_C_cell = C_cell;
    *out_C_height = C_height;
    *out_n = n;
    *out_m = m;
    return H;
}

/*
Convert the H matrix to a graph
*/
Graph *matrix_to_graph(int **H,
                       const int n,
                       const int m)
{
    int i, j;
    Graph *graph;

    assert(H != NULL);

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
            connect_adjacents(graph, graph->nodes[i][j]);
        }
    }

    return graph;
}

/*
Deallocate matrix memory
*/
void free_matrix(int **H, const int n)
{
    int i;

    assert(H != NULL);

    for (i = 0; i < n; i++)
    {
        free(H[i]);
    }
    free(H);
}

/* HEAP */

/*
Create empty min heap
*/
MinHeap *new_heap()
{
    MinHeap *heap;
    heap = (MinHeap *)safe_malloc(1, sizeof(MinHeap));

    heap->data = NULL;
    heap->size = 0;
    heap->n = 0;

    return heap;
}

/*
Deallocate heap memory
*/
void free_heap(MinHeap *heap)
{
    assert(heap != NULL);

    free(heap->data);
    free(heap);
}

/*
Check if i is a valid index in the h heap
*/
int heap_valid(const MinHeap *const heap, const int i)
{
    assert(heap != NULL);

    return ((i >= 0) && (i < heap->n));
}

/*
Check if heap is empty
*/
int heap_empty(const MinHeap *const heap)
{
    assert(heap != NULL);

    return heap->n == 0;
}

/*
Return root of heap
*/
Node *heap_min(const MinHeap *const heap)
{
    assert(heap != NULL);

    if (heap_empty(heap))
    {
        return NULL;
    }

    return heap->data[0];
}

/*
Set heap[i] = node and update node.h_index
*/
void heap_set(MinHeap *const heap, const int i, Node *const node)
{
    assert(heap != NULL);
    assert(node != NULL);
    assert(heap_valid(heap, i));

    heap->data[i] = node;
    node->h_index = i;
}

/*
Return key at heap[i]
*/
int heap_get(MinHeap *const heap, const int i)
{
    assert(heap != NULL);
    assert(heap_valid(heap, i));

    return heap->data[i]->effort;
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
    Node *tmp;

    assert(heap != NULL);
    assert(heap_valid(heap, i));
    assert(heap_valid(heap, j));

    tmp = heap->data[i];
    heap_set(heap, i, heap->data[j]);
    heap_set(heap, j, tmp);
}

/*
Transform single position to follow the min heap structure
*/
void min_heapify(MinHeap *const heap, const int i)
{
    int l, r, smallest;

    assert(heap != NULL);
    assert(heap_valid(heap, i));

    l = heap_left(i);
    r = heap_right(i);

    smallest = i;
    if (heap_valid(heap, l) && heap_get(heap, l) < heap_get(heap, smallest))
    {
        smallest = l;
    }
    if (heap_valid(heap, r) && heap_get(heap, r) < heap_get(heap, smallest))
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
Extend size of heap vector by 1
*/
void heap_extend(MinHeap *const heap)
{
    assert(heap != NULL);

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
void heap_decrease(MinHeap *const heap, int i, const long int new_effort)
{
    int p;

    assert(heap != NULL);
    assert(heap_valid(heap, i));
    assert(new_effort <= heap->data[i]->effort);

    heap->data[i]->effort = new_effort;
    p = heap_parent(i);
    while (heap_valid(heap, p) && heap_get(heap, p) > heap_get(heap, i))
    {
        heap_swap(heap, i, p);
        i = p;
        p = heap_parent(i);
    }
}

/*
Insert node in the heap with the specified effort value
*/
void heap_insert(MinHeap *const heap, Node *const node)
{
    int last;

    assert(heap != NULL);
    assert(node != NULL);

    last = heap->n;
    heap_extend(heap);
    heap_set(heap, last, node);
    heap_decrease(heap, node->h_index, node->effort);
}

/*
Extract root of heap
*/
Node *heap_extract(MinHeap *const heap)
{
    Node *min;
    int last;

    assert(heap != NULL);

    min = heap_min(heap);
    if (min == NULL)
    {
        return NULL;
    }

    last = heap->n - 1;
    heap_set(heap, 0, heap->data[last]);
    heap->n--;
    if (heap->n > 0)
    {
        min_heapify(heap, 0);
    }

    min->h_index = -1;
    return min;
}

/* DIJKSTRA */

/*
Per la ricerca del percorso più leggero, verrà utilizzato Dijkstra in quanto
- Sono presenti cicli, il che scarta l'ordinamento topologico
- I pesi sono tutti positivi (differenza di altitudine x costanti positive)
- Il numero di archi del grafo è theta(n) (una cella della matrice ha max 4 adiacenti),
  implementando quindi la coda utilizzata da Dijkstra tramite un heap, otteniamo un costo theta(nlog(n))
*/

/*
Relax single edge
*/
void relax(Edge *const edge, MinHeap *const heap, const int C_cell, const int C_height)
{
    long int new_effort;

    assert(edge != NULL);
    assert(heap != NULL);

    new_effort = edge->src->effort + (edge->weight * C_height) + C_cell;
    if (edge->dst->effort > new_effort)
    {
        heap_decrease(heap, edge->dst->h_index, new_effort);
        edge->dst->parent = edge->src;
    }
}

/*
Initialize graph nodes to calculate shortest paths from src
*/
void init_single_source(Graph *const graph, Node *const src, const int C_cell)
{
    int i, j;
    Node *node;

    assert(graph != NULL);
    assert(src != NULL);

    for (i = 0; i < graph->n; i++)
    {
        for (j = 0; j < graph->m; j++)
        {
            node = graph->nodes[i][j];
            node->effort = INT_MAX;
            node->parent = NULL;
        }
    }

    src->effort = C_cell;
}

/*
Find lightest path from src to every node
*/
void dijkstra(Graph *const graph, Node *const src, const int C_cell, const int C_height)
{
    MinHeap *Q;
    int i, j;
    Node *node;
    AdjacencyList *adj;

    assert(graph != NULL);
    assert(src != NULL);

    init_single_source(graph, src, C_cell);

    /* fill Q with nodes */
    Q = new_heap();
    for (i = 0; i < graph->n; i++)
    {
        for (j = 0; j < graph->m; j++)
        {
            heap_insert(Q, graph->nodes[i][j]);
        }
    }

    while (heap_empty(Q) == 0)
    {
        node = heap_extract(Q);
        adj = graph->adj[node->row][node->col];

        /* loop adjacents */
        adj->e = adj->head;
        while (adj->e != NULL)
        {
            relax(adj->e, Q, C_cell, C_height);

            adj->e = adj->e->next;
        }
    }

    free_heap(Q);
}

/* PATH */

/*
Create new empty nodes path
*/
Path *new_path()
{
    Path *path;
    path = (Path *)safe_malloc(1, sizeof(Path));

    path->effort = 0;
    path->head = NULL;

    return path;
}

/*
Add node to the path and increase the total path effort
*/
void push_node(Path *const path, Node *const node)
{
    assert(path != NULL);
    assert(node != NULL);

    node->next = path->head;
    path->head = node;
}

/*
Return path to dst based of previously executed dijkstra algorithm
*/
Path *extract_path(Node *const dst)
{
    Path *path;

    assert(dst != NULL);

    path = new_path();
    path->effort += dst->effort;

    path->n = dst;
    while (path->n != NULL)
    {
        push_node(path, path->n);

        path->n = path->n->parent;
    }

    return path;
}

/*
Print x y values divided by space and with a new line after
*/
void print_coordinates(const int row, const int col)
{
    printf("%d %d\n", row, col);
}

/*
Print path nodes and effort
*/
void print_path(Path *const path)
{
    assert(path != NULL);

    path->n = path->head;
    while (path->n != NULL)
    {
        print_coordinates(path->n->row, path->n->col);

        path->n = path->n->next;
    }

    print_coordinates(END_OUTPUT_VAL, END_OUTPUT_VAL);
    printf("%ld\n", path->effort);
}

/* MAIN */

int main(int argc, char *argv[])
{
    char *filename;
    FILE *filein = stdin;
    int **H;
    int n, m, C_cell, C_height;
    Graph *graph;
    Node *start, *end;
    Path *path;

    /* get file name from command arguments */

    if (argc != 2)
    {
        fprintf(stderr, "Invocare il programma con: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }
    filename = argv[1];

    /* filename = "test/test1.in"; */ /* DEBUG */

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
    graph = matrix_to_graph(H, n, m);

    free_matrix(H, n);

    /* find lightest path */
    start = graph->nodes[0][0];
    end = graph->nodes[n - 1][m - 1];

    dijkstra(graph, start, C_cell, C_height);
    path = extract_path(end);

    /* print the path found */
    print_path(path);

    free(path);
    free_graph(graph);

    return EXIT_SUCCESS;
}