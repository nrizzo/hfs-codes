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
#include "node/node.h"
#include "graph.h"

/* Dichiarazioni funzioni ausiliarie */
/*
 * children, dato il nodo di un grafo di appartenenza in costruzione, crea e
 * restituisce la lista dei bignat rappresentanti i codici dei figli del nodo,
 * ordinati dal più piccolo al più grande; la lista va eliminata da
 * dllb_destroy, i bignat vengono trattati dalla procedura join.
 */
static struct dl_list_b *children(struct node *x);

/*
 * join, data la lista V dei vertici di un grafo di appartenenza in costruzione,
 * il nodo x e la lista l dei codici dei figli di x, si occupa di aggiungere
 * i nodi (se non presenti) e gli archi collegati ai figli di x, restituendo
 * la lista dei vertici aggiornata; elimina o salva i puntatori ai bignat di l.
 */
static struct dl_list_n *join(struct dl_list_n *V, struct node *x,
	struct dl_list_b *l);

/*
 * calcrack, dati il nodo x di un grafo di appartenenza in costruzione, sui
 * figli di cui è già stata chiamata calcrack, e gli interi de, dn e da, calcola
 * l'approssimazione del codice della variante di Ackermann di x con le opzioni
 * specificate dagli interi.
 */
static void calcrack(struct node *x, long long de, long long dn,
	long long da);


struct graph *graph_create(struct dl_list_b *u)
{
	struct graph *G;
	struct dl_list_b *scan;
	struct node *x;

	G = malloc(sizeof(struct graph));
	G->V = dlln_create();
	G->xx = dlln_create();
	G->rcode = NULL;

	scan = dllb_last(u);
	while (!dllb_isempty(scan)) {
		x = node_create(dllb_get(scan));
		G->V = dlln_add(G->V, x);
		G->xx = dlln_add(G->xx, x);

		scan = dllb_prev(scan);
	}

	return G;
}

void graph_build(struct graph *G)
{
	struct dl_list_n *scan;
	struct dl_list_b *l;
	struct node *x;

	scan = G->V;
	while (!dlln_isempty(scan)) { /* la lista viene modificata.. */
		x = dlln_get(scan);

		l = children(x);
		G->V = join(G->V, x, l);
		dllb_destroy(l);

		scan = dlln_next(scan);
	}
}

void graph_calcrack(struct graph *G, long long de, long long dn, long long da)
{
	struct dl_list_n *l;
	struct interval *i;
	struct node *x;

	l = dlln_last(G->V);
	while (!dlln_isempty(l)) {
		x = dlln_get(l);

		calcrack(x, de, dn, da);

		l = dlln_prev(l);
	}
}

void graph_printDOT(struct graph *G)
{
	struct dl_list_n *scan;
	struct dl_list_n *scanx, *scany;

	printf("digraph G_T {\n");

	/* vertici */
	scan = G->V;
	while (!dlln_isempty(scan)) {
		bn_print(node_get(dlln_get(scan)));
		printf(" [label=\"");
		bn_print(node_get(dlln_get(scan)));
		printf("\\n");
		interval_print(dlln_get(scan)->rcode);
		printf("\"];\n");

		scan = dlln_next(scan);
	}

	/* archi */
	scanx = G->V;
	while (!dlln_isempty(scanx)) {
		scany = node_adj(dlln_get(scanx));
		while (!dlln_isempty(scany)) {
			bn_print(node_get(dlln_get(scanx)));
			printf(" -> ");
			bn_print(node_get(dlln_get(scany)));
			printf(";\n");

			scany = dlln_next(scany);
		}

		scanx = dlln_next(scanx);
	}

	printf("}\n");
}

void graph_destroy(struct graph *G)
{
	struct dl_list_n *scan;
	struct node *x;

	scan = G->V;
	while (!dlln_isempty(scan)) {
		x = dlln_get(scan);

		node_destroy(x);
		
		scan = dlln_next(scan);
	}

	dlln_destroy(G->V);
	dlln_destroy(G->xx);

	if (G->rcode != NULL)
		interval_destroy(G->rcode);

	free(G);
}

/* Implementazione funzioni ausiliarie */
static struct dl_list_b *children(struct node *v)
{
	struct dl_list_b *l = dllb_create();

	for (uint64_t i = bn_bits(v->code) - 1; i < UINT64_MAX; i--) {
		if (bn_get(v->code, i))
			l = dllb_add(l, bn_fromuint64(i));
	}

	return l;
}

static struct dl_list_n *join(struct dl_list_n *V, struct node *x,
	struct dl_list_b *l)
{
	/* l lista dei codici dei figli di x (decrescenti) */
	struct dl_list_n *scanV;
	struct dl_list_b *scanl;
	struct node *y;

	if (dllb_isempty(l))
		return V;

	scanV = dlln_last(V);
	y = dlln_get(scanV);
	scanl = dllb_last(l);
	while (!dllb_isempty(scanl)) {
		if (bn_cmp(node_get(y), dllb_get(scanl)) == 0) {
			/* il vertice esiste già */
			node_addarc(x,y);
			scanl = dllb_prev(scanl);
		} else if (bn_cmp(node_get(y), dllb_get(scanl)) >= 0) {
			/* il vertice non esiste */
			V = dlln_insert(V, node_create(dllb_get(scanl)), scanV);
			node_addarc(x,dlln_get(dlln_next(scanV)));
			scanl = dllb_prev(scanl);
		} else {
			/* non è figlio di x */
			scanV = dlln_prev(scanV);
			y = dlln_get(scanV);
		}
	}

	return V;
}

static void calcrack(struct node *x, long long de, long long dn,
	long long da)
{
	if (x->rcode != NULL)
		return;

	struct interval *res;
	struct interval *i, *t;
	struct dl_list_n *l;
	struct node *y;

	res = interval_zero();

	if (dlln_isempty(x->adj)) {
		x->rcode = res;
		return;
	}

	l = x->adj;
	while (!dlln_isempty(l)) {
		y = dlln_get(l);

		//calcrack(y,de,dn,da);
		i = interval_rec_exp_2(y->rcode, de, dn, da);

		t = res;
		res = interval_add(res, i);
		interval_destroy(t);

		interval_destroy(i);
		l = dlln_next(l);
	}

	x->rcode = res;
}
