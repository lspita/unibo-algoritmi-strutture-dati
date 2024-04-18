/****************************************************************************
 *
 * cut-rod.c -- Rod cutting problem
 *
 * Copyright (C) 2022, 2024 Moreno Marzolla
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
% LabASD - Rod cutting problem
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-04-12

![](cut-rod.jpg "Rod cutting problem")

Disponiamo di un'asta di lunghezza intera $n$, $1 \leq n < N$, che
possiamo tagliare in segmenti più corti da rivendere a prezzi
diversi. Le lunghezze ammesse sono $1, 2, \ldots, n$, e si vendono
rispettivamente al prezzo $p[1], p[2], \ldots, p[n]$ (valori
reali). Vogliamo determinare il massimo guadagno che è possibile
ottenere dal taglio dell'asta, quali lunghezze occorre tagliare per
ottenere tale guadagno massimo.

Il programma legge l'input da un file il cui nome va specificato sulla
riga di comando. Il file contiene $n$ prezzi, uno per riga:

```
p1
p2
...
pn
```

Ad esempio, il file [cut-rod1.in](cut-rod1.in) contiene i prezzi di
dieci segmenti ($n=10$) come segue:

Lunghezza $j$     1   2   3   4   5   6   7   8   9  10
--------------- --- --- --- --- --- --- --- --- --- ---
Prezzo $p[j]$     1   5   8   9  13  17  17  20  24  25

In questo caso la soluzione ottima consiste nel dividere l'asta lunga
$n=10$ in tre segmenti di lunghezze $2+2+6$, ottenendo un guadagno
$5+5+17=27$.

Per risolvere efficientemente il problema si può usare la
programmazione dinamica. Sia $P(j)$ il sottoproblema che consiste nel
determinare il massimo guadagno che si può ottenere suddividendo
un'asta di lunghezza $j$, $j = 0, \ldots, n$. Denotiamo con $r[j]$ la
soluzione del problema $P(j)$, cioè il guadagno massimo. Osserviamo
che:

- $r[0] = 0$, perché da un'asta di lunghezza zero non guadagniamo nulla;

- Se l'asta ha lunghezza $j>0$, possiamo decidere di tagliare un
  segmento lungo $i$ da rivendere al prezzo $p[i]$, e suddividere in
  modo ottimo il residuo di lunghezza $(j-i)$ ottenendo un guadagno
  $r[j-i]$. Per massimizzare il guadagno complessivo, scegliamo il
  valore $i$ che massimizza il guadagno. Il guadagno massimo sarà
  quindi $r[j] = \max_{1 \leq i \leq j} \{ p[i] + r[j-i] \}$

In modo più compatto, possiamo scrivere:

$$
p[j] = \begin{cases}
0 & \mbox{se}\ j = 0\\
\displaystyle\max_{1 \leq i \leq j} \{ p[i] + r[j-i] \} & \mbox{altrimenti}
\end{cases}
$$

La soluzione al problema di partenza (massimo guadagno ottenibile
suddividendo un tubo lungo $n$) è $r[n]$.

Per conoscere la sequenza di tagli che produce il guadagno massimo
$r[n]$ è utile introdurre un array di supporto $s[1], s[2], \ldots,
s[n]$.  Per ogni $j = 1, \ldots, n$, $s[j]$ è il valore di $i$ che
massimizza l'espressione $p[i] + r[j-1]$ di cui sopra. Nell'esempio
precedente si ha:

Lunghezza $j$         1   2   3   4   5   6   7   8   9  10
--------------- --- --- --- --- --- --- --- --- --- --- ---
Prezzo $p[j]$         1   5   8   9  13  17  17  20  24  25
Guadagno $r[j]$   0   1   5   8  10  13  17  18  22  25  27
$s[j]$                1   2   3   2   2   6   1   2   3   2

Da cui:

- Per ottenere il guadagno ottimo $r[10]$ è stato asportato per ultimo
  un segmento lungo $s[10] = 2$; il residuo ha lunghezza 8.

- Per ottenere il guadagno ottimo $r[8]$ è stato asportato per ultimo
  un segmento lungo $s[8] = 2$; il residuo ha lunghezza 6.

- Per ottenere il guadagno ottimo $r[6]$ è stato asportato per ultimo
  un segmento lungo $s[6] = 6$; il residuo ha lunghezza 0, e il
  procedimento termina.

Per compilare:

        gcc -std=c90 -Wall -Wpedantic cut-rod.c -o cut-rod

Per eseguire in ambiente Linux/MacOSX:

        ./cut-rod cut-rod1.in

Per eseguire in ambiente Windows:

        .\cut-rod cut-rod1.in

## File

- [cut-rod.c](cut-rod.c)
- [cut-rod1.in](cut-rod1.in) ([output atteso](cut-rod1.out))
- [cut-rod2.in](cut-rod2.in)
- [cut-rod3.in](cut-rod3.in)
- [cut-rod4.in](cut-rod4.in)

***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N 1000

/* Sebbene l'uso di variabili globali sia considerato (a ragione) una
   cattiva pratica di programmazione, la soluzione proposta le
   utilizza per semplificare l'interfaccia delle funzioni. Questo è
   giustificato dalla relativa semplicità di questo programma. Chi lo
   desidera può ristrutturare il codice trasformando le variabili
   globali in parametri da passare alle funzioni. */

