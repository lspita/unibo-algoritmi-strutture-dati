/****************************************************************************
 *
 * resto-greedy.c -- Il problema del resto limitato greedy
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
% LabASD - Il problema del resto limitato _greedy_
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-04-12

![](Euro_coins_and_banknotes.jpg "Euro in monete e banconote")

Il _problema del resto_ (_change-making problem_) è definito nel modo
seguente: date $t$ denominazioni (tagli) di monete o banconote $T_0,
\ldots, T_{t-1}$, in cui disponiamo di infiniti pezzi di ciascun
taglio, determinare il minimo numero di pezzi da utilizzare per
erogare un resto $R$. Nel caso dell'Euro, i tagli (espressi in
centesimi) sono 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000,
10000, 20000, 50000 (da un centesimo fino a 500€).

Il problema del resto può essere risolto usando una strategia
_greedy_: si utilizza il _massimo_ taglio minore o uguale al residuo
da erogare. Se alla fine del procedimento il residuo è zero, il resto
è erogabile e il numero di pezzi utilizzati è il minimo possibile.

Ad esempio, dovendo erogare un resto di $R=96$ centesimi di Euro,
l'algoritmo greedy utilizzerebbe pezzi da 50, 20, 20, 5, 1 per un
totale di 5 pezzi, che è il minimo numero di pezzi considerando tutti
i possibili tagli di euro a disposizione.

Sistemi monetari per i quali l'algoritmo greedy produce la soluzione
ottima sono detti [sistemi monetari
canonici](https://arxiv.org/pdf/0809.0400); quasi tutti i sistemi
monetari in uso sono canonici.

In questo esercizio applichiamo l'algoritmo greedy ad un problema
leggermente diverso, che chiamiamo _problema del resto limitato_, per
il quale **l'algoritmo greedy potrebbe non produrre la soluzione
ottima**, e potrebbe addirittura non trovare una soluzione anche
quando questa esiste.

Il problema del resto limitato è il seguente: disponiamo di $n$ monete
(o banconote) di valori interi strettamente positivi $m_0, \ldots,
m_{n-1}$; i valori non sono necessariamente ordinati, e possono
esistere più monete con lo stesso valore. **Ogni moneta/banconota può
essere usata al più una volta**. Determinare il minimo numero di
monete, tra quelle a disposizione, che è necessario usare per erogare
un dato resto $R \geq 0$.

Applichiamo l'algoritmo greedy nel modo seguente: ordiniamo le monete
in ordine _decrescente_ di valore, e consideriamo le monete una a una
nella lista ordinata. Se il residuo da erogare è maggiore o uguale al
valore della moneta che stiamo considerando, allora la usiamo,
altrimenti la scartiamo e passiamo alla successiva.

Ad esempio, supponiamo di avere le monete seguenti (che per comodità
vengono riportate già in ordine decrescente di valore):

50, 50, 20, 10, 10, 10, 5, 2, 1

e supponiamo di dover erogare l'importo $R=96$. La Tabella 1 mostra il
comportamento dell'algoritmo greedy: la colonna "Residuo" indica il
resto ancora da erogare; la colonna "Moneta" indica il valore della
moneta che stiamo considerando; infine, la colonna "Usata?" indica se
usiamo la moneta o la scartiamo.

: Tabella 1: Esempio di esecuzione dell'algoritmo greedy del resto

Residuo  Moneta   Usata?
-------- -------- --------
96       50       sì
46       50       no
46       20       sì
26       10       sì
16       10       sì
6        10       no
6        5        sì
1        2        no
1        1        sì
0        stop
-------- -------- --------

Quindi, l'algoritmo usa le sei monete di valore 50, 20, 10, 10, 5, 1.

L'algoritmo greedy potrebbe fallire in certi casi: ad esempio, se le
monete a disposizione fossero $5, 2, 2, 2, 2, 2$ e dovessimo erogare
un resto $R=10$, l'algoritmo utilizzerebbe la moneta da $5$ e non
sarebbe poi in grado di continuare, concludendo (erroneamente) che il
resto non è erogabile. In realtà l'importo $R=10$ può essere erogato
usando le cinque monete da $2$ [^1].

[^1]: La soluzione ottima può essere trovata usando un algoritmo più
      complesso basato sulla programmazione dinamica. L'algoritmo
      basato su programmazione dinamica funziona anche con sistemi
      monetari non canonici.

Il programma legge i parametri di input da un file il cui nome va
specificato sulla riga di comando. Il file ha il seguente formato:

```
R n
m[0]
...
m[n-1]
```

dove $R$ è il resto da erogare (intero maggiore o uguale a zero), $n$
indica il numero di monete a disposizione (intero maggiore di zero), e
`m[0..n-1]` sono i valori delle $n$ monete a disposizione (interi
strettamente maggiori di zero).

I valori delle monete non sono necessariamente ordinati. Per ordinarli
si può usare un algoritmo di ordinamento tra quelli visti a lezione,
oppure si può usare la funzione `qsort()` dichiarata in `<stdlib.h>`,
la cui segnatura è:

```C
#include <stdlib.h>

void qsort(void *base, size_t nmemb, size_t size,
           int (*compare)(const void *, const void *));
```

dove:

- `base` è un puntatore all'inizio dell'array da ordinare;

- `nmemb` è il numero di elementi dell'array da ordinare (attenzione,
  non il numero di byte!)

- `size` è la dimensione in byte di ciascuno degli elementi dell'array

- `compare` è un puntatore ad una funzione del tipo
  `int cmp(const void *a, const void *b)` che restituisce:

    + un valore negativo, se `a` viene prima di `b` nell'ordinamento;

    + zero, se `a` e `b` sono considerati uguali;

    + un valore positivo, se `a` viene dopo `b` nell'ordinamento

Occorre prestare attenzione alla funzione di confronto: dovendo
ordinare le monete in senso _decrescente_, la funzione di confronto
deve restituire un valore negativo se il primo parametro è
**maggiore** del secondo, un valore positivo se il primo parametro è
**minore** del secondo, e zero se sono uguali. Abbiamo già discusso
l'uso della funzione `qsort()` in un [altro
esercizio](quicksort.html).

Per compilare:

        gcc -std=c90 -Wall -Wpedantic resto-greedy.c -o resto-greedy

Per eseguire in ambiente Linux/MacOSX:

        ./resto-greedy resto-greedy.in

Per esguire in ambiente Windows:

        .\resto-greedy resto-greedy.in

:Esempi

+---------+------------------------------------------------------+
|Input    |Output                                                |
+=========+======================================================+
|```      |```                                                   |
|13 8     |Resto 13 erogabile con 3 monete:                      |
|20       |10 2 1                                                |
|20       |```                                                   |
|20       |                                                      |
|10       |                                                      |
|5        |                                                      |
|2        |                                                      |
|1        |                                                      |
|1        |                                                      |
|```      |                                                      |
+---------+------------------------------------------------------+
|```      |```                                                   |
|27 4     |Resto 27 non erogabile con le monete a disposizione   |
|50       |```                                                   |
|1        |                                                      |
|5        |                                                      |
|20       |                                                      |
|```      |                                                      |
+---------+------------------------------------------------------+
|```      |```                                                   |
|10 6     |Resto 10 non erogabile con le monete a disposizione   |
|5        |```                                                   |
|2        |(si noti che non sarebbe la soluzione corretta,       |
|2        |perché il resto è erogabile usando le cinque monete   |
|2        |di valore 2; è tuttavia la soluzione corretta         |
|2        |calcolata dall'algoritmo greedy da implementare).     |
|2        |                                                      |
|```      |                                                      |
+---------+------------------------------------------------------+

## File

- [resto-greedy.c](resto-greedy.c)
- [resto-greedy.in](resto-greedy.in) ([risultato atteso](resto-greedy.out))
- [resto-greedy1.in](resto-greedy1.in)
- [resto-greedy2.in](resto-greedy2.in)
- [resto-greedy3.in](resto-greedy3.in)
- [resto-greedy4.in](resto-greedy4.in)

***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Dobbiamo ordinare le monete in senso DECRESCENTE, quindi questa
   funzione deve restituire un valore negativo se m1 > m2 */
