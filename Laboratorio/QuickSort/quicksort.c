/****************************************************************************
 *
 * quicksort.c -- Quicksort
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
% LabASD - Quicksort
% Nicolas Farabegoli <nicolas.farabegoli2@unibo.it>
  Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-03-07

![C. A. R. Hoare (Photograph by Rama, Wikimedia Commons, CC BY-SA 2.0 fr, <https://commons.wikimedia.org/w/index.php?curid=15568323>)](Hoare.jpg "Sir Charles Anthony Richard Hoare")

Scopo di questo esercizio è implementare l'algoritmo
_Quicksort_. Quicksort è un algoritmo di ordinamento ricorsivo di tipo
_divide et impera_ inventato da [Charles Antony Richard
Hoare](https://en.wikipedia.org/wiki/Tony_Hoare) nel 1959 e
[pubblicato nel 1961](https://doi.org/10.1093/comjnl/5.1.10).

Quicksort fa uso di una funzione `partition(v, start, end)` che
seleziona un valore $p$, detto _pivot_, appartenente al sottovettore
`v[start..end]`. Il contenuto del sottovettore viene poi permutato in
modo che gli elementi minori o uguali a $p$ si trovino a sinistra
rispetto al pivot, mentre gli elementi maggiori di $p$ si trovino a
destra (Figura 1). Al termine della funzione `partition()` il pivot si
trova quindi nella posizione corretta rispetto all'array ordinato.

![Figura 1: Risultato della funzione `partition(v, start, end)`](partition.svg)

Per completare l'ordinamento occorre eseguire ricorsivamente Quicksort
sul sottovettore che precede e segue la posizione del pivot.

La funzione `partition()` è la parte più complessa di
QuickSort. Esistono diverse strategie per realizzarla; quella
descritta nal libro è detta [schema di
Lomuto](https://en.wikipedia.org/wiki/Quicksort#Lomuto_partition_scheme)[^1].
Si sposta il pivot nell'ultima posizione del sottovettore
`v[start..end]`, e si effettua una scansione di `v[start..end-1]`
dall'estremo sinistro a quello destro usando due indici $i \leq
j$. Durante la scansione si mantengono le seguenti invarianti (vedi
Figura 2):

- $v[k] \leq p$ per ogni $\textit{start} \leq k \leq i$

- $v[k] > p$ per ogni $i+1 \leq k < j$

![Figura 2: Invariante della funzione `partition(v, start, end)` secondo lo schema di Lomuto](partition-invariant.svg)

[^1]: Personalmente trovo lo [schema di Lomuto](https://en.wikipedia.org/wiki/Quicksort#Lomuto_partition_scheme) poco intuitivo; se
      dovessi implementare Quicksort a mano, probabilmente userei lo
      [schema di
      Hoare](https://en.wikipedia.org/wiki/Quicksort#Hoare_partition_scheme)
      in cui si usano due indici $i,j$ che partono dagli estremi del
      sottovettore `v[start..end]` e procedono verso il centro; non
      appena si trovano due valori che stanno entrambi "dalla parte sbagliata",
      cioè $v[i] > p$, $v[j] < p$, si scambiano tra loro e si continua
      ad avanzare verso il centro. Si tratta di una preferenza
      strettamente personale, e probabilmente non è nemmeno la scelta
      migliore dato che [lo schema di Lomuto sembra essere più efficiente
      in pratica](https://dlang.org/blog/2020/05/14/lomutos-comeback/).

Le prestazioni di Quicksort dipendono dalla bontà del partizionamento.
Il caso ideale si ha quando il pivot si trova quasi al centro del
sottovettore; in tal caso il costo asintotico $T(n)$ soddisfa
l'equazione di ricorrenza:

$$
T(n) = \begin{cases}
c_1 & \mbox{se } n \leq 1 \\
2T(n/2) + c_2 n & \mbox{altrimenti}
\end{cases}
$$

che ha soluzione $T(n) = \Theta(n \log n)$.

Se invece il partizionamento fosse totalmente sbilanciato, in cui il
pivot fosse il minimo o il massimo di ciascun sottovettore,
l'equazione di ricorrenza diventerebbe:

$$
T(n) = \begin{cases}
c_1 & \mbox{se } n \leq 1 \\
T(n-1) + c_2 n & \mbox{altrimenti}
\end{cases}
$$

che ha soluzione $T(n) = \Theta(n^2)$. È possibile evitare il caso
pessimo scegliendo il pivot in modo casuale, oppure con strategie di
partizionamento più sofisticate.

In questa esercitazione suggerisco di iniziare scegliendo il pivot in
modo deterministico (l'algoritmo descritto nel libro di testo sceglie
il pivot del sottovettore `v[start..end]` come `v[end]`). Una volta
ottenuta una versione funzionante la si può modificare per scegliere
il pivot in modo casuale. A tale scopo viene fornita una funzione
`randab(a, b)` che restituisce un intero casuale compreso tra $a$ e
$b$, estremi inclusi.

Il programma contiene alcuni test per verificare la parziale
correttezza della funzione `quicksort()`. Nei test si confronta il
risultato con quello prodotto dalla funzione `qsort()` della libreria
standard C. La segnatura di `qsort()` è:

```C
#include <stdlib.h>

void qsort(void *base, size_t nmemb, size_t size,
           int (*compare)(const void *, const void *));
```

dove:

- `base` è un puntatore all'inizio dell'array da ordinare;

- `nmemb` è il numero di elementi dell'array da ordinare (attenzione, **non** il numero di byte!)

- `size` è la dimensione in byte di ciascuno degli elementi dell'array

- `compare` è un puntatore ad una funzione del tipo
  `int cmp(const void *a, const void *b)` che restituisce:

    + un valore negativo, se `*a` viene prima di `*b` nell'ordinamento;

    + zero, se `*a` e `*b` sono considerati uguali;

    + un valore positivo, se `*a` viene dopo `*b` nell'ordinamento

Per compilare:

        gcc -std=c90 -Wall -Wpedantic quicksort.c -o quicksort

Per eseguire in ambiente Linux/MacOSX:

        ./quicksort

Per eseguire in ambiente Windows:

        .\quicksort

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

## Per approfondire

Questo programma misura in modo grossolano il tempo necessario per ordinare
l'array. Considerando array di dimensioni sufficientemente grandi (es.,
100000 elementi o più), come varia il tempo di esecuzione nei seguenti casi?

- array di input già ordinato;

- array di input "quasi" ordinato (esempio, un array ordinato in cui
  si scambiano due elementi casuali `v[i]` e `v[j]`);

- array di input ordinato in senso decrescente;

- array di input casuale;

- array di input composto da tutti valori uguali

Per eseguire le prove occorre modificare la funzione `main()` per
generare input opportuni. Nel caso possano risultare utili, vengono
fornite due funzioni `randab()` e `random_shuffle()`, la cui specifica
è indicata nei commenti al codice.

## File

- [quicksort.c](quicksort.c)

 ***/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/* Scambia il contenuto di `v[i]` e `v[j]` */
