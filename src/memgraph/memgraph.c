/*
 * github.com/nrizzo
 *
 * Implementazione del codice che si occupa della costruzione del grafo di
 * appartenenza di un insieme ereditariamente finito.
 */

#include <stdlib.h> /* malloc e free */
#include <stdio.h> /* printf */
#include <inttypes.h> /* uint32_t, uint64_t */

#include "../bignat/bignat.h"
#include "../interval/interval.h"
#include "../list/list.h"
#include "memnode/memnode.h"
#include "memgraph.h"

/* Dichiarazioni funzioni ausiliarie */
/*
 * children, dato il nodo di un grafo di appartenenza in costruzione, crea e
 * restituisce la lista dei bignat rappresentanti i codici dei figli del nodo,
 * ordinati dal più piccolo al più grande; la lista va eliminata da
 * dllb_destroy, i bignat vengono trattati dalla procedura join.
 */
static struct dl_list_b *children(struct memnode *x);

/*
 * join, data la lista V dei vertici di un grafo di appartenenza in costruzione,
 * il nodo x e la lista l dei codici dei figli di x, si occupa di aggiungere
 * i nodi (se non presenti) e gli archi collegati ai figli di x, restituendo
 * la lista dei vertici aggiornata; elimina o salva i puntatori ai bignat di l.
 */
static struct dl_list_n *join(struct dl_list_n *V, struct memnode *x,
	struct dl_list_b *l);

/*
 * calcrack, dati il nodo x di un grafo di appartenenza in costruzione, sui
 * figli di cui è già stata chiamata calcrack, e gli interi de, dn e da, calcola
 * l'approssimazione del codice della variante di Ackermann di x con le opzioni
 * specificate dagli interi.
 */
static void calcrack(struct memnode *x, long long de, long long dn,
	long long da);

/*
 * calcrack, data la lista di adiacenza di vertici del grafo di appartenenza,
 * di cui e' gia' stato calcolato il codice della variante di Ackermann, calcola
 * l'approssimazione del codice della variante di Ackermann del vertice con tali
 * figli con le opzioni specificate dagli interi de, dn e da.
 */
static struct interval *dlln_calcrack(struct dl_list_n *l, long long de,
	long long dn, long long da);


struct memgraph *memgraph_create(struct dl_list_b *l)
{
	struct memgraph *G;
	struct dl_list_b *scan;
	struct memnode *x;

	G = malloc(sizeof(struct memgraph));
	G->V = dlln_create();
	G->xx = dlln_create();
	G->rcode = NULL;

	scan = l;
	while (!dllb_isempty(scan)) {
		x = memnode_create(dllb_get(scan));
		G->V = dlln_add(G->V, x);
		G->xx = dlln_add(G->xx, x);

		scan = dllb_next(scan);
	}

	return G;
}

void memgraph_build(struct memgraph *G)
{
	struct dl_list_n *scan;
	struct dl_list_b *l;
	struct memnode *x;

	scan = G->V;
	while (!dlln_isempty(scan)) {
		x = dlln_get(scan);

		l = children(x);
		G->V = join(G->V, x, l);
		dllb_destroy(l);

		scan = dlln_next(scan);
	}
}

void memgraph_calcrack(struct memgraph *G, long long de, long long dn, long long da)
{
	struct dl_list_n *l;
	struct interval *i;
	struct memnode *x;

	l = dlln_last(G->V);
	while (!dlln_isempty(l)) {
		x = dlln_get(l);

		calcrack(x, de, dn, da);

		l = dlln_prev(l);
	}

	G->rcode = dlln_calcrack(G->xx, de, dn, da);
}

