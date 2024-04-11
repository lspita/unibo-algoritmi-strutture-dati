/****************************************************************************
 *
 * formatter.c -- Formattazione di testo
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
% LabASD - Formattazione di testo
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-04-10

![By NYC Wanderer (Kevin Eng) - originally posted to Flickr as Gutenberg Bible, CC BY-SA 2.0, <https://commons.wikimedia.org/w/index.php?curid=9914015>](libro.jpg "Libro aperto")

Scrivere un programma che legga un file di testo e stampi a video la
sequenza di parole che lo compongono in modo che ogni riga abbia
lunghezza massima di `Lmax` caratteri. Il valore di `Lmax` e il nome
del file di testo vengono passati come parametri sulla riga di
comando.

Per "parola" si intende una sottosequenza massimale di caratteri per
cui la funzione `isspace(c)` restituisca zero. Ciò significa che
consideriamo "parola" qualunque sequenza di caratteri diversi da
spazi, ritorno a capo, tabulazioni e simili[^1].

[^1]: Ciò è leggermente diverso da quanto abbiamo visto [in altri
      esercizi](conta-parole.html), in cui consideravamo parole solo
      le sequenze di lettere alfabetiche. Di conseguenza, chi vuole
      riutilizzare le funzioni usate in precedenza dovrà adattarle
      alla nuova definizione di "parola".

Il programma deve stampare le parole a video, separando ogni parola
dalla successiva con un _singolo_ spazio; gli eventuali "a capo"
nell'input devono essere trattati come se fossero spazi. La lunghezza
di una riga di output è data dalla somma delle lunghezze delle parole
e degli spazi che le separano, e deve essere minore o uguale a
`Lmax`. Si può assumere che le parole abbiano sempre lunghezza minore
di `WORDLEN`.  Si può inoltre assumere che il valore `Lmax` sia sempre
scelto in modo da essere maggiore o uguale della massima lunghezza
delle parole nel testo.

Ad esempio, il comando

        ./formatter 65 divina-commedia-canto1.in

produrrà un risultato le cui ultime cinque righe sono:

```
ivi elegge!>>. E io a lui: <<Poeta, io ti richeggio per quello
Dio che tu non conoscesti, a cio ch'io fugga questo male e
peggio, che tu mi meni la dov' or dicesti, si ch'io veggia la
porta di san Pietro e color cui tu fai cotanto mesti>>. Allor si
mosse, e io li tenni dietro.
```

Può essere utile leggere il prossimo carattere dal file usando la
funzione `int fgetc(FILE*)`, definita in `<stdio.h>`; la funzione
restituisce il codice ASCII del carattere successivo presente nel
file, oppure il valore `EOF` se si è raggiunta la fine del file.

Per riconoscere gli spazi si usi la funzione `int isspace(int c)`
definita in `<ctype.h>`, che ritorna _true_ (un intero diverso da
zero) se e solo se il carattere di codice ASCII `c` rappresenta uno
spazio o simile; sono considerati spazi anche i caratteri _form-feed_
`'\f'`, _newline_ `'\n'`, _carriage return_ `'\r'`, tabulazione
`'\t'`, e tabulazione verticale `'\v'`.

Per compilare:

        gcc -std=c90 -Wall -Wpedantic formatter.c -o formatter

Per eseguire in ambiente Linux/MacOSX:

        ./formatter 65 divina-commedia.in

Per eseguire in ambiente Windows:

        .\formatter 65 divina-commedia.in

## Suggerimenti

Questo esercizio può essere risolto mediante una strategia _greedy_,
aggiungendo parole alla riga corrente fin quando possibile.

Assicurarsi che il programma gestisca correttamente i casi
particolari; ad esempio:

- Il programma funziona correttamente se l'input è un file vuoto, o
  contenente solo spazi?

- Il programma funziona correttamente se l'input contiene una singola
  parola, senza spazi né prima né dopo?

- Il programma stampa correttamente la prima/l'ultima riga del file di
  input?

## Curiosità

Il programma da realizzare è una versione molto semplificata di
[fmt](https://en.wikipedia.org/wiki/Fmt_(Unix)). `fmt` formatta i
paragrafi di un file di testo in modo che ogni riga abbia lunghezza
minore o uguale a un valore definito dall'utente. Su distribuzioni
Linux basate su Debian, questo comando fa parte del pacchetto
`coreutils`. Si noti che il programma da realizzare si comporta in
modo diverso da `fmt`, perché deve compattare una sequenza di spazi
adiacenti nell'input in un singolo spazio in output, e deve trattare
gli "a capo" come se fossero spazi. `fmt` invece non compatta gli
spazi e tratta gli "a capo" nell'input come "a capo" nell'output.

## Per approfondire

- Si modifichi il programma in modo da giustificare le righe stampate
  a video, inserendo spazi aggiuntivi nel modo più uniforme possibile
  in modo che la lunghezza di ogni riga uguale a `Lmax`.

- La soluzione che si chiede di realizzare tratta gli "a capo" come se
  fossero spazi. Modificare il programma per mandare a capo l'output
  ogni volta che si incontra un "a capo" nell'input.

## File

- [formatter.c](formatter.c)
- [divina-commedia-canto1.in](divina-commedia-canto1.in) (da [LiberLiber](https://www.liberliber.it/online/autori/autori-a/dante-alighieri/la-divina-commedia-edizione-petrocchi)). Il comando `./formatter 75 divina-commedia-canto1.in` dovrebbe produrre [questo risultato](divina-commedia-canto1.out); la prima riga, prodotta dalla funzione `header()`, serve solo come riferimento.
- [the-time-machine.txt](the-time-machine.txt) ([Fonte](http://www.gutenberg.org/files/35/35-0.txt))
- [war-and-peace.txt](war-and-peace.txt) ([Fonte](http://www.gutenberg.org/files/2600/2600-0.txt))
- [the-hound-of-the-baskervilles.txt](the-hound-of-the-baskervilles.txt) ([Fonte](http://www.gutenberg.org/files/2852/2852-0.txt))
- [the-war-of-the-worlds.txt](the-war-of-the-worlds.txt) ([Fonte](http://www.gutenberg.org/files/36/36-0.txt))

***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/* massima lunghezza di una parola */
#define WORDLEN 1024

