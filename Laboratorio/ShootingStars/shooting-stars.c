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
ogni cella, ed è rappresentato con un `#` dalle figure seguenti:

```
0#.  #1#  .#2
##.  ...  .##
...  ...  ...

#..  .#.  ..#
3..  #4#  ..5
#..  .#.  ..#

...  ...  ...
##.  ...  .##
6#.  #7#  .#8
```

quindi il vicinato della cella 0 è costituita da (1, 3, 4); il
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

(infatti, a questo punto non sarebbe possibile far esplodere alcuna
stella).

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

#define STAR 1
#define STAR_SYMBOL '*'

#define BLACK_HOLE 0
#define BLACK_HOLE_SYMBOL '.'

#define FIELD_DIMENSION 3
#define CENTER ((int)(FIELD_DIMENSION / 2)) /* floored dimension / 2 */

#define LOSS 0
#define WIN 1
#define CONTINUE 2

typedef struct Cell
{
    int row;
    int col;
} Cell;

Cell new_cell(int row, int col)
{
    Cell c;
    c.row = row;
    c.col = col;

    return c;
}

Cell index_to_cell(int index)
{
    return new_cell(index / FIELD_DIMENSION, index % FIELD_DIMENSION);
}

int cell_to_index(int row, int col)
{
    return (row * FIELD_DIMENSION) + col;
}

void print_field(int (*const field)[FIELD_DIMENSION])
{
    int i, j;
    for (i = 0; i < FIELD_DIMENSION; i++)
    {
        for (j = 0; j < FIELD_DIMENSION; j++)
        {
            switch (field[i][j])
            {
            case STAR:
                printf("%d", cell_to_index(i, j));
                break;
            case BLACK_HOLE:
                printf("%c", BLACK_HOLE_SYMBOL);
                break;
            }
            putchar(' ');
        }
        putchar('\n');
    }
    putchar('\n');
}

int in_bounds(const Cell *const cell)
{
    return (cell->row >= 0 && cell->row < FIELD_DIMENSION &&
            cell->col >= 0 && cell->col < FIELD_DIMENSION);
}

Cell read_cell(int (*const field)[FIELD_DIMENSION])
{
    int index = -1;
    Cell cell = {-1, -1};

    do
    {
        int i, j;

        printf("Cell to explode [");
        for (i = 0; i < FIELD_DIMENSION; i++)
        {
            for (j = 0; j < FIELD_DIMENSION; j++)
            {
                if (field[i][j] == STAR)
                {
                    printf("%d, ", cell_to_index(i, j));
                }
            }
        }
        printf("\b\b]: ");

        scanf("%d", &index);
        cell = index_to_cell(index);
    } while (!in_bounds(&cell) || field[cell.row][cell.col] != STAR);

    return cell;
}

void explode_cell(const Cell *const cell, int (*const field)[FIELD_DIMENSION])
{
    switch (field[cell->row][cell->col])
    {
    case STAR:
        field[cell->row][cell->col] = BLACK_HOLE;
        break;
    case BLACK_HOLE:
        field[cell->row][cell->col] = STAR;
        break;
    }
}

int is_angle(const Cell *const cell)
{
    return ((cell->row == 0 || cell->row == FIELD_DIMENSION - 1) &&
            (cell->col == 0 || cell->col == FIELD_DIMENSION - 1));
}

void explode(const Cell *const cell, int (*field)[FIELD_DIMENSION])
{
    int angle;
    Cell bounds[8];

    bounds[0] = new_cell(cell->row - 1, cell->col);     /* up */
    bounds[1] = new_cell(cell->row + 1, cell->col);     /* down */
    bounds[2] = new_cell(cell->row, cell->col - 1);     /* left */
    bounds[3] = new_cell(cell->row, cell->col + 1);     /* right */
    bounds[4] = new_cell(cell->row - 1, cell->col - 1); /* nw */
    bounds[5] = new_cell(cell->row - 1, cell->col + 1); /* ne */
    bounds[6] = new_cell(cell->row + 1, cell->col - 1); /* sw */
    bounds[7] = new_cell(cell->row + 1, cell->col + 1); /* se */

    explode_cell(cell, field);
    angle = is_angle(cell);

    if (angle)
    {
        /* explode everithing around in bounds */
        int i;
        for (i = 0; i < 8; i++)
        {
            const Cell *const current = &(bounds[i]);
            if (in_bounds(current))
            {
                explode_cell(current, field);
            }
        }
    }
    else
    {
        /* explode only if both vertical or both horizontal are in bounds */
        const Cell *const up_ptr = &(bounds[0]), *const down_ptr = &(bounds[1]);
        const Cell *const left_ptr = &(bounds[2]), *const right_ptr = &(bounds[3]);

        if (in_bounds(up_ptr) && in_bounds(down_ptr))
        {
            explode_cell(up_ptr, field);
            explode_cell(down_ptr, field);
        }

        if (in_bounds(left_ptr) && in_bounds(right_ptr))
        {
            explode_cell(left_ptr, field);
            explode_cell(right_ptr, field);
        }
    }
}

int check_status(int (*field)[FIELD_DIMENSION])
{
    int count_blackholes = 0;

    int i, j;
    for (i = 0; i < FIELD_DIMENSION; i++)
    {
        for (j = 0; j < FIELD_DIMENSION; j++)
        {
            if (field[i][j] == BLACK_HOLE)
            {
                count_blackholes++;
            }
        }
    }

    if (count_blackholes == 1 && field[CENTER][CENTER] == BLACK_HOLE)
    {
        printf("\nVICTORY\n");
        return WIN;
    }
    if (count_blackholes == FIELD_DIMENSION * FIELD_DIMENSION)
    {
        printf("\nYOU LOST\n");
        return LOSS;
    }

    return CONTINUE;
}

int main(void)
{
    int field[FIELD_DIMENSION][FIELD_DIMENSION] = {
        {BLACK_HOLE, BLACK_HOLE, BLACK_HOLE},
        {BLACK_HOLE, STAR, BLACK_HOLE},
        {BLACK_HOLE, BLACK_HOLE, BLACK_HOLE},
    };

    int status = CONTINUE;
    Cell cell;

    while (status == CONTINUE)
    {
        print_field(field);
        cell = read_cell(field);
        explode(&cell, field);
        status = check_status(field);
    }

    return EXIT_SUCCESS;
}