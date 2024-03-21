/****************************************************************************
 *
 * hashtable.c -- Tabelle Hash
 *
 * Copyright (C) 2021--2024 Nicolas Farabegoli, Moreno Marzolla
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
% LabASD - Tabelle Hash
% Nicolas Farabegoli <nicolas.farabegoli2@unibo.it>
  Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-03-18

Realizzare una tabella hash in cui le collisioni vengono gestite
mediante liste di collisione (_chaining_). Le chiavi sono stringhe di
caratteri zero-terminate, come le normali stringhe in C; ad ogni
chiave è associato un valore intero, da trattare come "dato
satellite". L'interfaccia della struttura dati è descritta nel file
[hashtable.h](hashtable.h) a cui si rimanda per le specifiche delle
singole funzioni.

La tabella hash è rappresentata dalla struttura `HashTable` così
definita:

```C
typedef struct {
    HashNode **items;
    int size;
    int values_count;
} HashTable;
```

dove `items` è un array di `size` puntatori, ciascuno dei quali punta
all'inizio della lista di collisione, e `values_count` è il numero di
coppie (chiave, valore) presenti nella tabella hash (inizialmente
zero). Il campo `values_count` consente di conoscere in tempo $O(1)$
il numero di chiavi presenti nella hashtable, senza doverle contare di
volta in volta. Tale informazione può essere mantenuta aggiornata in
tempo $O(1)$ durante l'esecuzione delle altre operazioni.

I nodi delle liste di collisione sono rappresentati dalla struttura
`HashNode`:

```C
typedef struct HashNode {
    char *key;
    int value;
    struct HashNode *next;
} HashNode;
```

Come si vede, si tratta di una normale implementazione di liste
concatenate, in cui ogni nodo ha un puntatore al nodo successivo;
l'ultimo nodo della lista ha `NULL` come successore. In questa
implementazione non è richiesto di mantenere ordinato il contenuto
delle liste di collisione; è quindi possibile aggiungere i nuovi nodi
dalla testa, in modo da semplificare il codice.

Nella Figura 1 viene mostrato un esempio.

![Figura 1: Esempio di tabella hash con `size = 10`](hashtable.svg)

Vengono fornite due funzioni `encode(key)` e `hash_function(h,k)` per
associare le chiavi (stringhe) agli slot della tabella hash. La
funzione `encode(key)` trasforma una stringa in un valore intero
ottenuto sommando i codici ASCII di tutti i caratteri[^1].

[^1]: Sommare i codici ASCII non è una buona idea, sia perché se non
      viene fatto attentamente può causare overflow, sia perché può
      produrre collisioni in modo troppo prevedibile. La funzione
      `encode()` non andrebbe quindi usata in applicazioni reali. Una
      funzione hash che viene talvolta raccomandata è
      [djb2](http://www.cse.yorku.ca/~oz/hash.html).

Non è possibile usare direttamente il risultato per indicizzare la
tebella hash, dato che `encode()` potrebbe restituire un valore
superiore alla dimensione della tabella hash. Occorre invece usare la
funzione

```C
unsigned long hash_function(const HashTable *table, unsigned long k)
```

che accetta come parametri un puntatore ad una tabella hash `h` e un
intero `k` di tipo `unsigned long`, e restituisce un intero
appartenente all'insieme $\{0, \ldots, \texttt{h->size}-1\}$ usando
l'operatore modulo. Il valore prodotto da `hash_function()` viene
usato come indice dell'array `h->items[]` per decidere in quale lista
di trabocco inserire la chiave data.

L'uso di due funzioni separate, `encode()` e `hash_function()`, per
associare una stringa ad un elemento dell'array, è motivato dal fatto
che nel libro di testo si da per scontato che le chiavi siano interi,
mentre nel nostro caso le chiavi sono stringhe. Ho quindi deciso di
rendere esplicita la fase di trasformazione di una stringa in un
intero, che poi viene usato nella funzione hash.

Il programma [hashtable-main.c](hashtable-main.c) legge una sequenza
di comandi da un file il cui nome va passato sulla riga di comando. I
comandi manipolano una tabella hash inizialmente vuota, e sono
descritti nella Tabella 1. Il file [hashtable.in](hashtable.in)
contiene un esempio di input.

: Tabella 1: Comandi nel file di input

------------------------------------------------------------------------------
Comando           Significato
----------------  ------------------------------------------------------------
_n_               Inizializza la tabella hash creando _n_ puntatori
                  a liste di collisione, inizialmente vuote;
                  questa istruzione deve comparire
                  una sola volta all'inizio del file.

`+` _key val_     Inserisce la chiave _key_ con valore _val_.

`-` _key_         Cancella la chiave _key_ e il valore associato, se presenti.

`?` _key_         Restituisce il valore associato alla chiave _key_, se
                  presente, oppure un opportuno messaggio che indica
                  che la chiave non è presente.

`c`               Svuota la tabella hash.

`s`               Restituisce il numero di coppie (chiave, valore)
                  presenti nella tabella hash.

`p`               Stampa il contenuto della tabella hash
                  in forma leggibile; usare per debug.
------------------------------------------------------------------------------

Le chiavi `key` sono stringhe che non contengono spazi; i valori `val`
sono interi.

Nel caso in cui si tenti di inserire una chiave già presente nella
tabella, occorre aggiornare il vecchio valore con quello nuovo (si
faccia riferimento alla specifica della funzione `hash_insert()` nel
file [hashtable.h](hashtable.h)). Tale comportamento rispecchia il
modo in cui vengono gestite le tabelle hash in altri linguaggi di
programmazione, come ad esempio Java.

Per compilare:

        gcc -std=c90 -Wall -Wpedantic hashtable.c hashtable-main.c -o
hashtable-main

Per eseguire in ambiente Linux/MacOSX:

        ./hashtable-main hashtable.in

Per eseguire in ambiente Windows:

        .\hashtable-main hashtable.in

## Per approfondire

- Supponiamo di partire con una hash table di dimensione $k$, cioè una
  hash table in cui ci sono $k$ liste di collisione inizialmente
  vuote. Supponiamo di inserire $n$ coppie (chiave, valore), in cui
  tutte le chiavi sono distinte. Qual è la lunghezza media delle liste
  di collisione, nel caso migliore in cui l'hash delle chiavi sia
  uniformemente distribuito? Qual è invece la lunghezza _massima_ di
  una lista di collisione nel caso peggiore?

- La funzione `encode()` converte una stringa di caratteri in un
  valore numerico, da cui si ricava l'hash tramite l'operatore modulo.
  Come detto sopra, la funzione `encode()` non è molto sofisticata;
  riuscite a costruire una sequenza di chiavi diverse che producono
  tutte lo stesso valore hash?

## File

- [hashtable.c](hashtable.c)
- [hashtable.h](hashtable.h)
- [hashtable-main.c](hashtable-main.c)
- [hashtable.in](hashtable.in) ([output atteso](hashtable.out))
- [hashtable1.in](hashtable1.in)
- [hashtable2.in](hashtable2.in)
- [hashtable3.in](hashtable3.in)
- [hashtable4.in](hashtable4.in)

***/

