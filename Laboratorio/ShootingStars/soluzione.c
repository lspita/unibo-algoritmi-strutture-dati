/****************************************************************************
 *
 * shooting-stars.c - Shooting Stars
 *
 * Copyright (C) 2018--2024 Moreno Marzolla
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
% LabASD - Shooting Stars
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-02-26

![](shooting-stars.jpg "Copertina del numero 9, maggio 1976 di BYTE Magazine dedicata a shooting stars")

"Shooting stars" è un gioco di strategia descritto nella rivista [BYTE
Magazine n. 9, p. 42, maggio
1976](https://archive.org/details/byte-magazine-1976-05/page/n43/mode/1up).

Il gioco si svolge su una griglia $3 \times 3$ le cui celle sono
numerate da 0 a 8 come segue:

```
012
345
678
```

Ogni cella può contenere una stella (`*`) oppure un buco nero
(`.`). La configurazione iniziale contiene un'unica stella nella cella
4:

```
...
.*.
...
```

Ad ogni turno il giocatore può far "esplodere" una stella digitando il
numero della cella corrispondente. Una stella esplosa si trasforma in
un buco nero; inoltre, i frammenti della stella vanno a finire in
alcune delle celle adiacenti, trasformando i buchi neri in stelle, e
le stelle in buchi neri. Il vicinato è definito in modo differente per
ogni cella, come nella figura seguente:

![](shooting-stars.svg)

Ad esempio, il vicinato della cella 0 è costituita da (1, 3, 4); il
vicinato della cella 1 è costituito da (0, 2); il vicinato della cella
2 è costituito da (1, 4, 5), e così via. In pratica, fare esplodere la
stella in posizione $k$ significa trasformare stelle in buchi neri (e
viceversa) nella cella $k$ e in quelle "vicine" secondo le figure
sopra.

Ad esempio, partendo dalla configurazione iniziale (per facilitare la
lettura, le celle contenenti una stella sono indicate con il
rispettivo numero):

```
...
.4.
...
```

il giocatore può solo fare esplodere la stella 4, ottenendo la nuova
configurazione

```
.1.
3.5
.7.
```

Se ora decide di fare esplodere la stella 3, la nuova configurazione
sarà

```
01.
..5
67.
```

e così via.

Si vince se si ottiene la configurazione

```
012
3.5
678
```

mentre si perde se si ottiene la configurazione contenente solo buchi neri

```
...
...
...
```

dato che non sarebbe possibile far esplodere alcuna stella.

Compilare il programma con

        gcc -std=c90 -Wall -Wpedantic shooting-stars.c -o shooting-stars

In ambiente Linux/MacOSX eseguire con

        ./shooting-stars

In ambiente Windows eseguire con

        .\shooting-stars

## Per approfondire

Alla fine del corso sarete in grado di scrivere un programma
efficiente in grado di rispondere a queste domande:

- Qual è il numero minimo di mosse necessarie per vincere? Qual è la
  sequenza minima di mosse che porta alla vittoria?

- Qual è il numero minimo di mosse necessarie a perdere (cioè
  a raggiungere la configurazione vuota)?

- Esiste una configurazione di stelle/buchi neri che non può essere
  generata dallo stato iniziale effettuando solo mosse valide?

Per il momento si richiede di completare il programma in modo da
consentire all'utente di giocare una partita.

## File

- [shooting-stars.c](shooting-stars.c)

***/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int board[3][3]; /* board[i][j] == 0 indica un buco nero, 1 indica una stella */

enum
{
    BLACK_HOLE,
    STAR
};

