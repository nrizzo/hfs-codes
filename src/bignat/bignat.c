/*
 * github.com/nrizzo
 *
 * Implementazione della libreria per creazione e operazioni aritmetiche di
 * numeri naturali di grandezza arbitraria.
 */

#include <stdlib.h> /* malloc e free */
#include <inttypes.h> /* uint_32 */
#include <stdio.h> /* printf */

#include "bignat.h"

/* Dichiarazioni funzioni ausiliarie */

/*
 * max e min confrontano due numeri di macchina, interi nonnegativi di (al
 * massimo) 32 bit, restituendone rispettivamente il maggiore e il minore.
 */
uint32_t max(uint32_t a, uint32_t b);
uint32_t min(uint32_t a, uint32_t b);

/*
 * digit, dato il puntatore ad un bignat u, restituisce l'(i+1)-esima cifra meno
 * significativa (in base 2^32), anche quando l'indice i supera u->n - 1.
 */
static uint32_t digit(struct bignat *u, uint32_t i);

/*
 * normalize, dato il puntatore ad un bignat, ne corregge la rappresentazione
 * se errata, rimuovendo le cifre nulle più significative, se di indice
 * superiore a 0.
 */
static void normalize(struct bignat *u);


struct bignat *bn_zero()
{
	struct bignat *u;

	u = malloc(sizeof(struct bignat));
	u->n = 1;
	u->u = malloc(sizeof(uint32_t)*u->n);
	u->u[0] = 0;

	return u;
}

struct bignat *bn_fromuint32(uint32_t i)
{
	struct bignat *u;

	u = malloc(sizeof(struct bignat));
	u->n = 1;
	u->u = malloc(sizeof(uint32_t)*u->n);
	u->u[0] = i;

	return u;
}

struct bignat *bn_fromuint64(uint64_t i)
{
	struct bignat *u;

	u = malloc(sizeof(struct bignat));
	u->n = 2;
	u->u = malloc(sizeof(uint32_t)*u->n);
	u->u[0] = (uint32_t) i;
	u->u[1] = (uint32_t) (i>>32);

	normalize(u);
	return u;
}

struct bignat *bn_fromstring32(char *s)
{
	struct bignat *u = bn_zero();
	struct bignat *two = bn_fromuint32(2);
	struct bignat *one = bn_fromuint32(1);
	struct bignat *t;
	int bits;

	bits = 0;
	for (int i = 0; s[i] != '\0'; i++) {
		t = bn_mul(u,two); bn_destroy(u); u = t;

		if (s[i] != '0') {
			t = bn_add(u,one); bn_destroy(u); u = t;
		}
		bits++;
	}

	while (bits % 32 != 0) {
		t = bn_mul(u,two); bn_destroy(u); u = t;
		bits++;
	}

	bn_destroy(two);
	bn_destroy(one);
	return u;
}

struct bignat *bn_copy(struct bignat *u)
{
	struct bignat *c;

	c = malloc(sizeof(struct bignat));
	c->n = u->n;
	c->u = malloc(sizeof(uint32_t)*c->n);

	for (uint32_t i = 0; i < c->n; i++)
		c->u[i] = u->u[i];

	return c;
}

uint64_t bn_bits(struct bignat *u)
{
	uint64_t res;
	char i; /* indice primo bit != 0 (dal più significativo) */

	if (u->n == 1 && u->u[0] == 0) /* controllo zero */
		return 1;

	res = (u->n - 1) * 32; /* 32 per ogni uint32_t tranne l'ultimo */
	for (i = 0; i < 32 && !(u->u[u->n-1] >> (31-i)&1); i++) {}
	res += 32 - i; /* bit ultimo uint32_t */

	return res;
}

char bn_get(struct bignat *u, uint64_t i)
{
	/* gli uint32_t dell'array sono salvati dal meno significativo al più
	significativo, i bit di ogni uint32_t sono salvati dal più significativo
	al meno */
	return (char) (u->u[i / 32]<<((31 - i) % 32)>>31)&1;
}

