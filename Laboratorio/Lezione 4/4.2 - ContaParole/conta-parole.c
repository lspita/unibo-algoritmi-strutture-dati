/****************************************************************************
 *
 * conta-parole.c -- Conta il numero di parole distinte
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
% LabASD - Conta il numero di parole distinte
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-03-22

![By Willi Heidelbach, CC BY 2.5, <https://commons.wikimedia.org/w/index.php?curid=154912>](movable-type.jpg)

Scrivere un programma che legga un file di testo il cui nome viene
specificato sulla riga di comando, e determini il numero di parole e
il numero di parole distinte in esso contenute; non si deve fare
distinzione fra lettere maiuscole e minuscole, per cui "prova",
"Prova", "PROVA", ecc. sono la stessa parola.

Per "parola" si intende una sequenza contigua di caratteri per i quali
la funzione

```C
int isalpha(int c)
```

restituisce _true_ (cioè un valore diverso da zero); _c_ è il codice
ASCII del carattere letto. Ad esempio, la
stringa _"i diritti inviolabili dell'uomo"_ è composta da cinque
parole: _"i"_, _"diritti"_, _"inviolabili"_, _"dell"_ e _"uomo"_.

Quindi, nel testo

> La Repubblica riconosce e garantisce i diritti inviolabili
> dell'uomo, sia come singolo, sia nelle formazioni sociali ove si
> svolge la sua personalita', e richiede l'adempimento dei doveri
> inderogabili di solidarieta' politica, economica e sociale.

sono presenti le 36 parole seguenti:

```
la
repubblica
riconosce
e
garantisce
i
diritti
inviolabili
dell
uomo
sia
come
singolo
sia
nelle
formazioni
sociali
ove
si
svolge
la
sua
personalita
e
richiede
l
adempimento
dei
doveri
inderogabili
di
solidarieta
politica
economica
e
sociale
```

di cui 32 sono distinte.

Conviene leggere le parole un carattere alla volta usando la funzione

```C
int fgetc(FILE *stream)
```

dichiarata in `<stdio.h>`; la funzione restituisce il codice ASCII del
prossimo carattere presente nel file `stream`, oppure `EOF` in caso di
fine file; `EOF` è una costante definita in `<stdio.h>`.

Per convertire un carattere in minuscolo si può usare la funzione

```C
int tolower(int c)
```

dichiarata in `<ctype.h>`. La funzione accetta come parametro il
codice ASCII _c_ di un simbolo, e restituisce il codice ASCII del
corrispondente carattere minuscolo se _c_ è una lettera dell'alfabeto,
oppure restituisce _c_ se si tratta di un altro simbolo.

Si può assumere che tutte le parole abbiano lunghezza strettamente
minore a `WORDLEN`.

Per risolvere il problema si possono salvare le parole come chiavi di
una hash table, che è stata oggetto di [un altro
esercizio](hashtable.html). Si consiglia di procedere come segue:

- Si crea una tabella hash di dimensioni adeguate, ad esempio 10000.
  Dato che stiamo usando una tabella hash in cui le collisioni vengono
  gestite mediante liste di trabocco, è possibile inserire un numero
  qualsiasi di coppie; la dimensione incide solo sulle prestazioni
  delle operazioni.

- Si legge dal file la parola successiva usando una funzione
  `read_word()` (da completare), convertendo tutti i caratteri in
  minuscolo.

- Si usa la funzione `ht_insert()` per inserisce nella tabella hash la
  coppia (_w_, 0), dove _w_ è la parola letta al punto precedente.  La
  chiave associata a _w_ è posta arbitrariamente a zero, dato che non
  ci interessa per la soluzione del problema.

- Quando si arriva alla fine del file, si stampa il numero di coppie
  (chiave, valore) presenti nella tabella hash usando la funzione
  `ht_count()`.

Per compilare:

        gcc -std=c90 -Wall -Wpedantic hashtable.c conta-parole.c -o conta-parole

Per eseguire in ambiente Linux/MacOSX:

        ./conta-parole nome_file_input

Per eseguire in ambiente Windows:

        .\conta-parole nome_file_input

## Alcuni punti di attenzione

Si presti attenzione che il linguaggio C non specifica se il tipo
`char` dabba essere _signed_ o _unsigned_. In altre parole, la
dichiarazione `char c;` lascia libertà ai compilatori di decidere se
`c` rappresenta un valore con segno o senza segno. Nel caso si
desideri uno dei due occorre specificarlo esplicitamente (es.,
`unsigned char c` definisce `c` come _unsigned_).

In alcuni sistemi (tra cui il mio) viene usato il tipo `signed`;
questo significa che il seguente frammento di codice:

```C
char c; \/\* SBAGLIATO \*\/
FILE *filein = fopen(...);
...
c = fgetc(filein);
if (isalpha(c)) { ... }
```

non è corretto in presenza di lettere accentate, perché queste hanno
un codice ASCII superiore a 127. Ad esempio, se alla variabile `c`
fosse assegnato il carattere `'è'` (codice ASCII 195), il valore
memorizzato in `c` sarebbe -61, dato che 195 non può essere
rappresentato con 8 bit in complemento a due. Di conseguenza la
funzione `isalpha(c)` riceverebbe un valore negativo producendo un
risultato indefinito.

La soluzione consiste nel dichiarare `c` di tipo `int`:

```C
int c; \/\* Corretto \*\/
FILE *filein = fopen(...);
...
c = fgetc(filein);
if (isalpha(c)) { ... }
```

Si noti che _non_ sarebbe corretto definire `c` di tipo `unsigned
char`, perché in caso di fine file, la funzione `fetc()` restituisce
`EOF` che normalmente ha valore -1.

## File

- [conta-parole.c](conta-parole.c)
- [the-time-machine.txt](the-time-machine.txt) ([Fonte](http://www.gutenberg.org/files/35/35-0.txt)) (32886 parole, di cui 4593 distinte)
- [war-and-peace.txt](war-and-peace.txt) ([Fonte](http://www.gutenberg.org/files/2600/2600-0.txt))
- [the-hound-of-the-baskervilles.txt](the-hound-of-the-baskervilles.txt) ([Fonte](http://www.gutenberg.org/files/2852/2852-0.txt))
- [the-war-of-the-worlds.txt](the-war-of-the-worlds.txt) ([Fonte](http://www.gutenberg.org/files/36/36-0.txt))
- [la-divina-commedia.txt](la-divina-commedia.txt) ([Fonte](https://www.gutenberg.org/ebooks/1012.txt.utf-8))

***/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "hashtable.h"

