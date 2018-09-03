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

#define DE -2 /* precisione cambio di base da 2 ad e */
#define DN 15 /* grado polinomio di MacLaurin */
#define DA -2 /* precisione risultati parziali del polinomio */

int main()
{
	struct dl_list_b *list;
	struct graph *G;

	// test accuratezza
	/*for (uint64_t i = 0; i < 100000; i++) {
		list = dllb_create();
		list = dllb_add(list, bn_fromuint64(i));

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
