/****************************************************************************
 *
 * minheap.h -- Interfaccia Min-Heap binario
 *
 * Copyright (C) 2021 Moreno Marzolla
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

#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct {
    int key;
    double prio;
} HeapElem;

typedef struct {
    HeapElem *heap;
    int *pos;
    int n; /* quante coppie (chiave, prio) sono effettivamente presenti nello heap */
    int size; /* massimo numero di coppie (chiave, prio) che possono essere contenuti nello heap */
} MinHeap;

/* Crea uno heap vuoto in grado di contenere al più `size` coppie
   (chiave, priorità); le chiavi possono essere esclusivamente gli
   interi 0 .. `size` - 1; ogni chiave può essere presente nello heap
   al più una volta. Inizialmente lo heap è vuoto. */
MinHeap *minheap_create(int size);


void minheap_heapify(const MinHeap *h, int position);

/* Svuota lo heap */
void minheap_clear(MinHeap *h);

/* Dealloca la memoria occupata dallo heap h e dal suo contenuto */
void minheap_destroy(MinHeap *h);

/* Restituisce 1 se e solo se lo heap è vuoto */
int minheap_is_empty(const MinHeap *h);

/* Restituisce 1 se e solo se lo heap è pieno */
int minheap_is_full(const MinHeap *h);

/* Ritorna il numero di elementi effettivamente presenti nello heap */
int minheap_get_n(const MinHeap *h);

/* Restituisce la chiave associata alla minima priorità; non modifica
   lo heap */
int minheap_min(const MinHeap *h);

/* Ottiene il nodo padre di un nodo passato negli argomenti*/
int parent(const MinHeap *h  , int position);
int left(const MinHeap *h, int position);
int right(const MinHeap *h , int position);

/* Inserisce una nuova chiave `key` con priorità `prio`; `key` non
   deve essere già presente nello heap. */
void minheap_insert(MinHeap *h, int key , double prio);

/* Aumenta la priorita` */
void minheap_decrease_prio(MinHeap *h , int position , double new_prio);
void minheap_increase_prio(MinHeap *h , int position , double new_prio);

/* Rimuove dallo heap la coppia (chiave, prio) con priorità minima, e
   restituisce la chiave di tale coppia. */
int minheap_delete_min(MinHeap *h);

/* Modifica la priorità associata alla chiave `key`, che deve essere
   già presente nello heap. */
void minheap_change_prio(MinHeap *h, int key, double new_prio);

/* Stampa il contenuto dello heap */
void minheap_print(const MinHeap *h);

#endif
