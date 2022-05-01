/****************************************************************************
 *
 * minheap.c -- Min-Heap binario
 *
 * Copyright (C) 2021, 2022 Moreno Marzolla
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
% LabASD - Min-Heap binario
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2022-03-19

Scopo di questo esercizio è implementare un _min-heap binario_;
formuleremo il problema in modo leggermente diversa da quanto visto a
lezione (e descritto nel libro di testo), perché questa struttura dati
ci servirà in futuro.

Un min-heap è un contenitore di coppie (`key`, `prio`), di capienza
massima `size`, dove

- `key` è un intero compreso tra 0 e (`size`-1) inclusi,

- `prio` è un numero reale che rappresenta la priorità associata a quella chiave. Lo heap deve mantenere la proprietà di ordinamento parziale rispetto alla priorità.

> **Attenzione**: nel libro, `key` indica la priorità rispetto alla
> quale mantenere la proprietà di ordinamento parziale dello heap; qui
> invece `key` rappresenta un dato satellite, e `prio` è l'attributo
> in base al quale ordinare. Questa deviazione da quanto descritto nel
> libro ci tornerà utile quando implementeremo gli algoritmi di
> Dijkstra e di Prim.

Ogni chiave può apparire al più una volta; le priorità invece sono
numeri reali arbitrari (anche negativi), e non sono soggette ad alcun
vincolo. Lo heap supporta la ricerca efficiente della coppia (`key`,
`prio`) con `prio` minima.

Un min-heap viene rappresentato come un array di strutture `HeapElem`:

```C
typedef struct {
    int key;
    double prio;
} HeapElem;

typedef struct {
    HeapElem *heap;
    int n;
    int size;
} MinHeap;
```

dove `size` è la dimensione dell'array `heap[]`, e `n` è il numero di
coppie (`key`, `prio`) effettivamente presenti nello heap in un dato
momento. Si deve sempre avere $0 \leq n \leq \texttt{size}$.

In un min-heap ogni nodo soddisfa la seguente proprietà:

> La priorità di un nodo è _minore o uguale_ a quella di entrambi i
> figli (se esistono)

Il file [minheap.h](minheap.h) contiene la specifica dell'interfaccia
della struttura dati min-heap. Si chiede di realizzare le funzioni
indicate, lasciando `minheap_change_prio()` per ultima in quanto è la
più laboriosa. È sempre possibile definire ulteriori funzioni di
supporto che si ritengano utili.

La Figura 1 mostra un esempio di min-heap con capienza massima `size =
8` contenente $n = 5$ elementi. L'attributo `prio` di ogni nodo ha
valore minore o uguale a quello dei figli (se presenti); le chiavi
invece sono dati satellite, che non entrano in gioco nel mantenimento
dell'ordine nello heap. Lo heap è rappresentato da un array di `size`
elementi, di cui i primi `n` contengono le coppie (chiave, priorità)
effettivamente presenti, mentre i restanti hanno contenuto indefinito.

![Figura 1: Esempio di min-heap di capienza massima `size = 8` contenente $n = 5$ elementi, e sua rappresentazione tramite array](minheap.png)

> **Attenzione**: le formule date a lezione per individuare l'indice
> dei figli/del padre di un nodo $i$ devono essere modificate perché
> nel linguaggio C gli array sono indicizzati a partire da zero
> anziché da uno.

L'inserimento di una nuova coppia (chiave, priorità) avviene dalla
base dello heap; più in dettaglio, la nuova coppia viene inserita
nell'elemento `heap[n]` e si incrementa $n$. Successivamente, si
confronta l'attributo `prio` del nodo appena inserito con quello del
padre, effettuando uno scambio nel caso in cui venga violata la
proprietà di ordine. Gli scambi con il padre vanno ripetuti fino a
quando il nuovo nodo raggiunge il livello corretto (Figura 2).

![Figura 2: Inserimento della coppia `(key=4, prio=1)`](minheap-insert.png)

L'operazione più laboriosa è la `minheap_change_prio()`, che modifica
la priorità associata ad una chiave già presente nello heap. Se la
priorità di un nodo aumenta, occorre applicare l'operazione
`heapify()` vista a lezione per far "scendere" il nodo fino al livello
corretto (Figura 3).

![Figura 3: `minheap_change_prio(h, 0, 31.0)`](minheap-change-prio1.png)

Nel caso in cui la priorità diminuisca, potrebbe rendersi necessario
effettuare scambi con il padre fino a quando il nuovo nodo raggiunge
il livello corretto, in modo simile a quanto fatto durante
l'inserimento (Figura 4).

![Figura 4: `minheap_change_prio(h, 7, -2.0)`](minheap-change-prio2.png)

In uno heap binario con $n$ elementi, le operazioni possono essere
implementate con i costi asintotici riportati nella Tabella 1.

: Tabella 1: Costi asintotici delle operazioni su uno heap binario

Operazione                     Costo
-----------------------------  --------------
`minheap_min()`                $O(1)$
`minheap_delete_min()`         $O(\log n)$
`minheap_insert()`             $O(\log n)$
`minheap_change_prio()`        $O(n)$[^1]
-----------------------------  --------------

[^1]: Per iniziare, si realizzi la funzione `minheap_change_prio()`
mediante una scansione lineare dell'array `heap[]` per cercare la
posizione dell'elemento di chiave data. In questo documento vengono
forniti [alcuni suggerimenti](#suggerimenti) per ridurre a $O(\log n)$
il costo asintotico di questa operazione.

Il programma [minheap-main.c](minheap-main.c) contiene una funzione
`main()` che legge da standard input una sequenza di comandi, uno per
ogni riga, che manipolano uno heap inizialmente vuoto. I comandi sono
descritti nella Tabella 2.

: Tabella 2: Comandi nel file di input

----------------------------------------------------------------------------
Comando         Significato
--------------  ------------------------------------------------------------
_n_             Inizializza lo heap per contenere al più _n_ elementi;
                questa istruzione compare una sola volta all'inizio

`+` _key prio_  Inserisce la chiave _key_ con priorità _prio_

`-`             Cancella la coppia <_key, prio_> con priorità minima

`?`             Stampa la chiave associata alla priorità minima

`c` _key prio_  Modifica la priorità associata alla chiave _key_

`s`             Stampa il numero $n$ di elementi presenti nello heap

`p`             Stampa il contenuto dello heap (per debug)
----------------------------------------------------------------------------

Per compilare;

        gcc -std=c90 -Wall -Wpedantic minheap.c minheap-main.c -o minheap-main

Per eseguire:

        ./minheap-main < minheap.in

oppure

        ./minheap-main minheap.in

## Suggerimenti

Implementare `minheap_change_prio(h, key, newprio)` in modo efficiente
richiede attenzione, perché trovare il nodo dello heap contenente la
chiave data `key` richiede tempo $O(n)$ nel caso peggiore; la chiave,
infatti, potrebbe trovarsi in qualsiasi posizione. Una volta trovato
il nodo, serve tempo $O(\log n)$ per spostarlo verso l'alto o verso il
basso fino a raggiungere il nuovo livello corretto. Quindi il costo
asintotico di `minheap_change_prio()` è dominato dalla ricerca della
chiave, che è l'operazione più lenta.

Sfruttando il fatto che le chiavi appartengono all'insieme $\{0,
\ldots, \texttt{size}-1\}$, e che ogni chiave compare al più una
volta, possiamo effettuare la ricerca in tempo $O(1)$ utilizzando un
secondo array `pos[]` di lunghezza `size`. `pos[k]` è la posizione
(l'indice) dell'elemento di chiave `k` nell'array `heap[]`, se
presente (Figura 5). Quindi, per ogni chiave `k` presente nello heap
deve valere la proprietà:

        heap[pos[k]].key == k

Se `k` non è presente, si pone porre `pos[k] = -1`.

![Figura 5: Uso dell'array `pos[]` per indicare la posizione (indice)
delle chiavi nell'array `heap[]`](minheap-pos.png)

Il contenuto di `pos[]` può essere mantenuto aggiornato in tempo
$O(1)$ man mano che gli elementi in `heap[]` vengono scambiati tra
loro durante le varie operazioni sullo heap. Ciò richiede un po' di
attenzione, ma non è particolarmente problematico.

## File

- [minheap.c](minheap.c)
- [minheap.h](minheap.h)
- [minheap-main.c](minheap-main.c)
- [minheap.in](minheap.in) ([output atteso](minheap.out))
- [minheap1.in](minheap1.in)
- [minheap2.in](minheap2.in)
- [minheap3.in](minheap3.in)

 ***/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include "minheap.h"

 

