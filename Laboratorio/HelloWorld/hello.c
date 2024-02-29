/****************************************************************************
 *
 * hello.c -- Esempio "hello world"
 *
 * Copyright (C) 2021, 2022, 2023, 2024 Moreno Marzolla
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
% LabASD - hello.c
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-02-22

Questo esercizio ha lo scopo di fare pratica con la compilazione di
file multipli per produrre un singolo eseguibile, e il passaggio di
parametri sulla riga di comando. Il file [hello.c](hello.c) contiene
la funzione `void say_hello(const char *s)`, il cui prototipo è
dichiarato in [hello.h](hello.h); la funzione stampa un messaggio a
video. Il file [hello-main.c](hello-main.c) contiene la funzione
`main()` che invoca `say_hello()` passando come parametro la stringa
presente sulla riga di comando. I sorgenti vanno compilati per
produrre un unico eseguibile; usando la riga di comando è possibile
compilare con:

        gcc -std=c90 -Wall -Wpedantic hello.c hello-main.c -o hello-main

In ambiente Linux/MacOSX è possibile eseguire da riga di comando con:

        ./hello-main parola

In ambiente Windows è possibile eseguire da riga di comando,
spostandosi nella directory (cartella) contenente l'eseguibile e
digitando:

        .\hello-main parola

## File

- [hello.c](hello.c)
- [hello.h](hello.h)
- [hello-main.c](hello-main.c)

***/

#include <stdio.h>
#include "hello.h"

void say_hello( const char *s )
{
    printf("Hello, %s!\n", s);
}
