/*
 * github.com/nrizzo
 *
 * Intestazione del codice che tratta i nodi di un grafo di appartenenza di un
 * insieme ereditariamente finito.
 */

#ifndef NODEGUARD /* guardia */
#define NODEGUARD

#include "../../bignat/bignat.h"

/*
 * Struct che rappresenta un nodo del grafo di appartenenza di x; viene salvato
 * il suo codice di Ackermann e può venire salvata l'approssimazione del codice
 * della variante a valori reali della codifica di Ackermann.
 */
struct node {
	struct bignat *code; /* codice di Ackermann */

	int indegree; /* numero di genitori */
	int outdegree; /* numero di figli */
	struct dl_list_n *adj; /* lista di adiacenza */

	struct interval *rcode; /* approssimazione variante di Ackermann */
};

/*
 * node_create, dato il numero naturale rappresentato dal bignat u, crea il
 * nodo del grafo corrispondente e ne restituisce il puntatore, senza archi; non
 * copia il bignat u ma ne salva il puntatore.
 */
struct node *node_create(struct bignat *u);

/*
 * node_addarc, dati i puntatori a due nodi x e y, aggiunge y alla lista di
 * adiacenza di x.
 */
void node_addarc(struct node *x, struct node *y);

/*
 * node_get, dato il nodo x, restituisce il puntatore al codice di Ackermann di
 * x.
 */
struct bignat *node_get(struct node *x);

/*
 * node_indegree, dato il nodo x, restituisce il numero di archi entranti.
 */
int node_indegree(struct node *x);

/*
 * node_outdegree, dato il nodo x, restituisce il numero di archi uscenti.
 */
int node_outdegree(struct node *x);

/*
 * node_adj, dato il nodo x, restituisce il puntatore alla sua lista di
 * adiacenza.
 */
struct dl_list_n *node_adj(struct node *x);

/*
 * node_destroy libera lo spazio occupato dal nodo x, dal codice di Ackermann
 * e dall'approssimazione del codice della variante di Ackermann.
 */
void node_destroy(struct node *x);

#endif /* NODEGUARD */
