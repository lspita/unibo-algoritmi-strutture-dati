/****************************************************************************
 *
 * telefono.c -- Numero di telefono ripetuto
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
% LabASD - Numero di telefono ripetuto
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-01-23

![Tomasz Sienicki (Own work), CC BY 3.0, <https://commons.wikimedia.org/w/index.php?curid=10330603>](phone-book.jpg "Elenco telefonico")

Siete stati assunti come consulenti dalla IATM S.p.A. (Importante
Azienda di Telefonia Mobile) che è alle prese con il problema
seguente. L'azienda ha un file contenente $n$ numeri di telefono,
$200000 < n \leq \mathtt{MAXN}$. Tutti i numeri hanno sei cifre, la
prima delle quali può essere `3` oppure `5`; i numeri di telefono
hanno quindi la forma `3xxxxx` oppure `5xxxxx`, dove le `x` sono cifre
numeriche arbitrarie. Il contenuto del file non è necessariamente
ordinato.

Da quanto detto sopra si può dedurre che ci sia sicuramente almeno un
numero di telefono che compare più di una volta, perché il file
contiene più elementi di quanti siano i possibili numeri distinti. Il
vostro compito è di implementare un algoritmo efficiente che stampi a
video **il più piccolo numero di telefono che compare più di una
volta** nel file di input.

Il file di input ha $n$ righe, ciascuna delle quali contenente un
numero di telefono (si consideri [questo esempio](telefono.in)). Il
programma riceve il nome del file di input come unico parametro sulla
riga di comando, e deve stampare a video il minimo numero ripetuto
presente nel file.

Per compilare:

        gcc -std=c90 -Wall -Wpedantic telefono.c -o telefono

Per eseguire in ambiente Linux/MacOSX:

        ./telefono nome_file

Per eseguire in ambiente Windows:

        .\telefono nome_file

dove `nome_file` è il nome del file di input contenente l'elenco dei
numeri di telefono.

## Per approfondire

Un aspetto complicato di questo esercizio è la generazione dei file di
input (che non è richiesta, ma che ho dovuto fare io per preparare gli
input). Dato un intero $n > 200000$ e un numero di telefono $t$, come
fareste per generare un file di input composto da $n$ numeri di
telefono come sopra, non necessariamente in ordine, in cui il minimo
valore ripetuto sia esattamente $t$?

## File

- [telefono.c](telefono.c)
- [telefono.in](telefono.in) ([output atteso](telefono.out))
- [telefono1.in](telefono1.in)
- [telefono2.in](telefono2.in)
- [telefono3.in](telefono3.in)
- [telefono4.in](telefono4.in)

***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* MAXN è il massimo numero di valori che possono essere presenti nel
   file. Questo parametro si può considerare fissato, ma il programma
   dovrebbe funzionare con qualsiasi valore di MAXN */
#define MAXN 1000000

/*

Usando sistemi Unix il problema può essere risolto dalla riga di
comando senza scrivere codice, sebbene in modo meno efficiente di
quanto fatto in questo programma.

La riga di comando è la seguente

        cat telefono.in | sort -n | uniq -d | head -1

Una breve spiegazione:

- L'operatore `|` (_pipe_) rende l'output (cioè quello che viene
  stampato a video) del comando precedente nell'input del comando
  successivo, come se fosse digitato a tastiera;

- `cat telefono.in` legge il file `telefono.in` e ne copia il contenuto
  sullo standard output;

- `sort -n` ordina le righe in input in base al loro valore numerico;

- `uniq -d` stampa le righe corrispondenti ai valori duplicati (ogni
  valore duplicato viene stampato una sola volta). Dato che l'input è
  ordinato, anche i valori duplicati sono stampati in ordine
  crescente;

- `head -1` stampa solo la prima riga, che corrisponde al minimo
  numero di telefono che compare almeno due volte.

*/

/* N indica quanti sono i possibili numeri di telefono distinti. Nel
   nostro caso ci sono 200000 numeri di telefono distinti di 6 cifre
   la cui prima cifra sia 3 oppure 5. */
#define N 200000

/* IDEA della soluzione:

   Per iniziare, supponiamo che i numeri di telefono appartengano
   all'insieme {000000, ... 199999}. In questo caso possiamo usare un
   array occ[] di N=200000 interi, inizialmente tutti zero, mantenendo
   la seguente invariante:

   "occ[t] è il numero di volte in cui il numero di telefono `t`
   compare nell'input."

   L'array occ[] può essere riempito in tempo Theta(n) durante la
   lettura del file. Fatto questo, si scorre nuovamente l'array
   cercando il minimo indice t per cui si abbia occ[t] > 1. Tale
   indice t rappresenta il numero di telefono cercato.

   Nel nostro caso però i numeri di telefono iniziano per 3 oppure
   5. Questo complica le cose, ma l'idea rimane valida. Le occorrenze
   dei numeri di telefono che iniziano con 3 vengono memorizzate in
   occ[0..99999], mentre quelle dei numeri che iniziano con 5 vengono
   memorizzate in occ[100000..199999]:

   Per facilitare il passaggio tra numeri di telefono e indici
   dell'array (e viceversa), definiamo due funzioni ausiliarie
   `ind2tel()` e `tel2ind()` (vedi sotto).
*/

/* Converte un indice `i` in [0..N-1] in un numero di telefono. Si
   assume che la prima metà degli indici corrispondano ai numeri di
   telefono che iniziano per 3, mentre la seconda metà ai numeri che
   iniziano per 5. */
int ind2tel(int i)
{
    assert((i >= 0) && (i < N));
    return (i<N/2 ? 300000 + i : 500000 + i - N/2);
}

