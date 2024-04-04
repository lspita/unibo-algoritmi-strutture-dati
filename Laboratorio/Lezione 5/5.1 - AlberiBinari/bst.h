/****************************************************************************
 *
 * bst.h -- Interfaccia per alberi binari di ricerca (non bilanciati)
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

#ifndef BST_H
#define BST_H

typedef int BSTKey;

/* Struttura che rappresenta un nodo del BST */
typedef struct BSTNode {
    BSTKey key;
    struct BSTNode *parent, *left, *right;
} BSTNode;

/* Struttura che rappresenta un BST; "size" indica il numero di nodi
   dell'albero, in modo che sia possibile avere tale informazione in
   tempo O(1) senza bisogno di contarli ogni volta. */
typedef struct {
    BSTNode *root;
    int size;
} BST;

/* Restituisce un nuovo BST inizialmente vuoto */
BST *bst_create( void );

/* Svuota l'albero rimuovendo tutti i nodi in esso contenuto; è
   necessario liberare la memoria occupata dai nodi. Al termine di
   questa operazione, `T` rappresenta l'albero vuoto (cioè T->size ==
   0). */
void bst_clear(BST *T);

/* Distrugge l'albero e il suo contenuto, liberando tutta la memoria
   occupata dalla struttura `T` e da tutti i nodi eventualmente
   presenti. Dopo aver chiamato questa funzione il puntatore `T` punta
   ad un blocco di memoria non più valido, quindi non deve essere
   dereferenziato. */
void bst_destroy(BST *T);

/* Inserisce la chiave `k` in `T`. Non è ammessa la presenza di chiavi
   duplicate, per cui se una chiave è già presente la funzione non
   deve modificare l'albero. Ritorna 1 se la chiave è stata inserita,
   0 se era già presente. */
int bst_insert(BST *T, BSTKey k);

/* Restituisce il puntatore ad un nodo contenente la chiave `k`; se la
   chiave non è presente, restituisce `NULL`. */
BSTNode *bst_search(const BST *T, BSTKey k);

/* Cancella il nodo `n` da `T`.

   Precondizione: `n` deve puntare ad un nodo di `T` (`n` non può essere NULL). */
void bst_delete(BST *T, BSTNode *n);

/* Restituisce il numero di nodi in `T`. */
int bst_size(const BST *T);

/* Restituisce `true` (un valore diverso da zero) se l'albero è vuoto,
   0 altrimenti */
int bst_is_empty(const BST *T);

/* Restituisce l'altezza dell'albero `T`. L'altezza di un albero è il
   massimo numero di _archi_ che bisogna attraversare per raggiungere
   una delle sue foglie partendo dalla radice; in altre parole,
   l'altezza di un albero è la profondità massima tra le sue foglie.

   L'altezza di un albero vuoto si considera -1; l'altezza di un
   albero composto dalla sola radice si considera 0. */
int bst_height(const BST *T);

/* Stampa a video il contenuto dell'albero `T`. */
void bst_print(const BST *T);

/* Stampa a video il contenuto dell'albero `T` in modo più "leggibile". */
void bst_pretty_print(const BST *T);

#endif
