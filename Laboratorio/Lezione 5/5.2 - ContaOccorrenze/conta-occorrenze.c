/****************************************************************************
 *
 * conta-occorrenze.c -- Conta il numero di occorrenze delle parole in un testo
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
% LabASD - Conta il numero di occorrenze delle parole in un testo
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-04-04

![By In2thats12 - Own work, Public Domain, <https://commons.wikimedia.org/w/index.php?curid=11694892>](tag-cloud.png "Tag cloud")

Scrivere un programma che legge un file di testo e conta il numero di
occorrenze di ciascuna delle parole in esso contenute. Per "parola" si
intende una sequenza di caratteri alfabetici adiacenti per i quali la
funzione

```C
int isalpha(int c)
```

restituisca _true_ (cioè un valore diverso da zero); _c_ è il codice
ASCII del carattere. La funzione `isalpha()` è definita in
`<ctype.h>`. Il programma non deve fare differenza tra maiuscole e
minuscole.

Il programma legge il testo da un file il cui nome è passato come
unico parametro sulla riga di comando. Al termine dell'esecuzione, il
programma deve stampare a video le parole presenti nel file,
_in ordine alfabetico_, con accanto il numero di occorrenze (ogni parola
deve comparire una volta sola).

Ad esempio, se l'input fosse:

```
Nel mezzo del cammin di nostra vita
mi ritrovai per una selva oscura,
che' la diritta via era smarrita.
Ahi quanto a dir qual era e' cosa dura
esta selva selvaggia e aspra e forte
che nel pensier rinova la paura!
Tant' e' amara che poco e' piu' morte;
ma per trattar del ben ch'i' vi trovai,
diro' de l'altre cose ch'i' v'ho scorte.

```

il programma deve stampare:

```
a 1
ahi 1
altre 1
amara 1
aspra 1
ben 1
cammin 1
ch 2
che 3
cosa 1
cose 1
de 1
del 2
di 1
dir 1
diritta 1
diro 1
dura 1
e 5
era 2
esta 1
forte 1
ho 1
i 2
l 1
la 2
ma 1
mezzo 1
mi 1
morte 1
nel 2
nostra 1
oscura 1
paura 1
pensier 1
per 2
piu 1
poco 1
qual 1
quanto 1
rinova 1
ritrovai 1
scorte 1
selva 2
selvaggia 1
smarrita 1
tant 1
trattar 1
trovai 1
una 1
v 1
vi 1
via 1
vita 1
```

Per risolvere questo problema serve una struttura dati che consenta di
gestire un insieme di coppie (_parola, numero occorrenze_) supportando
le seguenti operazioni:

1. inserimento di una nuova parola;

2. incremento del conteggio di occorrenze di una parola esistente;

3. elenco delle parole in ordine alfabetico, con le rispettive occorrenze.

I requisiti 1 e 2 possono essere soddisfatti da una tabella hash, che
però non è adatta per realizzare il requisito 3 in modo efficiente.

Tra le tante soluzioni possibili, si richiede di usare un albero
binario di ricerca (_Binary Search Tree_, BST), dove le parole sono le
chiavi, e il numero delle occorrenze è un dato satellite. I nodi
dell'albero possono essere definiti come:

```C
typedef struct BSTNode {
    char *s;
    int count;
    struct BSTNode *left, *right;
} BSTNode;
```

dove `count` è il numero di occorrenze della parola `s`.

A differenza dell'implementazione dei BST vista [in un altro
esercizio](bst.html), non serve il puntatore al padre. Infatti, per
questo esercizio non occorre realizzare tutte le operazioni di un BST,
ma è sufficiente implementare l'inserimento modificato come segue:

- Se la parola da inserire non è presente, si crea un nuovo nodo
  impostando a uno il conteggio delle occorrenze.

- Se la parola da inserire è già presente, si incrementa il contatore
  del nodo già esistente.

Non è quindi richiesto di realizzare l'operazione di cancellazione,
che come abbiamo visto è la più laboriosa.

Per confrontare due parole e decidere come muoversi nell'albero si usi
la funzione

```C
int strcmp(const char *s1, const char *s2)
```

dichiarata in `<string.h>` che ritorna un valore negativo se `s1` è
"minore" di `s2`, un valore positivo se `s1` è "maggiore" di `s2`, e
zero se le due stringhe sono identiche.

## Suggerimento

Per inserire una parola nell'albero (o incrementare il contatore di
occorrenze) si può definire una funzione ricorsiva `insert()` con la
seguente segnatura:

```C
BSTNode *insert(const char *s, BSTNode *t);
```

La funzione inserisce una nuova parola `s` con associato contatore a
uno, oppure incrementa il contatore se `s` è già presente, nel
sottoalbero radicato in `t`; al termine, ritorna la radice del
sottoalberoche potrebbe coincidere con `t`.

Si osservi che `t` può essere `NULL`, indicando che la stringa deve
essere inserita in un (sotto-)albero vuoto. In tal caso la funzione
crea un nuovo nodo, imposta il contatore di occorrenze a uno, e
ritorna il puntatore al nodo appena creato. Nel caso in cui `t !=
NULL`, la radice del sottoalbero non cambia (la procedura di
inserimento in un BST aggiunge una foglia), per cui la funzione
ritorna lo stesso `t` passato come parametro.

Possiamo quindi schematizzare `insert()` nel modo seguente:

```C
BSTNode *insert(const char *s, BSTNode *t)
{
    if (t == NULL) {
      BSTNode *n;
      \/\* TODO: crea un nuovo nodo n contenente una copia di s
         con conteggio occorrenze posto a uno. \*\/
      return n;
    } else {
      if (strcmp(s, t->s) == 0) {
         t->count++;
      } else if (strcmp(s, t->s) < 0) {
          t->left = insert(s, t->left);
      } else {
          t->right = insert(s, t->right);
      }
      return t;
    }
}
```

**Attenzione**: la funzione `insert()` deve allocare spazio per una
_copia_ della stringa `s`, in modo simile a quanto avevamo visto [per
le tabelle hash](hashtable.html).

Usando la funzione `insert()`, l'algoritmo assume la forma seguente:

```C
BSTNode *root = NULL;
"Finché ci sono parole" {
  "Leggi la prossima parola s";
  root = insert(s, root);
}
```

Al termine, la lista ordinata si ottiene tramite una visita in
pre-ordine dell'albero binario di ricerca radicato in `root`.

Suggerisco di leggere un carattere alla volta dal file usando la
funzione

```C
int fgetc(FILE *stream)
```

dichiarata in `<stdio.h>`; la funzione ritorna il codice ASCII del
carattere letto, oppure il valore `EOF` nel caso in cui si sia
raggiunta la fine del file (`EOF` ha solitamente valore -1, ma non è
garantito).

Per convertire un carattere in minuscolo si usi la funzione

```C
int tolower(int c)
```

dichiarata in `<ctype.h>`. La funzione accetta come parametro il
codice ASCII di un simbolo _c_, e restituisce il codice ASCII della
versione minuscola di _c_, se è una lettera alfabetica, oppure _c_ nel
caso si tratti di un altro simbolo.

Si può assumere che tutte le parole presenti nel file siano sempre di
lunghezza strettamente minore a `WORDLEN`.

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

potrebbe non funzionare correttamente in presenza di caratteri del set
ASCII esteso (ad esempio, lettere accentate). Ad esempio, se alla
variabile `c` fosse assegnato il carattere `'è'` (codice ASCII 195),
il valore memorizzato in `c` sarebbe in realtà -61, dato che 195 non
può essere rappresentato con 8 bit in complemento a due. Di
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

- [conta-occorrenze.c](conta-occorrenze.c)
- [the-time-machine.txt](the-time-machine.txt) ([Fonte](http://www.gutenberg.org/files/35/35-0.txt)) (la parola "about" compare 69 volte; "certain" 18 volte; "you" 137 volte)
- [war-and-peace.txt](war-and-peace.txt) ([Fonte](http://www.gutenberg.org/files/2600/2600-0.txt))
- [the-hound-of-the-baskervilles.txt](the-hound-of-the-baskervilles.txt) ([Fonte](http://www.gutenberg.org/files/2852/2852-0.txt))
- [the-war-of-the-worlds.txt](the-war-of-the-worlds.txt) ([Fonte](http://www.gutenberg.org/files/36/36-0.txt))

***/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define WORDLEN 128