double p[N];    /* p[i] è il prezzo di vendita di un segmento lungo
                   i, i=1..n; p[0] non viene usato. */
int n;          /* lunghezza dell'asta da tagliare. */
double r[N];    /* r[j] è il massimo guadagno che è possibile ottenere
                   tagliando un'asta lunga j, j=0..n. */
int s[N];       /* s[j] è la lunghezza della sezione che viene
                   tagliata per ultima per ottenere il guadagno
                   massimo r[j] del problema P(j). s[0] non viene
                   usato. */

/* Legge l'input dal file `filein` */
void init( FILE *filein )
{
    double val;
    p[0] = 0.0; /* p[0] non viene usato, quindi è inizializzato ad un
                   valore arbitrario. */
    n = 0;
    while (1 == fscanf(filein, "%lf", &val)) {
        n++;
        assert(n < N);
        p[n] = val;
    }
}

/* Determina la soluzione del problema "cut rod". Restituisce il
   guadagno massimo. */
double solve( void )
{
    int i, j;

    for (i=0; i<=n; i++) {
        r[i] = 0.0;
        s[i] = 0;
    }

    for (j=1; j<=n; j++) {
        double guadagno_max = -1;
        for (i=1; i<=j; i++) {
            if (guadagno_max < p[i] + r[j-i]) {
                guadagno_max = p[i] + r[j-i];
                s[j] = i;
            }
        }
        r[j] = guadagno_max;
    }
    return r[n];
}

/* Stampa la soluzione del problema. */
void print( void )
{
    int j;

    printf("     | ");
    for (j=0; j<=n; j++) {
        printf("%6d ", j);
    }
    printf("\n");

    printf(" p[] |        ");
    for (j=1; j<=n; j++) {
        printf("%6.2f ", p[j]);
    }
    printf("\n");

    printf(" r[] | ");
    for (j=0; j<=n; j++) {
        printf("%6.2f ", r[j]);
    }
    printf("\n");

    printf(" s[] |        ");
    for (j=1; j<=n; j++) {
        printf("%6d ", s[j]);
    }
    printf("\n\n");

    printf("      Lun   Prezzo\n");
    printf("   ------ --------\n");
    for (j=n; j>0; j -= s[j]) {
        printf("   %6d %8.2f\n", s[j], p[s[j]]);
    }
    printf("   ------ --------\n");
    printf("Guadagno: %8.2f\n", r[n]);
}

double randab(double a, double b)
{
    return (rand() / (double)RAND_MAX) * (b-a) + a;
}

void generate_input(int n)
{
    int i;
    for (i=0; i<n; i++) {
        printf("%.2f\n", randab(2.0*(i+1), 4.0*(i+1)));
    }
}

int main( int argc, char *argv[] )
{
    FILE *filein = stdin;

    if (argc < 2) {
        fprintf(stderr, "Invocare il programma con: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    } else if (argc > 2) {
        generate_input(atoi(argv[1]));
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-") != 0) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    init( filein );
    solve( );
    print( );
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
