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
% Ultimo aggiornamento: 2024-03-19

![By Willi Heidelbach, CC BY 2.5, <https://commons.wikimedia.org/w/index.php?curid=154912>](movable-type.jpg)

Scrivere un programma che legga un file di testo il cui nome viene
specificato sulla riga di comando, e determini il numero totale di
parole e il numero di parole distinte in esso contenute; non si deve
fare distinzione fra lettere maiuscole e minuscole, per cui "prova",
"Prova", "PROVA", ecc. sono la stessa parola.

Conviene leggere i caratteri di input usando la funzione

```C
int fgetc(FILE *stream)
```

dichiarata in `<stdio.h>` e convertirli in minuscolo usando

```C
int tolower(int c)
```

dichiarata in `<ctype.h>`.

Per "parola" si intende una sequenza contigua di caratteri per i quali
la funzione

```C
int isalpha(int c)
```

restituisce _true_ (cioè un valore diverso da zero). Ad esempio, la
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

Per semplificare il codice si può assumere che tutte le parole abbiano
lunghezza strettamente minore a `WORDLEN`.

Esistono diverse soluzioni; si suggerisce di salvare le parole in una
hash table, che è stata oggetto di [un altro
esercizio](hashtable.html). Se implementata correttamente in base alle
specifiche assegnate, la tabella hash è in grado di gestire un insieme
dinamico di coppie (chiave, valore) in cui le chiavi sono stringhe e i
valori sono numeri interi. Nel caso si tenti di inserire una chiave
già presente nella tabella, l'operazione viene ignorata; di
conseguenza, ogni chiave è garantita essere presente al più una sola
volta.

Per risolvere questo esercizio si può quindi procedere nel modo seguente:

- Si crea una tabella hash di dimensioni adeguate, ad esempio 50000.
  Dato che stiamo usando una tabella hash in cui le collisioni vengono
  gestite mediante liste di trabocco, è possibile inserire un numero
  qualsiasi di chiavi; la dimensione incide solo sulle prestazioni
  delle operazioni.

- Si legge dal file la parola successiva.

- Si convertono tutti i caratteri della parola in minuscolo usando la
  funzione `int tolower(int c)`.

- Si usa la funzione `ht_insert()` per inserisce nella tabella hash la
  coppia (_parola_, 0) (si usa la parola come chiave, e si associa ad
  essa un valore qualsiasi). Infatti, vogliamo solo memorizzare le
  chiavi evitando duplicazioni, cosa che viene fatta in automatico
  dalla tabella hash.

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

Si presti attenzione ad una particolarità del linguaggio C. Ai
compilatori è lasciata facoltà di trattare il tipo `char` (senza
ulteriori specificazioni) come `signed` o `unsigned`. In alcuni
sistemi (tra cui il mio) viene usato il tipo `signed`; questo
significa che il seguente frammento di codice:

```C
char c; \/\* SBAGLIATO \*\/
FILE *filein = fopen(...);
...
c = fgetc(filein);
if (isalpha(c)) { ... }
```

potrebbe non funzionare correttamente in presenza di lettere
accentate, perché queste hanno un codice ASCII superiore a 127. Ad
esempio, se alla variabile `c` fosse assegnato il carattere `'è'`
(codice ASCII 195), il valore memorizzato in `c` è -61, dato che 195
non può essere rappresentato con 8 bit in complemento a due. Di
conseguenza la funzione `isspace(c)` riceverebbe un valore negativo
producendo un risultato indefinito.

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


/* Legge la prossima parola dal file `f` e la memorizza bel buffer `s`
   di lunghezza `WORDLEN`; `s` deve essere già stato allocato dal
   chiamante. Restituisce il numero di caratteri letti; 0 indica che
   si non è stato letto alcun carattere, cioè si è raggiunta la fine
   del file. */
int read_word(FILE *f, char *s)
{
    /* [TODO] */
    s[0] = '\0';
    return 0;
}

int main( int argc, char *argv[] )
{
    FILE *filein = stdin;
    char w[WORDLEN]; /* buffer per lettura delle parole */

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

    /* TODO: una volta completata la funzione read_word(), questo
       ciclo si limita a leggere e stampare le parole lette man mano
       dal file. Lo si modifichi per inserire le parole lette in una
       tabella hash e al termine stampare il numero di elementi della
       tabella chiamando la funzione `ht_round()`. */
    while (read_word(filein, w)) {
        printf("%s\n", w);
    }
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
