/*
 * github.com/nrizzo
 *
 * Codice dell'algoritmo risolutivo.
 */

#include <stdio.h> /* printf, getchar */
#include <ctype.h> /* isspace */

#include "bignat/bignat.h"
#include "list/list.h"
#include "memgraph/memgraph.h"
#include "interval/interval.h"

#define DE -3 /* precisione cambio di base da 2 ad e */
#define DN 15 /* grado polinomio di MacLaurin */
#define DA -2 /* precisione risultati parziali del polinomio */

/* Funzioni ausiliarie */
/* dllb_scanchildren legge dallo standard output i caratteri di un intero non
 * negativo in notazione binaria, dal bit piu' significativo a quello meno
 * significativo, creando la lista dei codici dei figli dell'insieme
 * ereditariamente finito corrispondente; lo spazio occupato dalla lista va
 * liberato da dllb_destroy, mentre quello occupato dai suoi elementi viene
 * trattato dalle procedure del grafo. */
struct dl_list_b *dllb_scanchildren();

int main()
{
	struct dl_list_b *list; /* lista dei figli di n */
	struct memgraph *G; /* grafo di appartenenza di n */

	list = dllb_scanchildren();

	G = memgraph_create(list);
	memgraph_build(G);
	memgraph_calcrack(G, DE, DN, DA);

	memgraph_printDOT(G);
	interval_print(G->rcode); printf("\n");

	dllb_destroy(list);
	memgraph_destroy(G);
}

/* Implementazione funzioni ausiliarie */
struct dl_list_b *dllb_scanchildren()
{
	struct dl_list_b *l; /* risultato */
	struct dl_list_b *scan;
	struct bignat *i; /* contatore */
	struct bignat *tbn;
	int c; /* carattere corrente */

	l = dllb_create();

	i = bn_zero();
	c = getchar();
	while (!isspace(c) && c != EOF) {
		if (c != '0')
			l = dllb_add(l, bn_copy(i));

		tbn = i;
		i = bn_succ(i);
		bn_destroy(tbn);

		c = getchar();
	}

	tbn = i;
	i = bn_pred(i);
	bn_destroy(tbn);

	scan = l;
	while (!dllb_isempty(scan)) {
		tbn = dllb_get(scan);
		dllb_set(scan, bn_sub(i, dllb_get(scan)));
		bn_destroy(tbn);

		scan = dllb_next(scan);
	}

	return l;
}