void memgraph_printDOT(struct memgraph *G)
{
	struct dl_list_n *scan;
	struct dl_list_n *scanx, *scany;

	printf("digraph G_T {\n");

	/* vertici */
	scan = G->V;
	while (!dlln_isempty(scan)) {
		bn_print(memnode_get(dlln_get(scan)));
		printf(" [label=\"");
		bn_print(memnode_get(dlln_get(scan)));
		printf("\\n");
		interval_print(dlln_get(scan)->rcode);
		printf("\"];\n");

		scan = dlln_next(scan);
	}

	/* archi */
	scanx = G->V;
	while (!dlln_isempty(scanx)) {
		scany = memnode_adj(dlln_get(scanx));
		while (!dlln_isempty(scany)) {
			bn_print(memnode_get(dlln_get(scanx)));
			printf(" -> ");
			bn_print(memnode_get(dlln_get(scany)));
			printf(";\n");

			scany = dlln_next(scany);
		}

		scanx = dlln_next(scanx);
	}

	printf("}\n");
}

void memgraph_destroy(struct memgraph *G)
{
	struct dl_list_n *scan;
	struct memnode *x;

	scan = G->V;
	while (!dlln_isempty(scan)) {
		x = dlln_get(scan);

		memnode_destroy(x);
		
		scan = dlln_next(scan);
	}

	dlln_destroy(G->V);
	dlln_destroy(G->xx);

	if (G->rcode != NULL)
		interval_destroy(G->rcode);

	free(G);
}

/* Implementazione funzioni ausiliarie */
static struct dl_list_b *children(struct memnode *v)
{
	struct dl_list_b *l = dllb_create();

	for (uint64_t i = bn_bits(v->code) - 1; i < UINT64_MAX; i--) {
		if (bn_get(v->code, i))
			l = dllb_add(l, bn_fromuint64(i));
	}

	return l;
}

static struct dl_list_n *join(struct dl_list_n *V, struct memnode *x,
	struct dl_list_b *l)
{
	/* l lista dei codici dei figli di x (decrescenti) */
	struct dl_list_n *scanV;
	struct dl_list_b *scanl;
	struct memnode *y;

	if (dllb_isempty(l))
		return V;

	scanV = dlln_last(V);
	y = dlln_get(scanV);
	scanl = dllb_last(l);
	while (!dllb_isempty(scanl)) {
		if (bn_cmp(memnode_get(y), dllb_get(scanl)) == 0) {
			/* il vertice esiste già */
			memnode_addarc(x,y);
			scanl = dllb_prev(scanl);
		} else if (bn_cmp(memnode_get(y), dllb_get(scanl)) >= 0) {
			/* il vertice non esiste */
			V = dlln_insert(V, memnode_create(dllb_get(scanl)), scanV);
			memnode_addarc(x,dlln_get(dlln_next(scanV)));
			scanl = dllb_prev(scanl);
		} else {
			/* non è figlio di x */
			scanV = dlln_prev(scanV);
			y = dlln_get(scanV);
		}
	}

	return V;
}

static void calcrack(struct memnode *x, long long de, long long dn,
	long long da)
{
	struct interval *res; /* risultato */
	struct interval *ti;
	struct dl_list_n *l; /* lista di adiacenza di x */
	struct memnode *y; /* figlio di x  */

	res = interval_zero();

	if (dlln_isempty(x->adj)) {
		x->rcode = res;
		return;
	}

	l = x->adj;
	while (!dlln_isempty(l)) {
		y = dlln_get(l);

		if (y->rsingleton == NULL)
			y->rsingleton = interval_rec_exp_2(y->rcode,de,dn,da);

		ti = res;
		res = interval_add(res, y->rsingleton);
		interval_destroy(ti);

		l = dlln_next(l);
	}

	x->rcode = res;
}

static struct interval *dlln_calcrack(struct dl_list_n *l, long long de,
	long long dn, long long da)
{
	struct interval *res; /* risultato */
	struct interval *ti;
	struct dl_list_n *scan;
	struct memnode *x; /* figlio di h_n */

	res = interval_zero();

	scan = l;
	while (!dlln_isempty(scan)) {
		x = dlln_get(scan);

		if (x->rsingleton == NULL)
			x->rsingleton = interval_rec_exp_2(x->rcode,de,dn,da);

		ti = res;
		res = interval_add(res, x->rsingleton);
		interval_destroy(ti);

		scan = dlln_next(scan);
	}

	return res;
}