void swap(int *v, int i, int j)
{
    const int tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

/* Applica la procedura "partition" al sottovettore v[start..end]
   (estremi inclusi). Ritorna la posizione `split` del pivot, tale che
   al termine di questa funzione si abbia:

   - v[start..split-1] <= v[split]

   - v[split+1..end] > v[split]

*/
int partition(int *v, int start, int end)
{
    /* [TODO] */
    return 0; /* Sostituire con il valore corretto */
}

/* Ordina il sottovettore `v[start..end]` (estremi inclusi) invocando
   ricorsivamente l'algoritmo Quicksort. */
void quicksort(int *v, int start, int end)
{
    /* [TODO] */
}

/* Ordina l'array `v[]` di lunghezza `n` usando l'algoritmo
   Quicksort. Questa funzione fa partire la ricorsione chiamando
   `quicksort()` con i parametri appropriati. Lo scopo di questa
   funzione è di nascondere i dettagli implementativi dell'algoritmo
   Quicksort, in particolare la necessità di passare gli indici degli
   estremi del sottovettore da ordinare. Questa tecnica prende il nome
   di "funzione trampolino". */
void sort(int *v, int n)
{
    quicksort(v, 0, n-1);
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

/* Ordina l'array v[] di lunghezza n. Confronta il risultato
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
    int v5[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    test(v1, ARRAY_LEN(v1));
    test(v2, ARRAY_LEN(v2));
    test(v3, ARRAY_LEN(v3));
    test(v4, ARRAY_LEN(v4));
    test(v5, ARRAY_LEN(v5));

    return EXIT_SUCCESS;
}
