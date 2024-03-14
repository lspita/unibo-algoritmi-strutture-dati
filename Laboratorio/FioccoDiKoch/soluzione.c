/****************************************************************************
 *
 * koch.c -- Curva di Koch
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
% LabASD - Fiocco di neve di Koch
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-03-05

(Crediti: prof. [Violetta Lonati](http://lonati.di.unimi.it/), Università di Milano)

![Niels Fabian Helge von Koch By Olof Edlund, Public Domain, <https://commons.wikimedia.org/w/index.php?curid=10118465>](Helge_von_Koch.jpg)

## Libreria `libpsgraph.c`

Questo esercizio fa uso della libreria `libpsgraph` che consente di
produrre dei semplici grafici in
[PostScript](https://en.wikipedia.org/wiki/PostScript). Per usare
la libreria si può procedere come segue:

* creare una directory (si può usare la directory corrente);

* salvare in essa i file [libpsgraph.c](libpsgraph.c) e
  [libpsgraph.h](libpsgraph.h);

* scrivere la propria applicazione (che nel nostro caso è il file
  [koch.c](koch.c)), che include all'inizio la direttiva

        #include "libpsgraph.h"

* compilare il programma con il comando

        gcc -std=c90 -Wall -Wpedantic libpsgraph.c koch.c -lm -o koch
  Attenzione alla necessità di linkare la libreria matematica usando
  il flag `-lm`; su Code::Blocks non dovrebbe essere necessario, dato
  che la libreria matematica viene linkata di default.

## Funzioni fornite dalla libreria

La libreria `libpsgraph` consente di disegnare usando le funzioni
della ["turtle
graphics"](https://en.wikipedia.org/wiki/Turtle_graphics); il disegno
viene salvato in formato
[PostScript](https://en.wikipedia.org/wiki/PostScript) su un file di
testo, il cui nome va specificato prima di iniziare a
disegnare. PostScript è un linguaggio di programmazione vero e
proprio: chi è curioso può aprire il file di output con un editor di
testo per vedere i comandi che contiene. Per visualizzare l'immagine
senza bisogno di inviarla ad una stampante si può usare
[Gimp](https://www.gimp.org/) che è già installato sui PC del
laboratorio.

La libreria `libpsgraph` va usata in questo modo:

* Per prima cosa, bisogna invocare la funzione `start(nomefile)`
  passando come parametro il nome del file su cui si vuole che il
  grafico venga salvato, ad esempio `start("prova.ps");`.

* A questo punto è possibile disegnare usando le funzioni seguenti,
  che accettano parametri di tipo `double`:

  * `draw(x)`: disegna un segmento lungo $x$ millimetri.

  * `move(x)`: si sposta (senza disegnare) di un segmento lungo $x$ millimetri.

  * `turn(x)`: ruota di $x$ gradi in senso orario; $x$ può essere
    negativo, nel qual caso gira in senso antiorario.

  * `setcolor(r, g, b)`: imposta il colore a $(r, g, b)$, dove i
    parametri possono assumere valori reali in $[0, 1]$.  `setcolor(0, 0, 0)` 
    è il colore nero, `setcolor(1, 1, 1)` è il colore bianco.

* Alla fine, prima di terminare il programma occorre invocare la
  funzione `end()`.

All'inizio, la "tartaruga" è orientata verso destra; quindi, se la
prima istruzione è `draw(10)` si ottiene un segmento orizzontale di
lunghezza 10 orientato da sinistra verso destra.

Il programma seguente disegna un quadrato:

```C
#include "libpsgraph.h"

int main( void )
{
    start("square.ps");
    draw(100); turn(90);
    draw(100); turn(90);
    draw(100); turn(90);
    draw(100);
    end();
    return 0;
}

```

## Curva di Koch

Realizzare una funzione che, data una lunghezza in millimetri $x$ e un
intero $n \geq 0$ produce la _curva di Koch_ di ordine $n$ e di
"lunghezza" $x$; per "lunghezza" qui si intende la lunghezza della sola
base.

La [curva di Koch](https://en.wikipedia.org/wiki/Koch_snowflake)
prende il nome dal matematico svedese [Niels Fabian Helge von
Koch](https://en.wikipedia.org/wiki/Helge_von_Koch), che l'ha studiata
per primo, ed è definita come segue:

* se $n = 0$, la curva è un segmento di lunghezza $x$;

* se $n > 0$, la curva è ottenuta concatenando quattro curve di Koch
  di ordine $n-1$ e lunghezza $x/3$ ciascuna, disposte come mostrato
  in Figura 1.

Le curve di Koch di ordine $n=0, \ldots, 4$ sono mostrate in Figura 1.
Si può notare come la curva di ordine $n=4$ sia composta da quattro
curve di ordine $n=3$ e lunghezza $x/3$ orientate allo stesso modo dei
quattro segmenti del caso $n=1$. Nel caso $n=1$ i due segmenti
centrali sono disposti lungo i lati di un triangolo equilatero a cui è
stata tolta la base.

![Figura 1: Curve di Koch dall'ordine $n=0$ (in basso) fino a $n=4$
 (in alto); in questa figura si è usato $x=50$, ma conviene provare
 altri valori per verificare quali producono il risultato più
 leggibile.](koch-curve.png)

## Fiocco di neve di Koch

Realizzare ora una funzione che, data una lunghezza in millimetri $x$
e un intero $n \geq 0$, produce il _fiocco di neve di Koch_ di ordine
$n$: esso si ottiene da un triangolo equilatero i cui lati siano
costituiti da tre curve di Koch di ordine $n$ e lunghezza $x$ (Figura
2).

![Figura 2: Fiocco di neve di Koch di ordine $n=4$ con $x=50$.](koch-snowflake.png)

## Domande

- Da quanti segmenti è composta la curva di Koch di ordine $n$? Per
  $n=0$ c'è un solo segmento, per $n=1$ ce ne sono quattro, ...

- Sia $K(n)$ il numero totale di invocazioni della funzione `koch()`
  necessarie per disegnare la curva di Koch di ordine $n$. Quindi
  $K(0)=1$, dato che la funzione viene invocata una sola volta quando
  $n=0$; $K(1)=5$ dato che la funzione viene invocata una volta con
  $n=1$ e quattro volte con $n=0$; e così via. Scrivere una
  espressione chiusa (cioè non ricorsiva) per $K(n)$ (Suggerimento: si
  può partire dalla forma ricorsiva ed espandere i primi termini per
  individuare l'espressione chiusa).

- Dati $x$ e $n$, qual è la lunghezza complessiva della curva
  disegnata dalla chiamate `koch(x, n)`? Per "lunghezza complessiva"
  si intende la somma delle lunghezze di tutti i segmenti che la
  compongono.

## File

- [koch.c](koch.c)
- [libpsgraph.c](libpsgraph.c)
- [libpsgraph.h](libpsgraph.h)

***/
#include <stdio.h>
#include <stdlib.h>
#include "libpsgraph.h"