#define WORDLEN 100

/*

Questo problema si può risolvere con la seguente riga di comando in
ambiente Linux:

cat war-and-peace.txt | tr -c " \nA-Za-z" " " | tr " " "\n" | sed '/^$/d' | tr A-Z a-z | sort | uniq | wc -l

La catena di comandi sopra si chiama _pipeline_, ed è costituita da
una sequenza di comandi in cui l'output di ciascuno viene usate come
input dal comando successivo.  Il significato delle varie parti è il
seguente:

- cat war-and-peace.txt stampa a video il contenuto del file
  `war-and-peace.txt`

- tr -c " \nA-Za-z" " " rimpiazza tutti i caratteri diversi
  dalle lettere, spazio e ritorno a capo con degli spazi.

- tr " " "\n" rimpiazza tutti gli spazi con dei ritorni a capo,
  in modo che ogni riga contenga una parola (possono tuttavia
  essere presenti righe vuote).

- sed '/^$/d' rimuove le righe vuote.

- tr A-Z a-z trasforma tutte le lettere maiuscole in minuscole

- sort ordina le righe (ogni riga contiene una parola)

- uniq compatta tutte le righe adiacenti uguali

- wc -l conte il numero di righe; poiché ogni riga contiene una parola,
  il valore stampato è il numero di parole.

*/

/* Legge la prossima parola dal file `f` e la memorizza bel buffer `s`
   di lunghezza `WORDLEN`; `s` deve essere già stato allocato dal
   chiamante. Restituisce il numero di caratteri letti; 0 indica che
   si non è stato letto alcun carattere, cioè si è raggiunta la fine
   del file. */
int read_word(FILE *f, char *s)
{
    int i = 0;
    int c;

    /* Salta gli eventuali caratteri non alfabetici */
    do {
        c = fgetc(f);
    } while ((c != EOF) && !isalpha(c));
    /* Legge la parola; se siamo usciti dal ciclo precedente perché il
       file è terminato (c == EOF), il blocco seguente viene
       correttamente saltato. */
    while ((c != EOF) && isalpha(c) && (i < WORDLEN-1)) {
        s[i] = tolower(c);
        i++;
        c = fgetc(f);
    }
    s[i] = '\0';
    return i;
}

int main( int argc, char *argv[] )
{
    FILE *filein = stdin;
    char w[WORDLEN]; /* buffer per lettura delle parole */
    int np = 0; /* numero parole */
    HashTable *h;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s inputfile\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-") != 0) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    h = ht_create(10000); /* la dimensione della tabella è scelta arbitrariamente */
    while (read_word(filein, w)) {
        ht_insert(h, w, 1);
        printf("%s\n", w);
        np++;
    }
    printf("%d parole, di cui %d distinte\n", np, ht_count(h));
    ht_destroy(h);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
