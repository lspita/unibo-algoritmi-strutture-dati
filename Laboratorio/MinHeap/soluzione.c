/****************************************************************************
 *
 * minheap.c -- Min-Heap binario
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
% LabASD - Min-Heap binario
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-03-15

![Fonte: <https://xkcd.com/835/>](http://imgs.xkcd.com/comics/tree.png)

Scopo di questo esercizio è implementare un _min-heap binario_;
formuleremo il problema in modo leggermente diverso da quanto visto a
lezione (e descritto nel libro di testo), perché questa struttura dati
ci servirà in futuro.

Un min-heap è un contenitore di coppie (`key`, `prio`), di capienza
massima `size`, dove

- `key` è un intero compreso tra 0 e (`size`-1) inclusi,

- `prio` è un numero reale che rappresenta la priorità associata a
  quella chiave. Lo heap deve mantenere la proprietà di ordinamento
  parziale rispetto alla priorità.

> **Attenzione**: nel libro, `key` indica la priorità rispetto alla
> quale mantenere l'ordinamento parziale; qui invece `key` è un
> satellite, e `prio` è l'attributo in base al quale ordinare. La
> specifica adottata in questo esercizio ci servirà per implementare
> gli algoritmi di Dijkstra e di Prim (che vedremo più avanti).

Ogni chiave può apparire al più una volta; le priorità invece sono
numeri reali arbitrari (anche negativi), e non sono soggette ad alcun
vincolo. Lo heap consente di trovare efficientemente la coppia (`key`,
`prio`) con `prio` minima.

Un min-heap viene rappresentato tramite un array di strutture `HeapElem`:

```C
typedef struct {
    int key;
    double prio;
} HeapElem;

typedef struct {
    HeapElem *heap;
    int n;
    int size;
} MinHeap;
```

dove `size` è la dimensione dell'array `heap[]`, e `n` è il numero di
coppie (`key`, `prio`) effettivamente presenti nello heap in un dato
momento. Quindi, si avrà sempre che $0 \leq n \leq \texttt{size}$.

![](minheap-struct.svg)

La struttura `MinHeap` rappresenta l'oggetto heap; le funzioni
che agiscono su uno heap accettano come parametro un puntatore
a `MinHeap`.

In un min-heap ogni nodo soddisfa la seguente proprietà:

> La priorità di un nodo è _minore o uguale_ a quella di entrambi i
> figli (se esistono)

Il file [minheap.h](minheap.h) contiene la specifica dell'interfaccia
della struttura dati min-heap. Si chiede di realizzare le funzioni
indicate, lasciando `minheap_change_prio()` per ultima perché è la più
laboriosa. È sempre possibile definire ulteriori funzioni di supporto
che si ritengano utili.

La Figura 1 mostra un esempio di min-heap con capienza massima `size =
8` contenente $n = 5$ elementi. L'attributo `prio` di ogni nodo ha
valore minore o uguale a quello dei figli (se presenti). Lo heap è
rappresentato da un array di `size` elementi, di cui i primi `n`
contengono le coppie (chiave, priorità) effettivamente presenti,
mentre i restanti hanno contenuto indefinito.

![Figura 1: Esempio di min-heap di capienza massima `size = 8` contenente $n = 5$ elementi, e sua rappresentazione tramite array](minheap.svg)

> **Attenzione**: le formule date a lezione per individuare l'indice
> dei figli/del padre di un nodo $i$ devono essere adattate al
> linguaggio C in cui gli array sono indicizzati a partire da zero
> anziché da uno.

## `minheap_insert()`

L'inserimento di una nuova coppia (chiave, priorità) avviene dalla
base dello heap. La nuova coppia viene inserita nell'elemento
`heap[n]` e si incrementa $n$. Successivamente, si confronta
l'attributo `prio` del nodo appena inserito con quello del padre,
effettuando uno scambio nel caso in cui venga violata la proprietà di
ordine. Gli scambi con il padre vanno ripetuti fino a quando il nuovo
nodo raggiunge il livello corretto (Figura 2).

![Figura 2: Inserimento della coppia `(key=4, prio=1)`](minheap-insert.svg)

## `minheap_change_prio()`

L'operazione più laboriosa è la `minheap_change_prio()`, che modifica
la priorità associata ad una chiave già presente nello heap. Se la
priorità di un nodo aumenta, occorre applicare l'operazione
`move_up()` (una funzione ausiliaria che viene fornita con questo
esercizio; si è liberi di usarla o meno) per far "scendere" il nodo
fino al livello corretto (Figura 3).

![Figura 3: `minheap_change_prio(h, 0, 31.0)`](minheap-change-prio1.svg)

Nel caso in cui la priorità diminuisca, potrebbe rendersi necessario
effettuare scambi con il padre usando un'altra funzione ausiliaria
`move_down()` fino a quando il nuovo nodo raggiunge il livello
corretto, in modo simile a quanto fatto durante l'inserimento (Figura
4).

![Figura 4: `minheap_change_prio(h, 7, -2.0)`](minheap-change-prio2.svg)

## `minheap_delete_min()`

Per cancellare il nodo contenente la priorità minima, occorre procedere
come segue (Figura 5):

- L'ultima foglia a destra (`heap[n-1]`) viene copiata nella radice
  (`heap[0]`, occorre prima salvare la chiave della vecchia radice,
  dato che andrà restituita al chiamante);

- Si decrementa il numero di nodi dello heap;

- Si confronta ed eventualmente scambia il nodo radice con il foglio
  di priorità minima, fino a quando il nodo radice raggiunge la
  profondità corretta.

![Figura 5: `minheap_delete_min(h)`](minheap-delete-min.svg)

## Analisi

In uno heap con $n$ elementi, le operazioni possono essere
implementate con i costi asintotici riportati nella Tabella 1.

: Tabella 1: Costi asintotici delle operazioni su uno heap binario

--------------------------------------------------------------
Operazione                     Costo
-----------------------------  -------------------------------
`minheap_min()`                $O(1)$

`minheap_delete_min()`         $O(\log n)$

`minheap_insert()`             $O(\log n)$

`minheap_change_prio()`        $O(n)$ oppure $O(\log n)$[^1]
--------------------------------------------------------------

[^1]: Per iniziare, realizzare `minheap_change_prio()` mediante una
scansione dell'array `heap[]` per cercare la posizione dell'elemento
di chiave data; questa soluzione richiede tempo $O(n)$ nel caso
peggiore. Nel seguito vengono forniti [alcuni
suggerimenti](#suggerimenti) per ridurre il costo asintotico di questa
operazione a $O(\log n)$ nel caso peggiore.

Il programma [minheap-main.c](minheap-main.c) contiene una funzione
`main()` che legge da file una sequenza di comandi, uno per ogni riga,
che manipolano uno heap inizialmente vuoto. I comandi sono descritti
nella Tabella 2.

: Tabella 2: Comandi nel file di input

----------------------------------------------------------------------------
Comando         Significato
--------------  ------------------------------------------------------------
_n_             Inizializza lo heap per contenere al più _n_ elementi;
                questa istruzione compare una sola volta all'inizio

`+` _key prio_  Inserisce la chiave _key_ con priorità _prio_

`-`             Cancella la coppia <_key, prio_> con priorità minima

`?`             Stampa la chiave associata alla priorità minima

`c` _key prio_  Modifica la priorità associata alla chiave _key_

`s`             Stampa il numero $n$ di elementi presenti nello heap

`p`             Stampa il contenuto dello heap (per debug)
----------------------------------------------------------------------------

Per compilare;

        gcc -std=c90 -Wall -Wpedantic minheap.c minheap-main.c -o minheap-main

Per eseguire in ambiente Linux/MacOSX:

        ./minheap-main minheap.in

Per eseguire in ambiente Windows

        .\minheap-main minheap.in

## Suggerimenti

### Funzioni di utilità

Nel file [minheap.c](minheap.c) sono presenti delle funzioni di
utilità quelle definite _static_, in modo che non possano essere
invocate da funzioni definite fuori da quel file. Lo scopo di ciascuna
è indicato nei commenti al codice.

Di particolare interesse sono le funzioni `move_up(h, i)` e
`move_down(h, i)`, che hanno lo scopo di far salire (rispettivamente,
scendere) il nodo `heap[i]` fino a fargli raggiungere la posizione
corretta. `move_up()` scambia ripetutamente il nodo con il padre,
mentre `move_down()` scambia ripetutamente il nodo con il figlio di
priorità minima; in entrambi i casi, lo scambio avviene se il nodo non
occupa già la posizione corretta rispetto al padre o ai figli.

Le funzioni `minheap_insert()`, `minheap_change_prio()` e
`minheap_delete_min()` possono essere realizzate in modo abbastanza
semplice sfruttando `move_up()` e `move_down()`. L'implementazione
risultante è però diversa da quella vista a lezione (e descritta nel
libro): chi vuole può seguire altre strade.

### Ottimizzazione di `minheap_change_prio()`

Implementare `minheap_change_prio(h, key, newprio)` in modo efficiente
richiede attenzione, perché trovare il nodo dello heap contenente la
chiave data `key` richiede tempo $O(n)$ nel caso peggiore; la chiave,
infatti, potrebbe trovarsi in qualsiasi posizione. Una volta trovato
il nodo, serve tempo $O(\log n)$ per spostarlo verso l'alto o verso il
basso fino a raggiungere il nuovo livello corretto. Quindi il costo
asintotico di `minheap_change_prio()` è dominato dalla ricerca della
chiave, che è l'operazione più lenta.

Sfruttando il fatto che le chiavi appartengono all'insieme $\{0,
\ldots, \texttt{size}-1\}$, e che ogni chiave compare al più una
volta, possiamo effettuare la ricerca in tempo $O(1)$ utilizzando un
secondo array `pos[]` di lunghezza `size`. `pos[k]` è la posizione
(l'indice) dell'elemento di chiave `k` nell'array `heap[]`, se
presente (Figura 6). Quindi, per ogni chiave `k` presente nello heap
deve valere la proprietà:

        heap[pos[k]].key == k

Se `k` non è presente, si pone `pos[k] = -1`.

![Figura 6: Uso dell'array `pos[]` per indicare la posizione (indice)
delle chiavi nell'array `heap[]`](minheap-pos.svg)

Il contenuto di `pos[]` può essere mantenuto aggiornato in tempo
$O(1)$ man mano che gli elementi in `heap[]` vengono scambiati tra
loro durante le varie operazioni sullo heap. Ciò richiede un po' di
attenzione, ma non è particolarmente problematico.

## File

- [minheap.c](minheap.c)
- [minheap.h](minheap.h)
- [minheap-main.c](minheap-main.c)
- [minheap.in](minheap.in) ([output atteso](minheap.out))
- [minheap1.in](minheap1.in)
- [minheap2.in](minheap2.in)
- [minheap3.in](minheap3.in)

 ***/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minheap.h"

