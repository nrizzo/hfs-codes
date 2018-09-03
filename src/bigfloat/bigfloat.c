/*
 * github.com/nrizzo
 *
 * Implementazione della libreria per creazione, operazioni aritmetiche e
 * operazioni di approssimazione di numeri razionali di precisione arbitraria.
 */

#include <inttypes.h> /* uint32_t, uint64_t */
#include <stdlib.h> /* malloc e free, llabs */
#include <stdio.h> /* printf */

#include "../bignat/bignat.h"
#include "bigfloat.h"

/* Dichiarazioni funzioni ausiliarie */
/*
 * digit, dato il puntatore ad un bignat u, restituisce l'(i+1)-esima cifra meno
 * significativa (in base 2^32) di u, anche quando l'indice i supera u->n - 1.
 */
static uint32_t digit(struct bignat *u, uint32_t i);

/*
 * bn_lshift e bn_rshift, dati il puntatore ad un bignat u e il numero naturale
 * n, creano e restituiscono il risultato di rispettivamente la moltiplicazione
 * e la divisione di u per (2^32)^n modulo (2^32); va accoppiata a bn_destroy
 * per liberare lo spazio occupato.
 */
static struct bignat *bn_lshift(struct bignat *u, uint32_t n);
static struct bignat *bn_rshift(struct bignat *u, uint32_t n);

/*
 * normalize, dato il puntatore ad un bigfloat f, ne corregge la
 * rappresentazione se errata, eliminando le cifre nulle meno significative
 * della mantissa e correggendo appropriatamente l'esponente.
 */
static void normalize(struct bigfloat *f);


struct bigfloat *bf_zero()
{
	struct bigfloat *f;

	f = malloc(sizeof(struct bigfloat));
	f->sign = 1;
	f->m = bn_zero();
	f->e = 0;

	return f;
}

struct bigfloat *bf_fromuint32(uint32_t i)
{
	struct bigfloat *f;

	f = malloc(sizeof(struct bigfloat));
	f->sign = 1;
	f->m = bn_fromuint32(i);
	f->e = 0;

	return f;
}

struct bigfloat *bf_copy(struct bigfloat *f)
{
	struct bigfloat *c;

	c = malloc(sizeof(struct bigfloat));
	c->sign = f->sign;
	c->m = bn_copy(f->m);
	c->e = f->e;

	return c;
}

signed char bf_cmp(struct bigfloat *f, struct bigfloat *g)
{
	if (f->sign == +1 && g->sign == -1)
		return 1;

	if (f->sign == -1 && g->sign == +1)
		return -1;

	/* f->sign == g->sign */
	if (f->e + f->m->n > g->e + g->m->n)
		return f->sign;

	if (f->e + f->m->n < g->e + g->m->n)
		return -f->sign;

	/* la cifra più significativa di entrambi è a f->e + f->m->n */
	int64_t i = f->e + f->m->n;
	while (1) {
		if (digit(f->m, i - f->e) > digit(g->m, i - g->e))
			return f->sign;
		if (digit(f->m, i - f->e) < digit(g->m, i - g->e))
			return -f->sign;

		/* le cifre sono uguali */
		i = i - 1;
		if (i < f->e && i < g->e)
			return 0;
		if (i < f->e)
			return -f->sign;
		if (i < g->e)
			return +f->sign;
	}
}

struct bigfloat *bf_add(struct bigfloat *f, struct bigfloat *g)
{
	struct bigfloat *h; /* risultato */
	struct bignat *u, *v; /* addendi parziali */

	h = malloc(sizeof(struct bigfloat));

	if (f->e < g->e) {
		u = bn_copy(f->m);
		v = bn_lshift(g->m, g->e - f->e);
		h->e = f->e;
	} else if (f->e > g->e) {
		u = bn_lshift(f->m, f->e - g->e);
		v = bn_copy(g->m);
		h->e = g->e;
	} else {
		u = bn_copy(f->m);
		v = bn_copy(g->m);
		h->e = f->e;
	}