int bn_cmp(struct bignat *u, struct bignat *v)
{
	if (u->n > v->n)
		return +1;
	if (u->n < v->n)
		return -1;

	/* u->n == v->n */
	for (uint32_t i = u->n - 1; i < UINT32_MAX; i--) {
		if (u->u[i] > v->u[i])
			return +1;
		if (u->u[i] < v->u[i])
			return -1;
	}

	return 0;
}

char bn_iszero(struct bignat *u)
{
	return (u->n == 1 && u->u[0] == 0);
}

struct bignat *bn_succ(struct bignat *u)
{
	struct bignat *s; /* risultato */
	char k; /* carry (0 o 1) */

	s = bn_copy(u);

	k = 1;
	for (uint32_t i = 0; (i < s->n) && (k == 1); i++) {
		s->u[i] = s->u[i] + 1;
		k = (s->u[i] == 0); /* overflow */
	}

	if (k == 1) {
		s->n = s->n + 1;
		s->u = realloc(s->u, sizeof(uint32_t)*s->n);
		s->u[s->n - 1] =  1;
	}

	return s;
}

struct bignat *bn_pred(struct bignat *u)
{
	struct bignat *p; /* risultato */
	signed char k; /* borrow (0 o -1) */

	if (u->n == 1 && u->u[0] == 0)
		return bn_zero();

	p = bn_copy(u);

	k = -1;
	for (uint32_t i = 0; (i < p->n) && (k == -1); i++) {
		p->u[i] = p->u[i] - 1;
		k = -(p->u[i] == UINT32_MAX); /* overflow */
	}

	if (k == -1) {
		p->n = p->n + 1;
		p->u = realloc(p->u, sizeof(uint32_t)*p->n);
		p->u[p->n - 1] =  1;
	}

	return p;
}

struct bignat *bn_add(struct bignat *u, struct bignat *v)
{
	char k; /* carry (0 o 1) */
	struct bignat *w; /* risultato */

	w = malloc(sizeof(struct bignat));
	w->n = max(u->n,v->n) + 1;
	w->u = malloc(sizeof(uint32_t)*w->n);

	k = 0;
	for (uint32_t j = 0; j < w->n - 1; j++) {
		w->u[j] = digit(u,j) + digit(v,j) + k;
		k = (w->u[j] < digit(u,j));
	}
	w->u[w->n - 1] = k;

	normalize(w);
	return w;
}

struct bignat *bn_sub(struct bignat *u, struct bignat *v)
{
	/* u >= v */
	signed char k; /* borrow (0 o -1) */
	struct bignat *w; /* risultato */
	
	w = malloc(sizeof(struct bignat));
	w->n = max(u->n,v->n);
	w->u = malloc(sizeof(uint32_t)*w->n);

	k = 0;
	for (uint32_t j = 0; j < u->n; j++) {
		w->u[j] = digit(u,j) - digit(v,j) + k;
		k = -(w->u[j] > digit(u,j));
	}
	/* k == 0 (perché u >= v) */

	normalize(w);
	return w;
}

struct bignat *bn_mul(struct bignat *u, struct bignat *v)
{
	struct bignat *w; /* risultato */
	uint32_t j, i; /* indici */
	uint64_t t; /* risultato moltiplicazione di due cifre */
	uint32_t k; /* cifra più significativa di t */

	w = malloc(sizeof(struct bignat));
	w->n = u->n + v->n;
	w->u = malloc(sizeof(uint32_t)*w->n);

	for (uint32_t i = 0; i < w->n; i++)
		w->u[i] = 0;

	for (uint32_t j = 0; j < v->n; j++) {
		// TODO: zero multiplier (M2)
		k = 0;
		for (uint32_t i = 0; i < u->n; i++) {
			t = (uint64_t) u->u[i] * (uint64_t) v->u[j] +
				(uint64_t) w->u[i+j] + (uint64_t) k;
			w->u[i+j] = (uint32_t) t;
			k = (uint32_t) (t>>32);
		}

		w->u[j + u->n] = k;
	}