void minheap_print(const MinHeap *h)
{
    int i, j, width = 1;

    assert(h != NULL);

    printf("\n** Contenuto dello heap:\n\n");
    printf("n=%d size=%d\n", h->n, h->size);
    printf("Contenuto dell'array heap[] (stampato a livelli):\n");
    i = 0;
    while (i < h->n) {
        j = 0;
        while (j<width && i < h->n) {
            printf("h[%2d]=(%2d, %6.2f) ", i, h->heap[i].key, h->heap[i].prio);
            i++;
            j++;
        }
        printf("\n");
        width *= 2;
    }
    printf("\nContenuto dell'array pos[]:\n");
    for (i=0; i<h->size; i++) {
        printf("pos[%d]=%d ", i, h->pos[i]);
    }
    printf("\n\n** Fine contenuto dello heap\n\n");
}

void minheap_clear( MinHeap *h )
{
    int i;
    assert(h != NULL);
    for (i=0; i<h->size; i++) {
        h->pos[i] = -1;
    }
    h->n = 0;
}

/* Costruisce un min-heap vuoto che può contenere al massimo
   `size` elementi */
MinHeap *minheap_create(int size)
{
    MinHeap *h = (MinHeap*)malloc(sizeof(*h));
    assert(h != NULL);
    assert(size > 0);

    h->size = size;
    h->heap = (HeapElem*)malloc(size * sizeof(*(h->heap)));
    assert(h->heap != NULL);
    h->pos = (int*)malloc(size * sizeof(*(h->pos)));
    assert(h->pos != NULL);
    minheap_clear(h);
    return h;
}

