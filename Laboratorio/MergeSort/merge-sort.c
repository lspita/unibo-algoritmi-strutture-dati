/****************************************************************************
 *
 * merge-sort.c -- Implementazione algoritmo Merge Sort.
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
% LabASD - Merge sort
% Nicolas Farabegoli <nicolas.farabegoli2@unibo.it>
  Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-03-06

![John von Neumann By LANL -
<http://www.lanl.gov/history/atomicbomb/images/NeumannL.GIF> (archive
copy), Attribution,
<https://commons.wikimedia.org/w/index.php?curid=3429594>](von-neumann.jpg "John von Neumann")

Implementare l'algoritmo _Merge sort_ in modo tale che i test forniti
nello schema di programma allegato vengano superati. Merge sort è
stato ideato da [John Von
Neumann](https://en.wikipedia.org/wiki/John_von_Neumann) nel 1945.

L'algoritmo Merge Sort basa il suo funzionamento sul paradigma _divide
et impera_. L'array da ordinare viene diviso in due metà che sono
ordinate ricorsivamente. Le metà ordinate vengono ricomposte mediante
una procedura `merge()`. Si faccia riferimento alla Figura 1 per un
esempio.

![Figura 1: Esempio di funzionamento di Merge Sort](merge-sort.svg)

Nell'algoritmo Merge Sort la fase _divide_ è semplice, mentre la fase
_impera_ (fusione di due array ordinati) è più laboriosa da
realizzare. Si faccia riferimento al materiale fornito nella parte di
teoria per i dettagli.

Chiamiamo $T(n)$ il tempo necessario ad ordinare un array di lunghezza
$n$ usando Merge Sort; possiamo definire $T(n)$ in modo ricorsivo come
segue:

$$
T(n) = \begin{cases}
c_1 & \mbox{se } n \leq 1 \\
2T(n/2) + c_2 n & \mbox{altrimenti}
\end{cases}
$$

dove $c_1$ e $c_2$ sono costanti. La componente $c_2 n$ indica il
tempo necessario alla fase di fusione (_merge_), che è proporzionale
alla somma delle lunghezze delle metà da fondere. Risolvendo la
ricorrenza, ad esempio usando il Master Theorem, si ha $T(n) =
\Theta(n \log n)$.

L'algoritmo Merge Sort può essere realizzato in modo ricorsivo o
iterativo; in questo esercizio verrà proposta la soluzione ricorsiva,
ma chi lo desidera può implementare l'altra.

Il programma fornito contiene alcuni test per verificare la parziale
correttezza della funzione `merge_sort()`. Nei test si confronta il
risultato prodotto da `merge_sort()` con quello della funzione
`qsort()` della libreria standard C. La dichiarazione di `qsort()` è:

```C
#include <stdlib.h>

void qsort(void *base, size_t nmemb, size_t size,
           int (*compare)(const void *, const void *));
```

dove:

- `base` è un puntatore all'inizio dell'array da ordinare;

- `nmemb` è il numero di elementi dell'array da ordinare (attenzione, **non** il numero di byte!)

- `size` è la dimensione in byte di ciascuno degli elementi dell'array

- `compare` è un puntatore ad una funzione `int cmp(const void *a,
  const void *b)` (il nome della funzione e dei parametri possono
  essere scelti in modo arbitrario) che restituisce:

    + un valore negativo, se `*a` viene prima di `*b` nell'ordinamento;

    + zero, se `*a` e `*b` sono considerati uguali;

    + un valore positivo, se `*a` viene dopo `*b` nell'ordinamento

Compilare con:

        gcc -std=c90 -Wall -Wpedantic merge-sort.c -o merge-sort

Per eseguire in ambiente Linux/MacOSX:

        ./merge-sort

Per eseguire in ambiente Windows:

        .\merge-sort

## Uso della funzione `qsort()`

Riportiamo un esempio di come usare la funzione `qsort()` per ordinare
un array di interi (è possibile ordinare un array di tipo
qualunque).

Occorre innanzitutto definire una funzione usata per confrontare gli
elementi da ordinare, che qui chiamiamo `cmp_increasing()`. Il nome
della funzione è arbitrario, ma il tipo dei parametri e del risultato
devono essere conformi a quanto sopra, cioè deve accettare due
parametri di tipo `const void*` e restituire un intero. La funzione
deve conoscere il vero tipo degli elementi dell'array da ordinare, per
cui bisogna definire una funzione diversa per ogni tipo di dato da
ordinare. Nel nostro caso, i puntatori `a` e `b` vanno convertiti a
puntatori a _interi costanti_, per poi essere internamente
dereferenziati per confrontare il valore puntato.

Il programma seguente mostra come si possa utilizzare in pratica
la funzione `qsort()`.

```C
#include <stdio.h>
#include <stdlib.h>

int cmp_increasing(const void *a, const void *b)
{
  const int va = *(const int*)a;
  const int vb = *(const int*)b;
  if (va < vb)
    return -1;
  else if (va > vb)
    return 1;
  else
   return 0;
}

int main( void )
{
  int a[] = {13, 0, -3, 4, 2};
  int i;
  const int LEN = sizeof(a)/sizeof(a[0]);

  qsort(a, LEN, sizeof(int), cmp_increasing);
  for (i=0; i<LEN; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
  return 0;
}
```

Internamente alla funzione `cmp_increasing()` occorre convertire i
parametri da "puntatore ad un valore generico costante" a "puntatore a
intero costante", per poi dereferenziarlo per leggere il valore
puntato. Conviene ragionare per passi:

* `a` ha tipo "puntatore ad un valore generico costante";

* `(const int *)a` ha tipo "puntatore ad un valore intero costante";

* `*(const int *)a` ha tipo "intero costante"

L'uso della parola chiave `const` è importante: l'espressione `(int
*)a` produce (come minimo) un _warning_, perché ha come valore un
puntatore _non costante_. Questo violerebbe il vincolo originale che
richiede che il valore puntato da `a` non possa essere modificato.

## Suggerimenti

La funzione `merge()` richiede un buffer temporaneo per effettuare la
fusione dei sottovettori ordinati. È possibile allocare e deallocare
il buffer all'interno di `merge()` ad ogni chiamata, ma ciò sarebbe
inefficiente. Nel codice fornito, la funzione `sort()` alloca un
buffer di lunghezza $n$ e lo passa alle funzioni `merge_sort()` e
`merge()`. Solo `merge()` utilizza il buffer, eliminando l'overhead
che sarebbe causato dalle ripetute chiamate a `malloc()` e `free()`.

## Per approfondire

Questo programma misura in modo grossolano il tempo necessario a
ordinare l'array. Considerando array di dimensioni sufficientemente
grandi (es., 100000 elementi o più), come varia (se varia...)  il
tempo di esecuzione nei seguenti casi? Le risposte dovrebbero essere
note dalla teoria.

- Input già ordinato.

- Input "quasi" ordinato (esempio, array ottenuto da un array ordinato
  in cui si scambiano tra loro due elementi in posizioni casuali $i$,
  $j$).

- Input ordinato in senso decrescente.

- Input casuale.

- Input composto da tutti valori uguali.

Per eseguire le misure occorre modificare la funzione `main()` per
generare degli input opportuni. Nel caso possano risultare utili,
vengono fornite due funzioni `randab()` e `random_shuffle()`, la cui
specifica è indicata nei commenti al codice.

## File

- [merge-sort.c](merge-sort.c)

***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/* Fonde i sottovettori ordinati `v[p..q]` e `v[q+1..r]`. Usa
   `buffer[]` come array temporaneo. `buffer[]` ha la stessa lunghezza
   dell'intero array `v[]`. */