typedef struct BSTNode {
    char *s;
    int count;
    struct BSTNode *left, *right;
} BSTNode;


/* Legge la prossima parola dal file `f` e la memorizza nel buffer `s`
   di lunghezza `WORDLEN`; il buffer deve essere già stato allocato
   dal chiamante. Restituisce il numero di caratteri letti; 0 indica
   che si è raggiunta la fine del file.

   SUGGERIMENTO: questa è la stessa funzione usata nell'esercizio sul
   conteggio del numero di parole distinte, per cui può essere copiata
   dalla rispettiva soluzione.
*/
int read_word(FILE *f, char *s)
{
    /* TODO */
    s[0] = '\0';
    return 0;
}

/* Se la parola `s` non è presente nell'albero di radice `t`, crea un
   nuovo nodo e inserisce una copia della parola con conteggio di
   occorrenze posto a uno.  Se la parola è già presente, incrementa il
   contatore di occorrenze. Ritorna la (nuova) radice dell'albero. */
BSTNode *insert(const char *s, BSTNode *t)
{
    /* [TODO] Si suggerisce di realizzare questa funzione in modo
       ricorsivo. */
    return NULL; /* valore di ritorno fittizio per evitare warning del compilatore. */
}

/* TODO: aggiungere tutte le funzioni che si ritengono necessarie. */

int main( int argc, char *argv[] )
{
    FILE *filein = stdin;
    int nwords;
    char s[WORDLEN];
    BSTNode *t = NULL;

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
    while (read_word(filein, s)) {
        /* printf("%s\n", s); per verificare quale parola è stata letta */
        nwords++;
        t = insert(s, t);
    }
    /* [TODO] Stampare il contenuto dell'albero con una visita in
       preordine (da realizare). */
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