int nbor[9][3][3] = {
    {{1, 1, 0}, /* cella 0 con il suo vicinato	*/
     {1, 1, 0},
     {0, 0, 0}},

    {{1, 1, 1}, /* cella 1 con il suo vicinato 	*/
     {0, 0, 0},
     {0, 0, 0}},

    {{0, 1, 1}, /* cella 2 con il suo vicinato 	*/
     {0, 1, 1},
     {0, 0, 0}},

    {{1, 0, 0}, /* cella 3 con il suo vicinato 	*/
     {1, 0, 0},
     {1, 0, 0}},

    {{0, 1, 0}, /* cella 4 con il suo vicinato 	*/
     {1, 1, 1},
     {0, 1, 0}},

    {{0, 0, 1}, /* cella 5 con il suo vicinato 	*/
     {0, 0, 1},
     {0, 0, 1}},

    {{0, 0, 0}, /* cella 6 con il suo vicinato 	*/
     {1, 1, 0},
     {1, 1, 0}},

    {{0, 0, 0}, /* cella 7 con il suo vicinato 	*/
     {0, 0, 0},
     {1, 1, 1}},

    {{0, 0, 0}, /* cella 8 con il suo vicinato 	*/
     {0, 1, 1},
     {0, 1, 1}}};

/* Stampa la griglia memorizzata nell'intero |stars|. */
void print_board(void)
{
    int i, j, idx = 0;
    printf("\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] == BLACK_HOLE)
            {
                printf(". ");
            }
            else
            {
                printf("%d ", idx);
            }
            idx++;
        }
        printf("\n\n");
    }
    printf("\n");
}

typedef enum
{
    INVALID = 0,
    OK,
    WON,
    LOST,
    RESIGNED
} Move;

void init_board(void)
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            board[i][j] = (i == 1 && j == 1);
        }
    }
}

/* Inverte il contenuto della cella in posizione i,j */
void inverti(int i, int j)
{
    assert(i >= 0 && i < 3);
    assert(j >= 0 && j < 3);
    board[i][j] = 1 - board[i][j];
}

/* ritorna true (nonzero) se e solo se la configurazione corrente è
   quella vincente */
int won(void)
{
    /* È possibile determinare se la configurazione è quella vincente
       senza un confronto esplicito con la matrice winning_conf[][],
       ma usando una condizione booleana. Il risultato è probabilmente
       meno comprensibile */
    static const int winning_conf[3][3] = {{1, 1, 1},
                                           {1, 0, 1},
                                           {1, 1, 1}};
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] != winning_conf[i][j])
                return 0;
        }
    }
    return 1;
}

/* ritorna true (nonzero) se e solo se la configurazione corrente è
   quella perdente */
int lost(void)
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] == STAR)
                return 0;
        }
    }
    return 1;
}

/* Fa esplodere la stella in posizione `k`; le posizioni sono numerate
   come:

   012
   345
   678

   Restituisce INVALID se la mossa non è valida, cioè se k non
   appartiene all'insieme 0..8 oppure se la cella k non contiene una
   stella. In caso di mossa valida, applica la mossa modificando la
   configurazione dell'area di gioco e restituisce WON se la nuova
   configurazione è quella vincente, LOST se la nuova configurazione è
   quella perdente (solo buchi neri), e OK se non è né vincente né
   perdente.
*/
Move shoot(int k)
{
    const int r = k / 3; /* row */
    const int c = k % 3; /* col */
    int i, j;

    if (k < 0 || k > 8)
        return INVALID;

    if (board[r][c] != STAR)
        return INVALID;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (nbor[k][i][j])
                inverti(i, j);
        }
    }

    if (won())
        return WON;
    else if (lost())
        return LOST;
    else
        return OK;
}

int main(void)
{
    Move outcome;

    init_board();
    print_board();
    do
    {
        int k;
        printf("Your move ( <0 to resign)? ");
        scanf("%d", &k);
        if (k < 0)
        {
            outcome = RESIGNED;
        }
        else
        {
            outcome = shoot(k);
            if (outcome == INVALID)
            {
                printf("\nInvalid move\n");
            }
            print_board();
        }
    } while (outcome == OK || outcome == INVALID);
    switch (outcome)
    {
    case WON:
        printf("You won!\n");
        break;
    case LOST:
        printf("Sorry, you lost\n");
        break;
    default:
        printf("Resigned\n");
    }
    return EXIT_SUCCESS;
}
