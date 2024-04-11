/****************************************************************************
 *
 * cut-rod.c -- Rod cutting problem
 *
 * Copyright (C) 2022 Moreno Marzolla
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
% Ultimo aggiornamento: 2022-04-04

![](cut-rod.jpg "Rod cutting problem")

Disponiamo di un'asta di lunghezza intera $n$, $1 \leq n < N$, che
possiamo tagliare in segmenti più corti da rivendere a prezzi
diversi. Le lunghezze ammesse sono $1, 2, \ldots, n$, e si vendono
rispettivamente al prezzo $p_1, p_2, \ldots, p_n$ (valori
reali). Vogliamo determinare il massimo guadagno che è possibile
ottenere dal taglio dell'asta; vogliamo inoltre determinare quali
lunghezze occorre tagliare per ottenere tale guadagno massimo.

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

Lunghezza $i$     1   2   3   4   5   6   7   8   9  10
--------------- --- --- --- --- --- --- --- --- --- ---
Prezzo $p[i]$     1   5   8   9  13  17  17  20  24  25

In questo caso la soluzione ottima consiste nel dividere l'asta lunga
$n=10$ in tre segmenti di lunghezze $2+2+6$, ottenendo un guadagno
$5+5+17=27$.

Per risolvere efficientemente il problema si può usare la
programmazione dinamica. Sia $P(i)$ il sottoproblema che consiste nel
determinare il massimo guadagno che si può ottenere suddividendo
un'asta di lunghezza $i$, $0 \leq i \leq n$. Denotiamo con $r[i]$ la
soluzione del problema $P(i)$, cioè il guadagno massimo. Osserviamo
che:

- $r[0] = 0$, perché da un'asta di lunghezza zero non guadagniamo nulla;

- Se l'asta ha lunghezza $j>0$, possiamo decidere di tagliare un
  segmento lungo $i$ da rivendere al prezzo $p[i]$, e suddividere in
  modo ottimo la lunghezza residua $(j-i)$ per ottenere un guadagno
  $r[j-i]$. Per massimizzare il guadagno complessivo, scegliamo il
  valore $i$ che massimizza il guadagno. Il guadagno massimo sarà
  quindi $r[j] = \max_{1 \leq i \leq j} \{ p[i] + r[j-i] \}$

Per compilare:

        gcc -std=c90 -Wall -Wpedantic cut-rod.c -o cut-rod

Per eseguire:

        ./cut-rod cut-rod1.in

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

double p[N]; /* p[i] è il prezzo di vendita di un segmento lungo
                i, i=1..n; p[0] non viene usato */
int n;       /* lunghezza dell'asta da tagliare */
double r[N]; /* r[j] è il massimo guadagno che è possibile ottenere
                tagliando un'asta lunga j, j=0..n */
int s[N];    /* s[j] è la lunghezza dell'ultimo taglio della
                soluzione ottima del problema P(j) */

/* Legge l'input dal file `filein` */
void init(FILE *filein)
{
    double val;
    p[0] = 0.0; /* p[0] non viene usato */
    n = 0;
    while (1 == fscanf(filein, "%lf", &val))
    {
        n++;
        assert(n < N);
        p[n] = val;
    }
}

/* Determina la soluzione del problema "cut rod". Restituisce il
   guadagno massimo */
double solve(void)
{
    int j, i;
    int q;
    r[0] = 0.0;
    for (j = 1; j <= n; j++)
    {
        q = -1;
        for (i = 1; i <= j; i++)
        {
            if (q < p[i] + r[j - i])
            {
                q = p[i] + r[j - i];
                s[j] = i;
            }
        }
        r[j] = q;
    }

    return r[n];
}

/* Stampa la soluzione del problema. */
void print(void)
{
    int i = n;

    printf("   Lun   Prezzo\n");
    printf("------ --------\n");

    while (i > 0)
    {
        printf("     %d     %.2f\n", s[i], p[s[i]]);
        i = i - s[i];
    }

    printf("Guadagno complessivo: %.2f\n", r[n]);
}

int main(int argc, char *argv[])
{
    FILE *filein = stdin;

    if (argc != 2)
    {
        fprintf(stderr, "Invocare il programma con: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-") != 0)
    {
        filein = fopen(argv[1], "r");
        if (filein == NULL)
        {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    init(filein);
    (void)solve(); /* il cast a void non serve, ma alcuni compilatori particolarmente solerti potrebbero segnalare un warning perché non stiamo utilizzando il valore di ritorno della funzione */
    print();
    if (filein != stdin)
        fclose(filein);

    return EXIT_SUCCESS;
}