void minheap_destroy( MinHeap *h )
{
    assert(h != NULL);

    h->n = h->size = 0;
    free(h->heap);
    free(h->pos);
    free(h);
}

/**
 ** Di seguito vengono definite alcune funzioni di utilità generale
 ** che potrebbero tornare comode nella gestione dello heap (la
 ** soluzione da me fornita userà queste funzioni). Sono tutte
 ** definite "static" perché possano essere richiamate solo da altre
 ** funzioni in questo file sorgente.
 **
 ** NON È OBBLIGATORIO USARLE: chi vuole può ignorarle o cancellarle,
 ** e procedere con la propria implementazione. (Se non vengono usate
 ** il compilatore segnalerà dei warning, per cui forse conviene
 ** eliminarle se non servono).
 **/

/* Funzione di supporto: restituisce 1 sse l'indice `i` appartiene
   all'intervallo degli indici validi degli elementi validi nell'array
   che rappresenta lo heap. */
static int valid(const MinHeap *h, int i)
{
    assert(h != NULL);

    return ((i >= 0) && (i < h->n));
}

/* Funzione di supporto: scambia heap[i] con heap[j] */
static void swap(MinHeap *h, int i, int j)
{
    HeapElem tmp;

    assert(h != NULL);
    assert(valid(h, i));
    assert(valid(h, j));
    assert(h->pos[h->heap[i].key] == i);
    assert(h->pos[h->heap[j].key] == j);

    tmp = h->heap[i];
    h->heap[i] = h->heap[j];
    h->heap[j] = tmp;

    h->pos[h->heap[i].key] = i;
    h->pos[h->heap[j].key] = j;
}

