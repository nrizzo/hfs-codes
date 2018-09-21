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
#include "memnode.h"

struct memnode *memnode_create(struct bignat *u)
{
	struct memnode *x;

	x = malloc(sizeof(struct memnode));
	x->code = u;
	x->indegree = 0;
	x->outdegree = 0;
	x->adj = dlln_create();

	x->rcode = NULL;
	x->rsingleton = NULL;

	return x;
}


void memnode_addarc(struct memnode *x, struct memnode *y)
{
	x->outdegree += 1;
	y->indegree += 1;
	x->adj = dlln_add(x->adj, y);
}

int memnode_indegree(struct memnode *x)
{
	return x->indegree;
}

int memnode_outdegree(struct memnode *x)
{
	return x->outdegree;
}

struct dl_list_n *memnode_adj(struct memnode *x)
{
	return x->adj;
}

struct bignat *memnode_get(struct memnode *x)
{
	return x->code;
}

void memnode_destroy(struct memnode *x)
{
	bn_destroy(x->code);
	if (x->rcode != NULL)
		interval_destroy(x->rcode);
	if (x->rsingleton != NULL)
		interval_destroy(x->rsingleton);
	dlln_destroy(x->adj);
	free(x);
}