void merge(int *v, int p, int q, int r, int *buffer)
{
    /* [TODO] */
}

/* Ordina il sottovettore buffer[p..r] (estremi inclusi) usando
   l'algoritmo Merge-Sort ricorsivo. L'array `buffer` ha lunghezzza n
   (pari all'intera lunghezza di `v[]`) ed è allocato dal chiamante;
   può essere utilizzato come buffer temporaneo per le operazioni
   `merge()`, senza necessità di allocare un nuovo buffer ad ogni
   chiamata. */
void merge_sort(int *v, int p, int r, int *buffer)
{
    /* [TODO] */
}

/* Ordina l'array v[] di lunghezza n>=0 usando Merge-Sort. L'utente
   invoca questa funzione, che a sua volta farà partire l'algoritmo
   ricorsivo implementato nella funzione `merge_sort()`. Lo scopo è di
   nascondere i dettagli implementativi di Merge-Sort, in particolare
   la necessità di passare gli indici iniziali e finali, e il buffer
   temporaneo. Questa tecnica prende il nome di "funzione
   trampolino". */
void sort(int *v, int n)
{
    int *buffer = (int*)malloc(n * sizeof(*buffer));
    assert(buffer != NULL); /* evita un warning con VS */
    merge_sort(v, 0, n-1, buffer);
    free(buffer);
}

