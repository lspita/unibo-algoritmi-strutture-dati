/****************************************************************************
 *
 * uova.c -- Problema delle uova di Fibonacci
 *
 * Copyright (C) 2021, 2022, 2023 Moreno Marzolla
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
% LabASD - Il problema delle uova di Fibonacci
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2023-02-28

![Leonardo Pisano detto Fibonacci (c. 1170-c. 1240)](Fibonacci.jpg)

Il problema seguente è tratto da [Liber
Abbaci](https://it.wikipedia.org/wiki/Liber_abbaci), un manuale di
algebra scritto nel 1202 da [Leonardo Pisano detto
Fibonacci](https://it.wikipedia.org/wiki/Leonardo_Fibonacci). Espresso
in forma moderna, il problema è il seguente:

> Un grosso cesto contiene delle uova. Il numero di uova gode delle
> seguenti proprietà: se si contano le uova due a due, oppure tre a
> tre, oppure quattro a quattro, oppure cinque a cinque, oppure sei a
> sei, allora alla fine del conteggio ne resta una. Se invece si
> contano le uova sette a sette, alla fine del conteggio non ne rimane
> nessuna.

Scrivere un programma che calcoli e stampi il minimo valore intero
$n>0$ che goda delle proprietà precedenti.

## File

- [uova.c](uova.c)

***/
#include <stdio.h>

int main(void)
{
    /* Dalla specifica del problema, n deve essere multiplo di 7; di
       conseguenza, esaminiamo solo i multipli di 7 per individuare il
       più piccolo di essi che goda delle rimanenti proprietà */
    int n = 0, trovato = 0;
    do
    {
        n += 7;
        trovato = ((n % 2 == 1) &&
                   (n % 3 == 1) &&
                   (n % 4 == 1) &&
                   (n % 5 == 1) &&
                   (n % 6 == 1));
    } while (!trovato);
    printf("%d\n", n); /* il risultato corretto e' 301 */
    return 0;
}