/* Converte un numero di telefono `t` in un indice intero in [0..N-1].
   I numeri di telefono che iniziano per 3 vengono mappati sulla prima
   metà degli indici, mentre quelli che iniziano per 5 vengono mappati
   sulla seconda metà. */
int tel2ind(int t)
{
    const int c = t/100000; /* prima cifra */
    const int r = t%100000; /* resto del numero */

    assert((c == 3) || (c == 5));

    if (c == 3)
        return r;
    else
        return N/2 + r;
}

/* Dato un puntatore `fin` ad un file con il contenuto come da
   specifica, restituisce il minimo numero di telefono duplicato. */
int min_duplicate(FILE *fin)
{
    int i, t;
    int *occ; /* occ[i] è il numero di occorrenze del numero di
                 telefono codificato dall'indice i, cioè del numero di
                 telefono ind2tel(i) */

    occ = (int*)malloc(N * sizeof(*occ));
    assert(occ != NULL);

    /* Ricordiamo che in linguaggio C il contenuto della memoria
       restituita da malloc() è _INDEFINITO_. Occorre quindi
       inizializzarlo esplicitamente. */
    for (i=0; i<N; i++) {
        occ[i] = 0;
    }

    i = 0;
    while (1 == fscanf(fin, "%d", &t)) {
        occ[ tel2ind(t) ] += 1;
        i++;
    }
    assert(i <= MAXN); /* non serve, perché questa funzione calcola il
                          risultato corretto qualunque sia la
                          dimensione del file. */

    for (i=0; (i < N) && (occ[i] <= 1); i++)
        ; /* corpo vuoto */

    /* Al termine del ciclo precedente si ha che `i` rappresenta il
       più piccolo indice nell'array delle occorrenze in cui si abbia
       occ[i] > 1 (si poteva usare anche un ciclo while()).

       Per ipotesi sappiamo che tale indice esiste sempre, per cui il
       test (i < N) sarebbe superfluo; tuttavia è opportuno lasciarlo
       a scopo difensivo (vedi dispensa), nel caso in cui questo
       programma venga applicato ad un input non valido (cosa che
       comunque è garantita non accadere mai in questo esercizio). */

    /* Per quanto detto sopra l'asserzione che segue è sicuramente
       sempre vera. */
    assert(i<N);

    free(occ);
    return ind2tel(i);
}

/**
 ** Le funzioni che seguono servono per generare l'input; non sono
 ** quindi richieste per la soluzione dell'esercizio.
 **/

/* Restituisce un intero casuale appartenente all'intervallo [a,b] */
int randab(int a, int b)
{
    return a + rand() % (b-a+1);
}

/* Permuta in modo casuale il contenuto dell'array v[]. Questa
   operazione viene eseguita come segue:

   - al primo passo si scambia v[0] con un elemento scelto casualmente
     in v[0..n-1]. Si noti che v[0] potrebbe essere scambiato con se
     stesso; questo è voluto.

   - al secondo passo si scambia v[1] con un elemento scelto
     casualmente in v[1..n-1];

   - ...

   - all'i-esimo passo si scambia v[i-1] con un elemento scelto
     casualmente in v[i-1..n-1]

   È possibile dimostrare che questo procedimento, applicato fino alla fine,
   produce un "rimescolamento casuale" (random shuffle) di v[].

   Riferimenti: Jon Bentley, "Programming Pearls" 2nd edition, 2000,
   ISBN 9780201657883, column 12.
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

/* Stampa `n` numeri di telefono (n>N) tali che `t` sia il minimo
   numero di telefono che compare almeno due volte. `t` deve avere sei
   cifre e iniziare per 3 oppure 5. */
void inputgen(int n, int t)
{
    int i;
    int *tel; /* array dei numeri di telefono */

    assert(n > N);
    assert((t / 100000 == 3) || (t / 100000 == 5));
    tel = (int*)malloc(n * sizeof(*tel));
    assert(tel != NULL);
    /* Riempiamo una porzione iniziale di v[] con numeri di telefono,
       senza inserire duplicati */
    for (i=0; i<N/2; i++) {
        tel[i] = ind2tel(2*i);
    }
    /* Assicuriamoci che t compaia almeno due volte */
    tel[i] = t; i++;
    tel[i] = t; i++;
    /* Riempiamo il resto dell'array con numeri di telefono casuali
       maggiori o uguali a t (dato che t è un numero di telefono,
       dobbiamo convertirlo in un indice usando tel2ind() ) */
    for (; i<n; i++) {
        const int tmp = randab(tel2ind(t), N-1);
        tel[i] = ind2tel(tmp);
    }
    random_shuffle(tel, n);
    for (i=0; i<n; i++) {
        printf("%d\n", tel[i]);
    }
    free(tel);
}

int main( int argc, char *argv[] )
{
    FILE *fin = stdin;

    /* Se il programma viene eseguito con un parametro sulla riga di
       comando, legge l'input da file:

       ./telefono telefono1.in

       (se il nome del file è '-', legge la sequenza di numeri di
       telefono da standard input).

       Se sono presenti due parametri

       ./telefono n t

       stampa una sequenza di `n` numeri di telefono in cui `t` è il
       minimo numero di telefono che verrà ripetuto almeno due
       volte. */
    if ((argc < 2) || (argc > 3)) {
        fprintf(stderr, "Usage: %s inputfile, or %s n t\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        inputgen( atoi(argv[1]), atoi(argv[2]) );
    } else {
        if (strcmp(argv[1], "-") != 0) {
            fin = fopen(argv[1], "r");
            if (fin == NULL) {
                fprintf(stderr, "Can not open \"%s\"\n", argv[1]);
                return EXIT_FAILURE;
            }
        }

        printf("%d\n", min_duplicate(fin));

        if (fin != stdin) fclose(fin);
    }
    return EXIT_SUCCESS;
}