	if (f->sign == 1 && g->sign == 1) {
		h->m = bn_add(u,v);
		h->sign = 1;
	} else if (f->sign == -1 && g->sign == -1) {
		h->m = bn_add(u,v);
		h->sign = -1;
	} else if (f->sign == 1) { /* g->sign == -1 */
		if (bn_cmp(u,v) >= 0) {
			h->m = bn_sub(u,v);
			h->sign = +1;
		} else {
			h->m = bn_sub(v,u);
			h->sign = -1;
		}
	} else { /* f->sign == -1 && g->sign == 1 */
		if (bn_cmp(u,v) < 0) {
			h->m = bn_sub(v,u);
			h->sign = +1;
		} else {
			h->m = bn_sub(u,v);
			h->sign = -1;
		}
	}

	normalize(h);
	bn_destroy(u);
	bn_destroy(v);
	return h;
}

struct bigfloat *bf_sub(struct bigfloat *f, struct bigfloat *g)
{
	struct bigfloat *mg;
	struct bigfloat *res;

	if (bn_iszero(g->m))
		return bf_copy(f);

	mg = bf_copy(g);
	mg->sign *= -1; /* cambio segno */

	res = bf_add(f, mg);

	bf_destroy(mg);
	return res;
}

struct bigfloat *bf_mul(struct bigfloat *f, struct bigfloat *g)
{
	struct bigfloat *h; /* risultato */

	if (bn_iszero(f->m) || bn_iszero(g->m))
		return bf_zero();

	h = malloc(sizeof(struct bigfloat));
	h->sign = f->sign * g->sign;
	h->m = bn_mul(f->m,g->m);
	h->e = f->e + g->e;

	normalize(h);
	return h;
}

struct bigfloat *bf_trunc(struct bigfloat *f, long long int d)
{
	struct bignat *tbn;
	struct bigfloat *res, *tbf;

	res = bf_copy(f);

	if (d <= f->e)
		return res;

	if (res->sign == -1) { /* per difetto */
		res->sign = 1;

		tbf = res;
		res = bf_round(res, d);
		bf_destroy(tbf);

		res->sign = -1;

		return res;
	}

	/* res->sign == 1 */
	tbn = res->m;
	res->m = bn_rshift(res->m, llabs(d - res->e));
	bn_destroy(tbn);

	res->e = d;

	normalize(res);
	return res;
}

struct bigfloat *bf_round(struct bigfloat *f, long long int d)
{
	struct bignat *tbn;
	struct bigfloat *res, *tbf;

	res = bf_copy(f);

	if (f->e >= d)
		return res;

	if (res->sign == -1) { /* per eccesso */
		res->sign = 1;

		tbf = res;
		res = bf_trunc(res, d);
		bf_destroy(tbf);

		res->sign = -1;

		return res;
	}

	tbn = res->m;
	res->m = bn_rshift(res->m, llabs(res->e - d));
	bn_destroy(tbn);

	res->e = d;

	tbn = res->m;
	res->m = bn_succ(res->m);
	bn_destroy(tbn);

	normalize(res);
	return res;
}

struct bigfloat *bf_div_uint32_trunc(struct bigfloat *f, uint32_t i,
	long long int d)
{
	struct bigfloat *res;
	struct bigfloat *tbf;
	struct bignat *tbn;

	res = bf_copy(f);

	if (d < res->e) {
		tbn = res->m;
		res->m = bn_lshift(res->m, (uint32_t)llabs(res->e - d));
		bn_destroy(tbn);

		res->e = d;

		tbn = res->m;
		res->m = bn_div_uint32(res->m, i);
		bn_destroy(tbn);

		normalize(res);
		return res;
	} else if (d > res->e) {
		tbn = res->m;
		res->m = bn_div_uint32(res->m, i);
		bn_destroy(tbn);

		normalize(res);

		tbf = res;
		res = bf_trunc(res, d);
		bf_destroy(tbf);

		return res;
	} else { /* d == res->e */
		tbn = res->m;
		res->m = bn_div_uint32(res->m, i);
		bn_destroy(tbn);

		normalize(res);
		return res;
	}
}