/* Disegna la curva di Koch di ordine n e lunghezza x */
void koch(double x, int n)
{
    if (n == 0) {
        draw(x);
    } else {
        koch(x/3, n-1);
        turn(-60);
        koch(x/3, n-1);
        turn(120);
        koch(x/3, n-1);
        turn(-60);
        koch(x/3, n-1);
    }
}

void fractal_tree(double x, int n)
{
    if (n > 0) {
        Turtle t;
        draw(x);
        save_state(&t);
        turn(-30);
        fractal_tree(x*0.7, n-1);
        restore_state(&t);
        turn(30);
        fractal_tree(x*0.7, n-1);
    }
}

int main( void )
{
    const int n = 4;
    int i;
    start("koch-curve.ps");
    for (i=0; i<4; i++) {
        koch(50, i);
        move(-50); turn(-90); move(15); turn(90);
    }
    koch(50.0/3, i-1);
    turn(-60);
    setcolor(1, 0, 0);
    koch(50.0/3, i-1);
    turn(120);
    setcolor(0, 1, 0);
    koch(50.0/3, i-1);
    turn(-60);
    setcolor(0, 0, 1);
    koch(50.0/3, i-1);
    end();

    start("koch-snowflake.ps");
    setcolor(1, 0, 0); koch(50, n);
    turn(120);
    setcolor(0, 1, 0); koch(50, n);
    turn(120);
    setcolor(0, 0, 1); koch(50, n);
    end();

    start("fractal-tree.ps");
    turn(-90);
    fractal_tree(20, 6);
    end();
    return EXIT_SUCCESS;
}