int confronta_monete(const void *m1, const void *m2)
{
    const int v1 = *(const int*)m1;
    const int v2 = *(const int*)m2;
    if (v1 > v2)
        return -1;
    else if (v1 < v2)
        return 1;
    else
        return 0;
}

/* Date n monete i cui valori sono memorizzati nell'array m[] (non
   necessariamente ordinato), restituisce il minimo numero di pezzi
   necessari per erogare un resto R. Se il resto non è erogabile
   restituisce -1. La funzione deve anche stampare quali monete usare,
   in caso di resto erogabile, oppure un opportuno messaggio che
   avvisa che il resto non è erogabile. */
int resto(int R, int m[], int n)
{
    int *use = (int*)malloc(n * sizeof(*use));
    int n_coins = 0; /* numero di monete utilizzate */
    int i = 0;
    int da_erogare = R;

    assert(use != NULL);

    /* ordiniamo le monete in senso decrescente */
    qsort(m, n, sizeof(*m), confronta_monete);

    for (i=0; i<n; i++) {
        use[i] = 0;
    }

    for (i=0; i<n; i++) {
        if (da_erogare >= m[i]) {
            da_erogare -= m[i];
            use[i] = 1;
            n_coins++;
        }
    }

    if (da_erogare > 0) {
        printf("Resto %d non erogabile con le monete a disposizione\n", R);
        n_coins = -1;
    } else {
        /* come ulteriore controllo, non richiesto dall'esercizio e
           tecnicamente inutile, ci assicuriamo che il numero di
           monete stampate sia esattamente uguale a `n_coins`. È
           consigliabile prudenza finché non si è ragionevolmente
           convinti che il programma sia corretto. */
        int n_printed = 0;
        printf("Resto %d erogabile con %d monete:\n", R, n_coins);
        for (i=0; i<n; i++) {
            if (use[i]) {
                printf("%d ", m[i]);
                n_printed++;
            }
        }
        printf("\n");
        assert(n_coins == n_printed);
    }
    free(use);
    return n_coins;
}

int main( int argc, char *argv[] )
{
    int R, n, i;
    int *m;
    FILE *filein = stdin;

    if (argc != 2) {
        fprintf(stderr, "Invocare il programma con: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-") != 0) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    fscanf(filein, "%d %d", &R, &n);
    m = (int*)malloc(n * sizeof(*m));
    assert(m != NULL);
    for (i=0; i<n; i++) {
        if (1 != fscanf(filein, "%d", &m[i])) {
            fprintf(stderr, "Errore durante la lettura della moneta %d di %d\n", i+1, n);
            return EXIT_FAILURE;
        }
    }

    resto(R, m, n);
    free(m);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
