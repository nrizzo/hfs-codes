/*
 * github.com/nrizzo
 *
 * Intestazione del codice che si occupa della costruzione del grafo di
 * appartenenza di un insieme ereditariamente finito.
 */

#ifndef GRAPHGUARD /* guardia */
#define GRAPHGUARD

#include "../bignat/bignat.h"
#include "../list/list.h"
#include "node/node.h"

/*
 * Struct che rappresenta il grafo di appartenenza di un insieme ereditariamente
 * finito; gli archi sono contenuti nelle liste di adiacenza dei singoli nodi,
 * mentre nelle due list V e xx i vertici sono salvati da quello con codice di
 * Ackermann maggiore al minore.
 */
struct graph {
	struct dl_list_n *V; /* lista dei vertici */
	struct dl_list_n *xx; /* primi figli */
	struct interval *rcode; /* codice variante Ackermann */
};

/*
 * graph_create, data la lista dei codici di Ackermann dei figli di x (bignat),
 * crea e restituisce il puntatore al grafo inizializzato con i nodi dei soli
 * figli di x, senza continuare la costruzione; dei bignat vengono salvati i
 * puntatori.
 */
struct graph *graph_create(struct dl_list_b *s);

/*
 * graph_build, dato un grafo inizializzato da graph_create, termina la
 * costruzione del grafo di appartenenza.
 */
void graph_build(struct graph *G);

/*
 * graph_calcrack, dati un grafo di appartenenza già costruito e gli interi de,
 * dn e da, calcola per ogni vertice un'approssimazione del codice della
 * variante della codifica di Ackermann con le opzioni specificate dagli interi.
 */
void graph_calcrack(struct graph *G, long long de, long long dn, long long da);

/*
 * graph_printDOT, dato un grafo di appartenenza, ne stampa la rappresentazione
 * del grafo in formato DOT, assieme al codice di Ackermann e al codice della
 * variante per ogni vertice.
 */
void graph_printDOT(struct graph *G);

/*
 * graph_destroy libera lo spazio occupato dal grafo di appartenenza G e quello
 * di tutti i suoi vertici.
 */
void graph_destroy(struct graph *G);

#endif