int parent(const MinHeap *h, int position){
  return floor(position / 2.0); 
}

void minheap_print(const MinHeap *h)
{
    int i, j, width = 1;

    assert(h != NULL);

    printf("\nn=%d size=%d\n", h->n, h->size);
    i = 0;
    while (i < h->n) {
        j = 0;
        while (j<width && i < h->n) {
            printf("h[%d]=(%d %f) ", i, h->heap[i].key, h->heap[i].prio);
            i++;
            j++;
        }
        printf("\n");
        width *= 2;
    }
}

void minheap_clear( MinHeap *h )
{
    assert(h != NULL);
    h->n = 0;
}

/* Costruisce un min-heap vuoto che può contenere al massimo
   size elementi */
MinHeap *minheap_create(int size)
{
    MinHeap *h = (MinHeap*)malloc(sizeof(*h));
    assert(h != NULL);

    h->size = size;
    h->heap = (HeapElem*)malloc(size * sizeof(*(h->heap)));
    assert(h->heap != NULL);
    minheap_clear(h);
    return h;
}


void minheap_heapify(const MinHeap *h, int position){
  HeapElem tmp;
  int l = left(h, position);
  int r = right(h , position);
  /* Getting the lower element */
  int lower = position;
  if(l <= h->n && h->heap[l].key <= h->heap[position].key)  lower = l;
  if( r <= h->n && h->heap[r].key <= h->heap[position].key ) lower = r;
  /* if the parent node is minor than one of his child excange the elements */
  if(lower != position){
    tmp = h->heap[position];
    h->heap[position] = h->heap[lower];
    h->heap[lower] = tmp;
    minheap_heapify(h, lower);
  }
}

