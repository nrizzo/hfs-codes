#include <stdio.h>

#include "bignat/bignat.h"
#include "list/list.h"
#include "graph/graph.h"
#include "interval/interval.h"

#define DE -2
#define DN 15
#define DA -2

int main()
{
	struct dl_list_b *list = dllb_create();
	list = dllb_add(list, bn_scan());

	struct graph *G = graph_create(list);
	graph_build(G);
	graph_calcrack(G, DE, DN, DA);
	graph_printDOT(G);
}