struct bigfloat *bf_div_uint32_round(struct bigfloat *f, uint32_t i, long long int d)
{
	struct bigfloat *res = bf_copy(f);
	struct bigfloat *tbf;
	struct bignat *tbn;

	if (d - 1 < res->e) {
		tbn = res->m;
		res->m = bn_lshift(res->m, (uint32_t)llabs(res->e - (d-1)));
		bn_destroy(tbn);

		res->e = d - 1;

		tbn = res->m;
		res->m = bn_div_uint32(res->m, i);
		bn_destroy(tbn);

		normalize(res);

		tbf = res;
		res = bf_round(res, d);
		bf_destroy(tbf);

		return res;
	} else if (d - 1 > res->e) {
		tbn = res->m;
		res->m = bn_div_uint32(res->m, i);
		bn_destroy(tbn);

		normalize(res);
		tbf = res;
		res = bf_round(res, d);
		bf_destroy(tbf);

		return res;
	} else {
		tbn = res->m;
		res->m = bn_div_uint32(res->m, i);
		bn_destroy(tbn);

		normalize(res);
		tbf = res;
		res = bf_round(res, d);
		bf_destroy(tbf);

		return res;
	}
}

struct bigfloat *bf_rshift(struct bigfloat *f, long long int d)
{
	struct bigfloat *g = bf_copy(f);
	f->e -= d;
	return f;
}

struct bigfloat *bf_lshift(struct bigfloat *f, long long int d)
{
	struct bigfloat *g = bf_copy(f);
	f->e += d;
	return f;
}

struct bignat *bf_tobignat(struct bigfloat *f)
{
	struct bigfloat *tbf;
	struct bignat *res;

	if (f->e + f->m->n <= 0)
		return bn_zero();

	tbf = bf_trunc(f, 0);
	res = bn_copy(tbf->m);
	bf_destroy(tbf);

	return res;
}

struct bigfloat *bf_frac(struct bigfloat *f)
{
	struct bigfloat *res;
	struct bigfloat *tbf;

	if (bn_iszero(f->m))
		return bf_zero();

	tbf = bf_trunc(f, 0);
	res = bf_sub(f, tbf);
	bf_destroy(tbf);

	return res;
}

struct bigfloat *bf_div_pow2_bn(struct bigfloat *f, struct bignat *n)
{
	struct bigfloat *res;
	struct bigfloat *tbf;

	struct bignat *r;
	struct bignat *tbn;
	struct bignat *zero = bn_zero();

	res = bf_copy(f);
	r = bn_div_uint32(n, 32);

	while (!bn_iszero(r)) {
		tbf = res;
		res = bf_rshift(res, 1);
		bf_destroy(tbf);

		tbn = r;
		r = bn_pred(r);
		bn_destroy(tbn);
	}

	tbf = res;
	res = bf_div_uint32_trunc(res, 1<<bn_rem_uint32(n, 32), res->e - 1);
	bf_destroy(tbf);

	return res;
}

struct bigfloat *bf_scan()
{
	struct bigfloat *f;

	f = malloc(sizeof(struct bigfloat));
	f->sign = 1;
	f->m = bn_scan();
	f->e = 0;
	f->e -= bn_bits(f->m) / 32;
	f->e -= ((bn_bits(f->m) % 32) != 0);

	normalize(f);
	return f;
}

struct bigfloat *bf_log2_min()
{
	struct bigfloat *f;

	f = malloc(sizeof(struct bigfloat)),
	f->sign = 1;
	f->m = bn_fromstring32(LOG2);
	f->e = 0;
	f->e -= bn_bits(f->m) / 32;
	f->e -= ((bn_bits(f->m) % 32) != 0);

	normalize(f);
	return f;
}

struct bigfloat *bf_log2_max()
{
	struct bigfloat *f;
	struct bignat *bnt;

	f = malloc(sizeof(struct bigfloat)),
	f->sign = 1;
	f->m = bn_fromstring32(LOG2);

	bnt = f->m;
	f->m = bn_succ(f->m);
	bn_destroy(bnt);