void minheap_destroy( MinHeap *h )
{
    assert(h != NULL);

    h->n = h->size = 0;
    free(h->heap);
    free(h);
}

/* Restituisce true (nonzero) se lo heap è vuoto */
int minheap_is_empty(const MinHeap *h)
{
    assert(h != NULL);

    return (h->n == 0);
}

/* Restituisce true (nonzero) se lo heap è pieno, cioè è stata
   esaurita la capienza a disposizione */
int minheap_is_full(const MinHeap *h)
{
    assert(h != NULL);

    return (h->n == h->size);
}

/* Restituisce il numero di elementi presenti nello heap */
int minheap_get_n(const MinHeap *h)
{
    assert(h != NULL);

    return h->n;
}

/* Restituisce la chiave associata alla priorità minima */
int minheap_min(const MinHeap *h)
{
    assert( !minheap_is_empty(h) );

    return h->heap[0].key;
}

/* Inserisce una nuova coppia (key, prio) nello heap. */
void minheap_insert(MinHeap *h, int key, double prio){
  if(minheap_is_full(h)){
    /* Radoppia la dimensione dello heap */
    h->heap = (HeapElem *)realloc(h->heap , 2 * h->size);
    h->size *= 2;
  }
  assert(!minheap_is_full(h));
  if(minheap_is_empty(h)){
    /* Inserimento del primo elemento dello heap */
    assert( h->heap != NULL );
    h->heap->key = key;
    h->heap->prio = prio;
  }
  else{
    HeapElem *new_elem = &(h->heap[h->n]);
    new_elem->key = key;
    /* Massimo valore rappresentabile = infinito (DA FIXARE )*/
    new_elem->prio = DBL_MAX;
    /* Incrementa di uno il numero di elementi presenti nell'array */
    h->n += 1;
    /* Colloca l'elemento nella giusta posizione */
    minheap_decrease_prio(h , h->n - 1  , prio);
  }
}


void minheap_decrease_prio(MinHeap *h , int position , double prio){
  HeapElem tmp;
  h->heap[position].prio = prio;
  while(position >= 0 && h->heap[parent(h , position)].prio <= h->heap[position].prio){
    /* Cambia il nodo del padre con quello del figlio */
    tmp = h->heap[position];
    h->heap[position] = h->heap[parent(h , position)];
    h->heap[parent(h, position)] = tmp;
    /* Ora la posizione del nodo risulta essere quella del vecchio nodo padre */
    position = parent(h , position);
  }
}



/* Rimuove la coppia (chiave, priorità) con priorità minima */
int minheap_delete_min(MinHeap *h)
{
  if(!minheap_is_empty(h)){
    h->heap[0] = h->heap[h->n - 1];
    minheap_heapify(h,0); 
    return 1;
  }
    return -1;
}

/* Modifica la priorità associata alla chiave key. La nuova priorità
   può essere maggiore, minore o uguale alla precedente. */
void minheap_change_prio(MinHeap *h, int key, double newprio)
{
  double oldprio = h->heap[h->pos[key]].prio;
  if(oldprio > newprio) minheap_increase_prio(h, h->pos[key] , newprio);
  if(oldprio < newprio) minheap_decrease_prio(h, h->pos[key], newprio);  
}
