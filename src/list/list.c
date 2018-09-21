#include <stdlib.h>

#include "../memgraph/memnode/memnode.h"
#include "../bignat/bignat.h"
#include "list.h"

struct dl_list_n *dlln_create()
{
	return NULL; /* NULL rappresenta la lista vuota */
}

struct dl_list_n *dlln_add(struct dl_list_n *list, struct memnode *x)
{
	struct dl_list_n *newtail = malloc(sizeof(struct dl_list_n));

	newtail->node = x;
	newtail->next = NULL;

	if (list != NULL) { /* newtail va in coda a list */
		newtail->prev = list->last;
		list->last->next = newtail;
		list->last = newtail;
		return list;
	} else { /* newtail e' l'unico nodo */
		newtail->prev = NULL;
		newtail->last = newtail;
		return newtail;
	}
}

struct dl_list_n *dlln_insert(struct dl_list_n *list, struct memnode *x,
	struct dl_list_n *el)
{

	struct dl_list_n *new;
	if (dlln_istail(el))
		return dlln_add(list,x);

	new = malloc(sizeof(struct dl_list_n));
	new->node = x;
	new->prev = el;
	new->next = el->next;

	el->next = new;

	if (new->next != NULL)
		new->next->prev = new;

	return list;
}
struct dl_list_n *dlln_remove(struct dl_list_n *list, struct dl_list_n *el)
{
	/* el e list devono essere != NULL */
	/* el deve essere un elemento di list */
	if (el->prev == NULL && el->next == NULL) { /* el e' l'unico elemento */
		free(el);
		return NULL;
	} else if (el->prev == NULL) { /* el e' in testa (list==el) */
		list = el->next;
		list->prev = NULL;
		list->last = el->last;

		free(el);
		return list;
	} else if (el->next == NULL) { /* el e' in coda */
		list->last = el->prev;
		el->prev->next = NULL;
		
		free(el);
		return list;
	} else {
		el->prev->next = el->next;
		el->next->prev = el->prev;

		free(el);
		return list;
	}
}

struct memnode *dlln_get(struct dl_list_n *el)
{
	return el->node;
}

int dlln_isempty(struct dl_list_n *list)
{
	return (list == NULL);
}

int dlln_ishead(struct dl_list_n *el)
{
	if (el->prev == NULL)
		return 1;
	else
		return 0;
}

int dlln_istail(struct dl_list_n *el)
{
	if (el->next == NULL)
		return 1;
	else
		return 0;
}

struct dl_list_n *dlln_next(struct dl_list_n *el)
{
	return el->next;
}

struct dl_list_n *dlln_prev(struct dl_list_n *el)
{
	return el->prev;
}

struct dl_list_n *dlln_last(struct dl_list_n *list)
{
	if (list == NULL)
		return NULL;
	else
		return list->last;
}

void dlln_destroy(struct dl_list_n *list)
{
	struct dl_list_n *temp;
	while (list != NULL){
		temp = list->next;
		free(list);
		list = temp;
	}
}

struct dl_list_b *dllb_create()
{
	return NULL; /* NULL rappresenta la lista vuota */
}

struct dl_list_b *dllb_add(struct dl_list_b *list, struct bignat *u)
{
	struct dl_list_b *newtail = malloc(sizeof(struct dl_list_b));

	newtail->u = u;
	newtail->next = NULL;

	if (list != NULL) { /* newtail va in coda a list */
		newtail->prev = list->last;
		list->last->next = newtail;
		list->last = newtail;
		return list;
	} else { /* newtail e' l'unico nodo */
		newtail->prev = NULL;
		newtail->last = newtail;
		return newtail;
	}
}

struct dl_list_b *dllb_insert(struct dl_list_b *list, struct bignat *u,
	struct dl_list_b *el)
{

	struct dl_list_b *new;
	if (dllb_istail(el))
		return dllb_add(list,u);

	new = malloc(sizeof(struct dl_list_b));
	new->u = u;
	new->prev = el;
	new->next = el->next;

	el->next = new;

	if (new->next != NULL)
		new->next->prev = new;

	return new;
}

struct dl_list_b *dllb_remove(struct dl_list_b *list, struct dl_list_b *el)
{
	/* el e list devono essere != NULL */
	/* el deve essere un elemento di list */
	if (el->prev == NULL && el->next == NULL) { /* el e' l'unico elemento */
		free(el);
		return NULL;
	} else if (el->prev == NULL) { /* el e' in testa (list==el) */
		list = el->next;
		list->prev = NULL;
		list->last = el->last;

		free(el);
		return list;
	} else if (el->next == NULL) { /* el e' in coda */
		list->last = el->prev;
		el->prev->next = NULL;
		
		free(el);
		return list;
	} else {
		el->prev->next = el->next;
		el->next->prev = el->prev;

		free(el);
		return list;
	}
}

void dllb_set(struct dl_list_b *el, struct bignat *u)
{
	el->u = u;
}

struct bignat *dllb_get(struct dl_list_b *el)
{
	return el->u;
}

int dllb_isempty(struct dl_list_b *list)
{
	return (list == NULL);
}

int dllb_ishead(struct dl_list_b *el)
{
	if (el->prev == NULL)
		return 1;
	else
		return 0;
}

int dllb_istail(struct dl_list_b *el)
{
	if (el->next == NULL)
		return 1;
	else
		return 0;
}

struct dl_list_b *dllb_next(struct dl_list_b *el)
{
	return el->next;
}

struct dl_list_b *dllb_prev(struct dl_list_b *el)
{
	return el->prev;
}

struct dl_list_b *dllb_last(struct dl_list_b *list)
{
	if (list == NULL)
		return NULL;
	else
		return list->last;
}

void dllb_destroy(struct dl_list_b *list)
{
	struct dl_list_b *temp;
	while (list != NULL){
		temp = list->next;
		free(list);
		list = temp;
	}
}