	normalize(w);
	return w;
}

void bn_print(struct bignat *u)
{
	for (uint64_t i = bn_bits(u) - 1; i > 0; i--)
		putchar('0' + bn_get(u,i));

	putchar('0' + bn_get(u,0));
}

void bn_print_hex(struct bignat *u)
{
	for (uint32_t i = u->n-1; i > 0; i--)
		printf("%.8" PRIX32 " ", u->u[i]);

	printf("%.8" PRIX32, u->u[0]);
}

struct bignat *bn_scan()
{
	struct bignat *u = bn_zero();
	struct bignat *two = bn_fromuint32(2);
	struct bignat *one = bn_fromuint32(1);
	struct bignat *t;
	int c; /* carattere letto */

	c = getchar();
	while (c != EOF && c != '\n') {
		t = bn_mul(u,two); bn_destroy(u); u = t;

		if (c != '0') {
			t = bn_add(u,one); bn_destroy(u); u = t;
		}

		c = getchar();
	}

	bn_destroy(two);
	bn_destroy(one);
	return u;
}

struct bignat *bn_div_uint32(struct bignat *u, uint32_t a)
{
	struct bignat *v; /* risultato */
	uint64_t t; /* dividendo parziale */
	uint32_t r; /* resto parziale */

	v = malloc(sizeof(struct bignat));
	v->n = u->n;
	v->u = malloc(sizeof(uint32_t)*v->n);

	r = 0;
	for (uint32_t i = u->n - 1; i < UINT32_MAX; i--) {
		t = ((uint64_t)r<<32) + u->u[i];
		v->u[i] = (uint32_t) (t / a);
		r = (uint32_t) (t % a);
	}

	normalize(v);
	return v;
}

uint32_t bn_rem_uint32(struct bignat *u, uint32_t n)
{
	struct bignat *qq;
	int j;
	uint32_t q,r;
	uint64_t d;

	qq = malloc(sizeof(struct bignat));
	qq->n = u->n;
	qq->u = malloc(sizeof(uint32_t)*qq->n);

	j = qq->n - 1;
	d = 0;
	do {
		d = d<<32;
		d = d + u->u[j];

		qq->u[j] = d / n;
		d = d % n;

		j = j - 1;
	} while (j >= 0);

	normalize(qq);
	bn_destroy(qq);

	return (uint32_t) d;
}

char bn_remcheck_uint32(struct bignat *u, uint32_t n)
{
	struct bignat *qq;
	int j;
	uint32_t q,r;
	uint64_t d;

	qq = malloc(sizeof(struct bignat));
	qq->n = u->n;
	qq->u = malloc(sizeof(uint32_t)*qq->n);

	j = qq->n - 1;
	d = 0;
	do {
		d = d<<32;
		d = d + u->u[j];

		qq->u[j] = d / n;
		d = d % n;

		j = j - 1;
	} while (j >= 0);

	normalize(qq);
	bn_destroy(qq);

	return (d != 0);
}

void bn_destroy(struct bignat *u)
{
	free(u->u);
	free(u);
}

/* Implementazione funzioni ausiliarie */
uint32_t max(uint32_t a, uint32_t b)
{
	if (a > b)
		return a;
	else
		return b;
}

uint32_t min(uint32_t a, uint32_t b)
{
	if (a < b)
		return a;
	else
		return b;
}

static uint32_t digit(struct bignat *u, uint32_t i)
{
	if (i <= u->n - 1)
		return u->u[i];
	else
		return 0;
}

static void normalize(struct bignat *u)
{
	uint32_t j; /* cifra più significativa non nulla */

	for (j = u->n - 1; u->u[j] == 0 && j > 0; j--) {}
	u->n = j + 1;
	u->u = realloc(u->u, sizeof(uint32_t)*(u->n));
}
