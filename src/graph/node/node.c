/*
 * github.com/nrizzo
 * 
 * Implementazione del codice che tratta i nodi di un grafo di appartenenza di
 * un insieme ereditariamente finito.
 */

#include <stdlib.h> /* malloc e free */

#include "../../bignat/bignat.h"
#include "../../list/list.h"
#include "../../interval/interval.h"
#include "node.h"

struct node *node_create(struct bignat *u)
{
	struct node *x;

	x = malloc(sizeof(struct node));
	x->code = u;
	x->indegree = 0;
	x->outdegree = 0;
	x->adj = dlln_create();
	x->rcode = NULL;

	return x;
}


void node_addarc(struct node *x, struct node *y)
{
	x->outdegree += 1;
	y->indegree += 1;
	x->adj = dlln_add(x->adj, y);
}

int node_indegree(struct node *x)
{
	return x->indegree;
}

int node_outdegree(struct node *x)
{
	return x->outdegree;
}

struct dl_list_n *node_adj(struct node *x)
{
	return x->adj;
}

struct bignat *node_get(struct node *x)
{
	return x->code;
}

void node_destroy(struct node *x)
{
	bn_destroy(x->code);
	if (x->rcode != NULL)
		interval_destroy(x->rcode);
	dlln_destroy(x->adj);
	free(x);
}
