#include "../bignat/bignat.h"
#include "../graph/node/node.h"

#ifndef LISTGUARD /* guardia */
#define LISTGUARD

/* 
 * dl_list_n sta per doubly linked list of nodes. Struct per rappresentare una
 * lista doppiamente concatenata di nodi di G_T (o G_T^*).
 */
struct dl_list_n{
	struct node *node;

	struct dl_list_n *next;
	struct dl_list_n *prev;
	struct dl_list_n *last;
};

struct dl_list_n *dlln_create();
struct dl_list_n *dlln_add(struct dl_list_n *list, struct node *x);
struct dl_list_n *dlln_insert(struct dl_list_n *list, struct node *x,
	struct dl_list_n *el);
struct dl_list_n *dlln_remove(struct dl_list_n *list, struct dl_list_n *el);
struct node *dlln_get(struct dl_list_n *el);
int dlln_isempty(struct dl_list_n *list);
int dlln_ishead(struct dl_list_n *list);
int dlln_istail(struct dl_list_n *list);
struct dl_list_n *dlln_next(struct dl_list_n *list);
struct dl_list_n *dlln_prev(struct dl_list_n *list);
struct dl_list_n *dlln_last(struct dl_list_n *list);
void dlln_destroy(struct dl_list_n *list);



struct dl_list_b{
	struct bignat *u;

	struct dl_list_b *next;
	struct dl_list_b *prev;
	struct dl_list_b *last;
};

struct dl_list_b *dllb_create();
struct dl_list_b *dllb_add(struct dl_list_b *list, struct bignat *x);
struct dl_list_b *dllb_insert(struct dl_list_b *x, struct bignat *y,
	struct dl_list_b *el);
struct dl_list_b *dllb_remove(struct dl_list_b *list, struct dl_list_b *el);
struct bignat *dllb_get(struct dl_list_b *el);
int dllb_isempty(struct dl_list_b *list);
int dllb_ishead(struct dl_list_b *list);
int dllb_istail(struct dl_list_b *list);
struct dl_list_b *dllb_next(struct dl_list_b *list);
struct dl_list_b *dllb_prev(struct dl_list_b *list);
struct dl_list_b *dllb_last(struct dl_list_b *list);
void dllb_destroy(struct dl_list_b *list);
#endif
