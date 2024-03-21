/****************************************************************************
 *
 * list.c -- Liste doppiamente concatenate con sentinella
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
% LabASD - Liste doppiamente concatenate con sentinella
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-03-18

(Crediti: prof. [Violetta Lonati](http://lonati.di.unimi.it/),
Università di Milano)

> **Versione modificata rispetto a quanto originariamente mostrato in
> laboratorio**: il nodo sentinella è ora un puntatore anziché una
> struttura membro di `List` come in precedenza.  Questo consente di
> evitare il cast che era necessario a eliminare il `const` nella
> funzione `list_end()`. In generale, eliminare il `const` in modo
> forzoso non è consigliabile e andrebbe evitato.

![Fonte: <https://www.youtube.com/watch?v=UqmnUe4pDdU>](treno.jpg)

Implementare la struttura dati "lista doppiamente concatenata con
sentinella", la cui interfaccia è specificata nel file
[list.h](list.h). In una lista doppiamente concatenata, ogni nodo (che
ha tipo `ListNode`) ha un puntatore al nodo precedente e al nodo
successivo, oltre ad un attributo `val` che contiene il valore
memorizzato in quel nodo. I valori sono di tipo `ListInfo`, che di
default è impostato a `int`.

```C
typedef int ListInfo;

typedef struct ListNode {
    ListInfo val;
    struct ListNode *succ, *pred;
} ListNode;

typedef struct {
    int length;
    ListNode *sentinel;
} List;
```

In una lista con sentinella è presente un nodo speciale, detto appunto
"sentinella", che non contiene alcuna informazione utile ma serve solo
per marcare l'inizio (o la fine) della lista; di fatto la presenza
della sentinella trasforma la lista in un anello chiuso. La sentinella
consente di accedere in tempo $O(1)$ al primo o ultimo elemento della
lista: il primo elemento è il successore della sentinella, mentre
l'ultimo è il predecessore.

Il tipo `List` è una struttura contenente un campo che indica la
lunghezza della lista (esclusa la sentinella), e un puntatore alla
sentinella (Figura 1).

![Figura 1: Lista doppiamente concatenata con sentinella](list.svg)

Nel caso di lista vuota, c'è comunque il nodo sentinella,
il cui successore e predecessore è il nodo stesso (Figura 2).

![Figura 2: Lista vuota con sentinella](empty-list.svg)

Oltre a consentire accesso in tempo costante agli estremi della lista,
la sentinella semplifica le operazioni di inserimento e cancellazione,
perché grazie ad essa ogni nodo ha sempre un predecessore e un
successore.  Non occorre quindi gestire in modo speciale la lista
vuota o le operazioni che coinvolgono il primo o l'ultimo elemento.

Nel file [list.h](list.h) vengono descritte le funzioni
dell'interfaccia del tipo di dato `List`. L'unico punto di attenzione
è l'iterazione. Come esempio consideriamo la funzione `list_print(L)`
che stampa il contenuto di `L`. La funzione si può realizzare mediante
un ciclo `while`

```C
void list_print(List *L)
{
    ListNode *node = list_first(L);
    while (node != list_end(L)) {
        printf("%d ", node->val);
        node = list_succ(node);
    }
    printf("\n");
}
```

oppure `for`

```C
void list_print(List *L)
{
    ListNode *node;
    for (node = list_first(L); node != list_end(L); node = list_succ(node)) {
        printf("%d ", node->val);
    }
    printf("\n");
}
```

Si noti che `list_end()` _non restituisce un puntatore all'ultimo nodo
della lista, ma alla sentinella_; questo è conforme al comportamento
di analoghe strutture dati in linguaggi come Java o C++;

Il file [list-main.c](list-main.c) contiene un `main()` che legge ed
esegue una sequenza di comandi da un file il cui nome va specificato
sulla riga di comando. L'elenco dei comandi con il relativo
significato è riportato nella Tabella 1.

: Tabella 1: Comandi nel file di input

-------------------------------------------------------------------------
Operazione    Significato
------------  -----------------------------------------------------------
`+` _k_       Inserisci un nuovo nodo contenente _k_ in testa alla lista.
              Sono ammessi valori duplicati.

`-` _k_       Cancella il primo nodo contenente _k_, se presente;
              altrimenti, non fa nulla.

`?` _k_       Cerca il valore _k_ nella lista, e stampa un opportuno
              messaggio per indicarne o meno la presenza.

`n` _idx_     Stampa il valore contenuto nel nodo in posizione _idx_;
              il primo nodo ha posizione _idx_ = 0,
              il secondo ha posizione _idx_ = 1, e così via.
              Se _idx_ è maggiore o uguale al numero di elementi
              della lista, stampa un opportuno messaggio.

`l`           Stampa la lunghezza della lista.

`p`           Stampa tutto il contenuto della lista.

`c`           Svuota la lista, cancellando tutti i nodi in essa
              contenuti.
-------------------------------------------------------------------------

È possibile inserire più volte lo stesso valore; in caso di
cancellazione di un valore ripetuto, sarà sufficiente cancellarne una
occorrenza qualsiasi.

Un esempio di file di input è [list.in](list.in). Per compilare:

        gcc -std=c90 -Wall -Wpedantic list.c list-main.c -o list-main

Per eseguire in ambiente Linux/MaxOSX:

        ./list-main list.in

Per eseguire in ambiente Windows:

        .\list-main list.in

Come sempre, è possibile realizzare ulteriori funzioni di supporto.

## Suggerimento

Le funzioni `list_add_first()` e `list_add_last()` possono essere
espresse facilmente sfruttando una funzione ausiliaria (da definire)

        static void list_insert_after(List *L, ListNode *n, ListInfo k)

che crea un nuovo nodo contenente l'informazione `k` e lo inserisce
immediatamente dopo il nodo `n` della lista `L`.

## File

- [list.c](list.c)
- [list.h](list.h)
- [list-main.c](list-main.c)
- [list.in](list.in) ([output atteso](list.out))
- [list1.in](list1.in)
- [list2.in](list2.in)
- [list3.in](list3.in)
- [list4.in](list4.in)

***/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* Crea un nuovo nuovo oggetto nodo contenente valore `v`. I puntatori
   al successore e predecessore del nuovo nodo puntano a se stesso.
   Trattandosi di una funzione ad uso interno (non è elencata
   nell'interfaccia descritta nel file <list.h>), è dichiarata
   "static" in modo che non sia visibile esternamente a questo file
   sorgente. */
static ListNode *list_new_node(int v)
{
    ListNode *r = (ListNode *)malloc(sizeof(ListNode));
    assert(r != NULL); /* evitiamo un warning con VS */
    r->val = v;
    r->succ = r->pred = r;
    return r;
}

/* Restituisce l'indirizzo di memoria della sentinella di `L` */
ListNode *list_end(const List *L)
{
    assert(L != NULL);
    return L->sentinel;
}

List *list_create( void )
{
    List *L = (List*)malloc(sizeof(List));
    assert(L != NULL);

    L->length = 0;
    L->sentinel = list_new_node(0); /* il valore contenuto nel nodo sentinella è irrilevante */
    return L;
}

int list_length(const List *L)
{
    assert(L != NULL);

    return L->length;
}



void list_clear(List *L)
{
    /* [TODO] */
}

void list_destroy(List *L)
{
    list_clear(L);
    free(L->sentinel);
    free(L);
}

/* Nota: questa funzione assume che `ListInfo` sia il tipo "int" */
void list_print(const List *L)
{
    const ListNode *node;

    assert(L != NULL);

    printf("(");
    for (node = list_first(L); node != list_end(L); node = list_succ(node)) {
        printf("%d ", node->val);
    }
    printf(")\n");
}

int list_is_empty(const List *L)
{
    assert(L != NULL);

    return (list_first(L) == list_end(L));
}

ListNode *list_search(const List *L, ListInfo k)
{
    /* [TODO] */
    return NULL; /* Sostituire con il valore di ritorno corretto */
}

ListNode *list_first(const List *L)
{
    assert(L != NULL);

    return L->sentinel->succ;
}

ListNode *list_last(const List *L)
{
    assert(L != NULL);

    return L->sentinel->pred;
}


/* Inserisce un nuovo nodo contenente k all'inizio della lista */
void list_add_first(List *L, ListInfo k)
{
    assert(L != NULL);
    /* [TODO] */
}

/* Inserisce un nuovo nodo contenente k alla fine della lista */
void list_add_last(List *L, ListInfo k)
{
    assert(L != NULL);
    /* [TODO] */
}

/* Rimuove il nodo n dalla lista L */
void list_remove(List *L, ListNode *n)
{
    assert(L != NULL);
    assert(n != NULL);
    assert(n != list_end(L));
    /* [TODO] */
}

ListInfo list_remove_first(List *L)
{
    /* [TODO] */
    return -1; /* Sostituire con il valore di ritorno corretto */
}

ListInfo list_remove_last(List *L)
{
    /* [TODO] */
    return -1; /* Sostituire con il valore di ritorno corretto */
}

ListNode *list_succ(const ListNode *n)
{
    assert(n != NULL);

    return n->succ;
}

ListNode *list_pred(const ListNode *n)
{
    assert(n != NULL);

    return n->pred;
}

ListNode *list_nth_element(const List *L, int n)
{
    /* [TODO] */
    return NULL; /* Sostituire con il valore di ritorno corretto */
}

void list_concat(List *L1, List *L2)
{
    assert(L1 != NULL);
    assert(L2 != NULL);

    /* [TODO] */
}

int list_equal(const List *L1, const List *L2)
{
    assert(L1 != NULL);
    assert(L2 != NULL);

    /* [TODO] */
    return -1; /* Sostituire con il valore di ritorno corretto */
}