/* Funzione di supporto: restituisce l'indice del padre del nodo i */
static int parent(const MinHeap *h, int i)
{
    assert(valid(h, i));

    return (i+1)/2 - 1;
}

/* Funzione di supporto: restituisce l'indice del figlio sinistro del
   nodo `i`. Ritorna un indice non valido se `i` non ha figlio
   sinistro. */
static int lchild(const MinHeap *h, int i)
{
    assert(valid(h, i));

    return 2*i + 1;
}

/* Funzione di supporto: restituisce l'indice del figlio destro del
   nodo `i`. Ritorna un indice non valido se `i` non ha figlio
   destro. */
static int rchild(const MinHeap *h, int i)
{
    assert(valid(h, i));

    return 2*i + 2;
}

/* Funzione di supporto: restituisce l'indice del figlio di `i` con
   priorità minima. Se `i` non ha figli, restituisce -1 */
static int min_child(const MinHeap *h, int i)
{
    int l, r, result = -1;

    assert(valid(h, i));

    l = lchild(h, i);
    r = rchild(h, i);
    if (valid(h, l)) {
        result = l;
        if (valid(h, r) && (h->heap[r].prio < h->heap[l].prio)) {
            result = r;
        }
    }
    return result;
}

/* Funzione di supporto: scambia l'elemento in posizione `i` con il
   padre fino a quando raggiunge la posizione corretta nello heap */
static void move_up(MinHeap *h, int i)
{
    int p;

    assert(valid(h, i));

    p = parent(h, i);
    while ( valid(h, p) && (h->heap[i].prio < h->heap[p].prio) ) {
        swap(h, i, p);
        i = p;
        p = parent(h, i);
    }
}

