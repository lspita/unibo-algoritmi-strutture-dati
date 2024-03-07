/****************************************************************************
 *
 * telefono.c -- Numero di telefono ripetuto
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
% LabASD - Numero di telefono ripetuto
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-01-23

![Tomasz Sienicki (Own work), CC BY 3.0,
<https://commons.wikimedia.org/w/index.php?curid=10330603>](phone-book.jpg
"Elenco telefonico")

Siete stati assunti come consulenti dalla IATM S.p.A. (Importante
Azienda di Telefonia Mobile) che è alle prese con il problema
seguente. L'azienda ha un file contenente $n$ numeri di telefono,
$200000 < n \leq \mathtt{MAXN}$. Tutti i numeri hanno sei cifre, la
prima delle quali può essere `3` oppure `5`; i numeri di telefono
hanno quindi la forma `3xxxxx` oppure `5xxxxx`, dove le `x` sono cifre
numeriche arbitrarie. Il contenuto del file non è necessariamente
ordinato.

Da quanto detto sopra si può dedurre che ci sia sicuramente almeno un
numero di telefono che compare più di una volta, perché il file
contiene più elementi di quanti siano i possibili numeri distinti. Il
vostro compito è di implementare un algoritmo efficiente che stampi a
video **il più piccolo numero di telefono che compare più di una
volta** nel file di input.

Il file di input ha $n$ righe, ciascuna delle quali contenente un
numero di telefono (si consideri [questo esempio](telefono.in)). Il
programma riceve il nome del file di input come unico parametro sulla
riga di comando, e deve stampare a video il minimo numero ripetuto
presente nel file.

Per compilare:

        gcc -std=c90 -Wall -Wpedantic telefono.c -o telefono

Per eseguire in ambiente Linux/MacOSX:

        ./telefono nome_file

Per eseguire in ambiente Windows:

        .\telefono nome_file

dove `nome_file` è il nome del file di input contenente l'elenco dei
numeri di telefono.

## Per approfondire

Un aspetto complicato di questo esercizio è la generazione dei file di
input (che non è richiesta, ma che ho dovuto fare io per preparare gli
input). Dato un intero $n > 200000$ e un numero di telefono $t$, come
fareste per generare un file di input composto da $n$ numeri di
telefono come sopra, non necessariamente in ordine, in cui il minimo
valore ripetuto sia esattamente $t$?

## File

- [telefono.c](telefono.c)
- [telefono.in](telefono.in) ([output atteso](telefono.out))
- [telefono1.in](telefono1.in)
- [telefono2.in](telefono2.in)
- [telefono3.in](telefono3.in)
- [telefono4.in](telefono4.in)

***/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MAXN è il massimo numero di valori che possono essere presenti nel
   file. Questo parametro si può considerare fissato, ma il programma
   dovrebbe funzionare con qualsiasi valore di MAXN */
#define MAXN 1000000

int main(int argc, char *argv[]) {
  /* [TODO] Il programma fornitosi limita a leggere e stampare il
     contenuto del file di input; lo si modifichi per risolvere il
     problema proposto, anche definendo ulteriori funzioni di
     supporto. */

  FILE *fin;
  int num_tel;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s input_file_name\n", argv[0]);
    return EXIT_FAILURE;
  }

  fin = fopen(argv[1], "r");
  if (fin == NULL) {
    fprintf(stderr, "Can not open \"%s\"\n", argv[1]);
    return EXIT_FAILURE;
  }

  while (1 == fscanf(fin, "%d", &num_tel)) {
    printf("%d\n", num_tel);
  }

  fclose(fin);
  return EXIT_SUCCESS;
}
