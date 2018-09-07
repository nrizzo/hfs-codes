/*
 * github.com/nrizzo
 *
 * Codice dell'algoritmo risolutivo.
 */

#include <stdio.h>

#include "bignat/bignat.h"
#include "list/list.h"
#include "graph/graph.h"
#include "interval/interval.h"

#define DE -3 /* precisione cambio di base da 2 ad e */
#define DN 15 /* grado polinomio di MacLaurin */
#define DA -2 /* precisione risultati parziali del polinomio */

int main()
{
	struct dl_list_b *list;
	struct graph *G;
	struct bignat *u, *v, *tbn;

	// test accuratezza
	/*u = bn_fromuint32(1);
	v = bn_fromuint32(55);
	for (uint64_t i = 0; i < 10000; i++) {
		tbn = bn_copy(u);
		u = bn_mul(u,v);

		list = dllb_create();
		list = dllb_add(list, tbn);

		G = graph_create(list);
		graph_build(G);
		graph_calcrack(G, DE, DN, DA);
		printf("%"PRIu64"\t%lld\n",i,interval_accuracy(G->xx->node->rcode));

		dllb_destroy(list);
		graph_destroy(G);
	}*/

	/* grafo di appartenenza di {h_n} */
	list = dllb_create();
	list = dllb_add(list, bn_scan());
	G = graph_create(list);
	graph_build(G);
	graph_calcrack(G, DE, DN, DA);
	graph_printDOT(G);
	dllb_destroy(list);
	graph_destroy(G);
}