/* Funzione di supporto: scambia l'elemento in posizione `i` con il
   figlio avente priorità minima, fino a quando l'elemento raggiunge
   la posizione corretta. */
static void move_down(MinHeap *h, int i)
{
    int done = 0;

    assert(valid(h, i));

    /* L'operazione viene implementata iterativamente, sebbene sia
       possibile una implementazione ricorsiva probabilmente più
       leggibile. */
    do {
        const int dst = min_child(h, i);
        if (valid(h, dst) && (h->heap[dst].prio < h->heap[i].prio)) {
            swap(h, i, dst);
            i = dst;
        } else {
            done = 1;
        }
    } while (!done);
}

/* Restituisce true (nonzero) se lo heap è vuoto */
int minheap_is_empty(const MinHeap *h)
{
    assert(h != NULL);

    return (h->n == 0);
}

/* Restituisce true (nonzero) se lo heap è pieno, cioè è stata
   esaurita la capienza a disposizione */
int minheap_is_full(const MinHeap *h)
{
    assert(h != NULL);

    return (h->n == h->size);
}

/* Restituisce il numero di elementi presenti nello heap */
int minheap_get_n(const MinHeap *h)
{
    assert(h != NULL);

    return h->n;
}

/* Restituisce la chiave associata alla priorità minima */
int minheap_min(const MinHeap *h)
{
    assert( !minheap_is_empty(h) );

    return h->heap[0].key;
}

/* Come minheap_min(), ma restituisce la coppia (chiave, prio); questa
   funzione verrà utilizzata in future edizioni del corso al posto di
   minheap_min(). */
HeapElem minheap_min2( const MinHeap *h)
{
    assert( !minheap_is_empty(h) );

    return h->heap[0];
}

/* Inserisce una nuova coppia (key, prio) nello heap. */
void minheap_insert(MinHeap *h, int key, double prio)
{
    int i;

    assert( !minheap_is_full(h) );
    assert((key >= 0) && (key < h->size));
    assert(h->pos[key] == -1);

    i = h->n++;
    h->pos[key] = i;
    h->heap[i].key = key;
    h->heap[i].prio = prio;
    move_up(h, i);
}

/* Rimuove la coppia (chiave, priorità) con priorità minima;
   restituisce la chiave associata alla priorità minima. */
int minheap_delete_min(MinHeap *h)
{
    int result;

    assert( !minheap_is_empty(h) );

    result = minheap_min(h);
    swap(h, 0, h->n-1);
    assert( h->heap[h->n - 1].key == result );
    h->pos[result] = -1;
    h->n--;
    if (!minheap_is_empty(h)) {
        move_down(h, 0);
    }
    return result;
}

/* Come minheap_delete_min(), ma restituisce la coppia (chiave, prio);
   questa funzione verrà utilizzata al posto di minheap_delete_min()
   in future edizioni del corso. */
HeapElem minheap_delete_min2(MinHeap *h)
{
    HeapElem result;

    assert( !minheap_is_empty(h) );

    result = minheap_min2(h);
    swap(h, 0, h->n-1);
    assert( h->heap[h->n - 1].key == result.key );
    h->pos[result.key] = -1;
    h->n--;
    if (!minheap_is_empty(h)) {
        move_down(h, 0);
    }
    return result;
}

/* Modifica la priorità associata alla chiave key. La nuova priorità
   può essere maggiore, minore o uguale alla precedente. */
void minheap_change_prio(MinHeap *h, int key, double newprio)
{
    int j;
    double oldprio;

    assert(h != NULL);
    assert(key >= 0 && key < h->size);
    j = h->pos[key];
    assert( valid(h, j) );
    oldprio = h->heap[j].prio;
    h->heap[j].prio = newprio;
    if (newprio > oldprio) {
        move_down(h, j);
    } else {
        move_up(h, j);
    }
}