/* Legge la prossima parola dal file `f` e la memorizza in `s`; il
   buffer `s` deve essere creato dal chiamante e avere capienza
   `WORDLEN`. Restituisce il numero di caratteri letti, oppure 0 se
   non è stata letta alcuna parola. */
int read_word(FILE *f, char *s)
{
  int i = 0;
  int c;

  /* Salta gli eventuali spazi */
  do
  {
    c = fgetc(f);
  } while (c != EOF && isspace(c));
  /* Leggi la parola */
  while (c != EOF && !isspace(c))
  {
    assert(i < WORDLEN - 1);
    s[i] = c;
    i++;
    c = fgetc(f);
  }
  s[i] = '\0';
  return i;
}

/* Stampa una riga di intestazione di lunghezza `n` */
void header(int n)
{
  int i;

  for (i = 1; i <= n; i++)
  {
    printf("%c", i % 10 ? '-' : '|');
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  FILE *filein = stdin;
  int Lmax;
  char w[WORDLEN];
  if (argc != 3)
  {
    fprintf(stderr, "Uso: %s Lmax filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  Lmax = atoi(argv[1]);

  if (strcmp(argv[2], "-") != 0)
  {
    filein = fopen(argv[2], "r");
    if (filein == NULL)
    {
      fprintf(stderr, "Can not open %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  }

  header(Lmax);
  /* [TODO]: il blocco seguente legge e stampa le "parole" dal file,
     una per riga. Si tratta solo di un esempio di uso della
     funzione `read_word()`, e andrà modificato (o riscritto) per
     risolvere il problema. */

  while (read_word(filein, w) > 0)
  {
    printf("%s\n", w);
  }
  printf("\n");
  if (filein != stdin)
    fclose(filein);

  return EXIT_SUCCESS;
}