	f->e = 0;
	f->e -= bn_bits(f->m) / 32;
	f->e -= ((bn_bits(f->m) % 32) != 0);


	normalize(f);
	return f;
}

void bf_print(struct bigfloat *f)
{
	if (f->sign == 1)
		printf("+");
	else
		printf("-");

	bn_print(f->m);
	printf("x(2^32)^%"PRId32"",f->e);
}

void bf_print_hex(struct bigfloat *f)
{
	if (f->sign == 1)
		printf("+");
	else
		printf("-");

	bn_print_hex(f->m);
	printf("x(2^32)^%"PRId32"",f->e);
}

void bf_print_common_prefix(struct bigfloat *f, struct bigfloat *g)
{
	uint64_t i,j; /* indici dei bit */
	uint64_t maxbits;

	if ((f->e + f->m->n != g->e + g->m->n) || (f->sign != g->sign)) {
		printf("X");
		return;
	}

	if (f->sign == 1)
		printf("+");
	else
		printf("-");

	printf("0,");
	i = f->m->n*32 - 1;
	j = g->m->n*32 - 1;
	while (i < UINT64_MAX && j < UINT64_MAX &&
		bn_get(f->m, i) == bn_get(g->m, j)) {
		putchar('0'+bn_get(f->m,i));
		i--;
		j--;
	}

	printf(" x ");
	printf("(2^32)^%"PRId32"",f->e + f->m->n);
}

long long int bf_accuracy(struct bigfloat *f, struct bigfloat *g)
{
	uint64_t e;
	struct bignat *u, *v; /* addendi parziali */
	uint64_t i;

	if (f->e < g->e) {
		u = bn_copy(f->m);
		v = bn_lshift(g->m, g->e - f->e);
		e = f->e;
	} else if (f->e > g->e) {
		u = bn_lshift(f->m, f->e - g->e);
		v = bn_copy(g->m);
		e = g->e;
	} else {
		u = bn_copy(f->m);
		v = bn_copy(g->m);
		e = f->e;
	}

	if (bn_bits(u) >= bn_bits(v))
		i = bn_bits(u);
	else
		i = bn_bits(v);

	while (i > 0 && bn_get(u, i) == bn_get(v, i)) i--;

	bn_destroy(u);
	bn_destroy(v);

	return (e * 32) + i;
}

void bf_destroy(struct bigfloat *f)
{
	bn_destroy(f->m);
	free(f);
}

/* Implementazione funzioni ausiliarie */
static uint32_t digit(struct bignat *u, uint32_t i)
{
	if (i <= u->n - 1)
		return u->u[i];
	else
		return 0;
}

static struct bignat *bn_lshift(struct bignat *u, uint32_t n)
{
	struct bignat *l;

	l = malloc(sizeof(struct bignat));
	l->n = u->n + n;
	l->u = malloc(sizeof(uint32_t)*l->n);
	
	for (uint32_t i = 0; i < n; i++)
		l->u[i] = 0;

	for (uint32_t i = n; i < l->n; i++)
		l->u[i] = digit(u, i - n);

	return l;
}

static struct bignat *bn_rshift(struct bignat *u, uint32_t n)
{
	struct bignat *r;

	if (u->n <= n)
		return bn_zero();

	r = malloc(sizeof(struct bignat));
	r->n = u->n - n;
	r->u = malloc(sizeof(uint32_t)*r->n);

	for (uint32_t i = 0; i < r->n; i++)
		r->u[i] = digit(u, i + n);

	return r;
}

static void normalize(struct bigfloat *f)
{
	unsigned long int i; /* indice cifra meno significativa non nulla */
	struct bignat *tbn;

	if (bn_iszero(f->m)) { /* controllo zero */
		bn_destroy(f->m);
		f->sign = 1;
		f->m = bn_zero();
		f->e = 0;
		return;
	}

	for (i = 0; f->m->u[i] == 0 && i < f->m->n; i++) {}

	if (i > 0) {
		tbn = f->m;
		f->m = bn_rshift(f->m, i);
		bn_destroy(tbn);

		f->e += i;
	}
}