#include "hashtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long hash_function(const HashTable *table, unsigned long k)
{
    assert(table != NULL);

    return k % table->size;
}

unsigned long encode(const char *key)
{
    int i;
    unsigned long s;

    assert(key != NULL);

    s = 0;
    /* Secondo lo standard C99
       (http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
       sezione 6.2.5/9) l'aritmetica con tipi unsigned non produce mai
       overflow; non sono riuscito a trovare quale sia il
       comportamento atteso in ANSI C, ma per i fini di questo
       esercizio assumiamo che sia lo stesso. */
    for (i = 0; key[i]; i++)
    {
        s += key[i];
    }
    return s;
}

/* Ritorna true (nonzero) se le chiavi k1 e k2 sono uguali, cioè se le
   stringhe sono uguali carattere per carattere. */
static int keys_equal(const char *k1, const char *k2)
{
    assert(k1 != NULL);
    assert(k2 != NULL);

    return (0 == strcmp(k1, k2));
}

HashTable *ht_create(const int size)
{
    HashTable *h = (HashTable *)malloc(sizeof(*h));
    int i;

    assert(h != NULL);
    h->size = size;
    h->values_count = 0;
    h->items = (HashNode **)malloc(h->size * sizeof(*(h->items)));
    assert(h->items != NULL);
    for (i = 0; i < h->size; i++)
    {
        h->items[i] = NULL;
    }
    return h;
}

/* Funzione ausiliaria che crea un nuovo nodo per le liste di trabocco
   contenente una copia della chiave `key` con valore associato il
   valore `value`. Il successore del nuovo nodo viene posto a
   `next`. */
static HashNode *hashtable_new_node(const char *key, int value, HashNode *next)
{
    HashNode *item = (HashNode *)malloc(sizeof(HashNode));
    const int keylen = strlen(key);

    assert(item != NULL);
    /* Per duplicare la stringa `key` sarebbe possibile usare la
       funzione `strdup()`. Purtroppo tale funzione non è parte di
       ANSI C né di C99, ma è inclusa nelle estensioni POSIX. Di
       conseguenza, potrebbe non essere disponibile ovunque. Conviene
       quindi realizzare la copia manualmente, allocando un buffer di
       lunghezza opportuna e poi copiando `key` in tale buffer. */
    item->key = (char *)malloc(keylen + 1);
    assert(item->key != NULL);
    strcpy(item->key, key);
    item->next = next;
    item->value = value;
    return item;
}

/* Libera la memoria allocata per il nodo n della tabella hash.
   Libera anche la memoria riservata alla chiave. */
static void free_node(HashNode *n)
{
    assert(n != NULL);

    free(n->key);
    free(n);
}

int ht_insert(HashTable *h, const char *key, int value)
{
    unsigned long index;
    HashNode *head = NULL, *node = NULL;

    if (h->size)
    {
    }

    /* [TODO] */
    return -1; /* Cambiare il valore di ritorno in modo opportuno */
}

HashNode *ht_search(HashTable *h, const char *key)
{
    /* [TODO] */
    return NULL; /* Cambiare il valore di ritorno in modo opportuno */
}

int ht_delete(HashTable *h, const char *key)
{
    /* [TODO] */
    return -1; /* Cambiare il valore di ritorno in modo opportuno */
}

void ht_clear(HashTable *h)
{
    int i;

    assert(h != NULL);

    for (i = 0; i < h->size; i++)
    {
        HashNode *current = h->items[i];
        while (current != NULL)
        {
            HashNode *next = current->next;
            free_node(current);
            current = next;
        }
        h->items[i] = NULL;
    }
    h->values_count = 0;
}

void ht_destroy(HashTable *h)
{
    assert(h != NULL);

    ht_clear(h);
    free(h->items);
    h->items = NULL; /* non serve */
    h->size = h->values_count = 0;
    free(h);
}

int ht_count(const HashTable *h)
{
    assert(h != NULL);
    return (h->values_count);
}

void ht_print(const HashTable *h)
{
    int i;
    assert(h != NULL);
    for (i = 0; i < h->size; i++)
    {
        const HashNode *iter;
        printf("[%3d] ", i);
        for (iter = h->items[i]; iter != NULL; iter = iter->next)
        {
            printf("->(%s, %d)", iter->key, iter->value);
        }
        printf("\n");
    }
}
