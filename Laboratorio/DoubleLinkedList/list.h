/****************************************************************************
 *
 * list.h -- Interfaccia lista concatenata con sentinella
 *
 * Copyright (C) 2021, 2023, 2024 Moreno Marzolla
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

#ifndef LIST_H
#define LIST_H

typedef int ListInfo;

typedef struct ListNode {
    ListInfo val;
    struct ListNode *succ, *pred;
} ListNode;

typedef struct {
    int length;
    ListNode *sentinel;
} List;

/* Crea una lista, inizialmente vuota. */
List *list_create( void );

/* Restituisce la lunghezza (numero di nodi) della lista `L`; se `L` è
   la lista vuota, restituisce 0 */
int list_length(const List *L);

/* Svuota la lista L; questa funzione deve liberare la memoria
   occupata da tutti i nodi di `L`. */
void list_clear(List *L);

/* Distrugge la lista `L` e libera tutta la memoria occupata dai nodi
   di `L` e dalla struttura puntata da `L`; dopo questa funzione non
   si può più accedere alla memoria puntata da `L`, perché è stata
   deallocata. */
void list_destroy(List *L);

/* Restituisce true (un valore diverso da zero) se la lista è vuota, 0
   altrimenti */
int list_is_empty(const List *L);

/* Restituisce il puntatore al primo nodo contenente `k` in `L`; se
   `k` non è presente, restituisce un puntatore alla sentinella */
ListNode *list_search(const List *L, ListInfo k);

/* Restituisce il puntatore al primo nodo di `L`; se `L` è vuota,
   restituisce un puntatore alla sentinella */
ListNode *list_first(const List *L);

/* Restituisce il puntatore all'ultimo nodo di `L`; se `L` è vuota,
   restituisce un puntatore alla sentinella */
ListNode *list_last(const List *L);

/* Inserisce un nuovo nodo contenente `k` all'inizio della lista */
void list_add_first(List *L, ListInfo k);

/* Inserisce un nuovo nodo contenente `k` alla fine della lista */
void list_add_last(List *L, ListInfo k);

/* Rimuove il nodo `n` dalla lista, liberando anche la memoria
   occupata da `n`. */
void list_remove(List *L, ListNode *n);

/* Rimuove il primo nodo della lista e ne restituisce il valore */
ListInfo list_remove_first(List *L);

/* Rimuove l'ultimo nodo della lista e ne restituisce il valore */
ListInfo list_remove_last(List *L);

/* Restituisce il successore del nodo `n`; se `n` è l'ultimo nodo
   della lista, restituisce un puntatore alla sentinella. */
ListNode *list_succ(const ListNode *n);

/* Restituisce il predecessore del nodo `n`; se `n` è il primo nodo
   della lista, restituisce un puntatore alla sentinella. */
ListNode *list_pred(const ListNode *n);

/* Ritorna un puntatore all'n-esimo elemento di `L` (n = 0 indica il
   primo elemento della lista). Se l'elemento non esiste, cioè se n <
   0 oppure n >= list_length(L), ritorna un puntatore alla
   sentinella */
ListNode *list_nth_element(const List *L, int n);

/* Concatena gli elementi di `L2` in coda a quelli di `L1`. Questa
   funzione non crea nuovi nodi, ma modificare `L1` e `L2`. Al termine
   di questa funzione, `L2` deve diventare la lista vuota. */
void list_concat(List *L1, List *L2);

/* Ritorna true se e solo se le liste `L1` e `L2` contengono gli stessi
   valori, nello stesso ordine. */
int list_equal(const List *L1, const List *L2);

/* Stampa il contenuto della lista */
void list_print(const List *L);

/* Ritorna l'indirizzo di memoria occupato dalla sentinella */
ListNode* list_end(const List *L);

#endif