void print_array(const int *v, int n)
{
    int i;

    printf("[ ");
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("]");
}

/* Restituisce un valore casuale compreso tra a e b (estremi inclusi) */
int randab(int a, int b)
{
    return a + rand() % (b-a+1);
}

/* Permuta il contenuto dell'array v[] in modo casuale. Per fare
   questo:

   - Si scambia v[0] con uno elemento casuale scelto in v[0..n-1].

   - Si scambia v[1] con un elemento casuale scelto in v[1..n-1].

   - ...

   - Si scambia v[i] con un elemento casuale scelto in v[i..n-1].

   - ...

   È possibile dimostrare che questo procedimento, applicato fino alla
   fine, produce una permutazione casuale (random shuffle) di v[].
*/
void random_shuffle(int *v, int n)
{
    int i;

    for (i=0; i<n-1; i++) {
        const int j = randab(i, n-1);
        const int tmp = v[i];
        v[i] = v[j];
        v[j] = tmp;
    }
}

/* Restituisce un intero < 0 se *p1 è minore di *p2 (interpretati come
   interi), 0 se sono uguali, > 0 se il primo è maggiore del
   secondo. */
int compare(const void *p1, const void *p2)
{
    const int v1 = *(const int*)p1;
    const int v2 = *(const int*)p2;
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

/* Confronta il contenuto di due array v1 e v2 di lunghezza n;
   restituisce l'indice del primo elemento il cui valore differisce
   nei due array, oppure -1 se gli array hanno lo stesso contenuto */
int compare_vec(const int *v1, const int *v2, int n)
{
    int i;
    for (i=0; i<n; i++) {
        if (v1[i] != v2[i])
            return i;
    }
    return -1;
}

/* Ordina l'array v[] di lunghezza n. Confrontiamo il risultato
   dell'ordinamento con quello prodotto dalla funzione qsort() della
   libreria standard C. Restituisce true (nonzero) se il test ha
   successo, 0 altrimenti. */
int test(int *v, int n)
{
    int result;
    int *tmp = (int*)malloc(n * sizeof(*tmp));
    clock_t tstart, elapsed;
    int diff;

    assert(tmp != NULL); /* evita un warning con VS */
    memcpy(tmp, v, n*sizeof(*v));
    qsort(tmp, n, sizeof(*tmp), compare);
    tstart = clock();
    sort(v, n);
    elapsed = clock() - tstart;
    diff = compare_vec(v, tmp, n);
    if (diff < 0) {
        printf("Test OK (%f seconds)\n", ((double)elapsed) / CLOCKS_PER_SEC);
        result = 1;
    } else {
        printf("Test FALLITO: v[%d]=%d, atteso=%d\n", diff, v[diff], tmp[diff]);
        result = 0;
    }
    free(tmp);
    return result;
}

/* ATTENZIONE: questa macro produce il valore corretto SOLO se v[] è
   un array dichiarato sullo stack (quindi NON con malloc()). La
   macro DEVE essere chiamata all'interno di un blocco in cui è stato
   dichiarato v[] */
#define ARRAY_LEN(v) (sizeof(v)/sizeof(v[0]))

int main( void )
{
    int v1[] = {0, 8, 1, 7, 2, 6, 3, 5, 4};
    int v2[] = {0, 1, 0, 6, 10, 10, 0, 0, 1, 2, 5, 10, 9, 6, 2, 3, 3, 1, 7};
    int v3[] = {-1, -3, -2};
    int v4[] = {2, 2, 2};

    test(v1, ARRAY_LEN(v1));
    test(v2, ARRAY_LEN(v2));
    test(v3, ARRAY_LEN(v3));
    test(v4, ARRAY_LEN(v4));

    return EXIT_SUCCESS;
}
