/*
 * github.com/nrizzo
 *
 * Implementazione della liberia per creazione e manipolazione di intervalli di
 * razionali, aperti o chiusi; in particolare è supportata l'operazione di somma
 * e l'approssimazione la potenza di 2 con esponente la negazione di un
 * intervallo positivo.
 */

#include <stdlib.h> /* malloc e free */
#include <stdio.h> /* printf */

#include "../bignat/bignat.h"
#include "../bigfloat/bigfloat.h"
#include "interval.h"

/* Dichiarazione funzioni ausiliarie */
/*
 * rec_exp_2_min e rec_exp_2_max, dati il puntatore ad un bigfloat x e gli
 * interi de, dn e da, approssima 2^(-x) rispettivamente per difetto e per
 * eccesso con un polinomio di MacLaurin tale che:
 *  - l'approssimazione di x * log(2) è approssimata a de cifre;
 *  - il polinomio di MacLaurin ha grado maggiore o uguale a dn;
 *  - ad ogni passo del calcolo del polinomio il risultato parziale è
 *     approssimato a da cifre;
 * va accoppiata a bf_destroy per liberare lo spazio occupato.
 */
struct bigfloat *rec_exp_2_min(struct bigfloat *xmax, long long de,
	long long dn, long long da);
struct bigfloat *rec_exp_2_max(struct bigfloat *xmin, long long de,
	long long dn, long long da);

/*
 * rec_exp_2_min_frac e rec_exp_2_max_frac, dati il puntatore ad un bigfloat x
 * rappresentante un numero razionale non negativo minore di 1 e gli interi de,
 * dn e da, approssima 2^(-x) rispettivamente per difetto e per eccesso con un
 * polinomio di MacLaurin tale che:
 *  - l'approssimazione di x * log(2) è approssimata a de cifre;
 *  - il polinomio di MacLaurin ha grado maggiore o uguale a dn;
 *  - ad ogni passo del calcolo del polinomio il risultato parziale è
 *     approssimato a da cifre;
 * va accoppiata a bf_destroy per liberare lo spazio occupato.
 */
struct bigfloat *rec_exp_2_max_frac(struct bigfloat *xmin, long long de,
	long long dn, long long da);
struct bigfloat *rec_exp_2_min_frac(struct bigfloat *xmax, long long de,
	long long dn, long long da);

/*
 * rec_exp_e_min_frac e rec_exp_e_max_frac, dati il puntatore ad un bigfloat x
 * rappresentante un numero razionale non negativo minore di log(2) e gli interi
 * dn e da, approssima e^(-x) rispettivamente per difetto e per eccesso con un
 * polinomio di MacLaurin tale che:
 *  - il polinomio di MacLaurin ha grado maggiore o uguale a dn;
 *  - ad ogni passo del calcolo del polinomio il risultato parziale è
 *     approssimato a da cifre;
 * va accoppiata a bf_destroy per liberare lo spazio occupato.
 */
struct bigfloat *rec_exp_e_min_frac(struct bigfloat *ymax, long long dn,
	long long da);
struct bigfloat *rec_exp_e_max_frac(struct bigfloat *ymin, long long dn,
	long long da);


struct interval *interval_zero()
{
	struct interval *X;

	X = malloc(sizeof(struct interval));
	X->incl = 1;
	X->min = bf_zero();
	X->max = bf_zero();

	return X;
}

struct interval *interval_create(struct bigfloat *min, struct bigfloat *max)
{
	struct interval *X;

	X = malloc(sizeof(struct interval));
	X->incl = 1;
	X->min = bf_copy(min);
	X->max = bf_copy(max);

	return X;
}

struct interval *interval_add(struct interval *X, struct interval *Y)
{
	struct interval *Z;

	Z = malloc(sizeof(struct interval));
	Z->incl = ((X->incl) && (Y->incl));
	Z->min = bf_add(X->min, Y->min);
	Z->max = bf_add(X->max, Y->max);

	return Z;
}

struct interval *interval_rec_exp_2(struct interval *X, long long de, long long dn, long long da)
{
	struct interval *Y;

	Y = malloc(sizeof(struct interval));
	Y->min = rec_exp_2_min(X->max,de,dn,da);
	Y->max = rec_exp_2_max(X->min,de,dn,da);

	if (bf_cmp(Y->min, Y->max) == 0)
		Y->incl = 1;
	else
		Y->incl = 0;

	return Y;
}

void interval_print_common_prefix(struct interval *X)
{
	bf_print_common_prefix(X->min, X->max);
}

void interval_print(struct interval *X)
{
	if (X->incl)
		printf("[");
	else
		printf("(");

	bf_print_hex(X->min);
	printf(", ");
	bf_print_hex(X->max);

	if (X->incl)
		printf("]");
	else
		printf(")");
}

long long int interval_accuracy(struct interval *X)
{
	return bf_accuracy(X->min, X->max);
}

void interval_destroy(struct interval *X)
{
	bf_destroy(X->min);
	bf_destroy(X->max);
	free(X);
}

/* Implementazione funzioni ausiliarie */
struct bigfloat *rec_exp_2_min(struct bigfloat *xmax, long long de,
	long long dn, long long da)
{
	struct bignat *whole;
	struct bigfloat *fraction;
	struct bigfloat *p;
	struct bigfloat *res;

	whole = bf_tobignat(xmax);
	fraction = bf_frac(xmax);

	if (fraction->m->n == 1 && fraction->m->u[0] == 0)
		p = bf_fromuint32(1);
	else
		p = rec_exp_2_min_frac(fraction, de, dn, da);

	if (whole->n == 1 && whole->u[0] == 0)
		res = bf_copy(p);
	else
		res = bf_div_pow2_bn(p, whole);

	bn_destroy(whole);
	bf_destroy(fraction);
	bf_destroy(p);

	return res;
}

struct bigfloat *rec_exp_2_max(struct bigfloat *xmin, long long de,
	long long dn, long long da)
{
	struct bignat *whole;
	struct bigfloat *fraction;
	struct bigfloat *p;
	struct bigfloat *res;

	whole = bf_tobignat(xmin);
	fraction = bf_frac(xmin);

	if (fraction->m->n == 1 && fraction->m->u[0] == 0)
		p = bf_fromuint32(1);
	else
		p = rec_exp_2_max_frac(fraction, de, dn, da);

	if (whole->n == 1 && whole->u[0] == 0)
		res = bf_copy(p);
	else
		res = bf_div_pow2_bn(p, whole);

	bn_destroy(whole);
	bf_destroy(fraction);
	bf_destroy(p);

	return res;
}

struct bigfloat *rec_exp_2_min_frac(struct bigfloat *xmax, long long de,
	long long dn, long long da)
{
	struct bigfloat *log2 = bf_log2_max();
	struct bigfloat *ymax = bf_mul(xmax, log2);
	struct bigfloat *t, *res;

	t = ymax;
	ymax = bf_round(ymax, de);
	bf_destroy(t);

	res = rec_exp_e_min_frac(ymax, dn, da);

	bf_destroy(log2);
	bf_destroy(ymax);

	return res;
}

struct bigfloat *rec_exp_2_max_frac(struct bigfloat *xmin, long long de,
	long long dn, long long da)
{
	struct bigfloat *log2 = bf_log2_min();
	struct bigfloat *ymin = bf_mul(xmin, log2);
	struct bigfloat *res, *t;

	t = ymin;
	ymin = bf_trunc(ymin, de);
	bf_destroy(t);

	res = rec_exp_e_max_frac(ymin, dn, da);

	bf_destroy(log2);
	bf_destroy(ymin);

	return res;
}

struct bigfloat *rec_exp_e_min_frac(struct bigfloat *ymax, long long dn,
	long long da)
{
	struct bigfloat *res;
	struct bigfloat *t;

	struct bigfloat *one = bf_fromuint32(1);

	res = bf_zero();
	for (int i = ((dn/2)*2)+1; i > 0; i--) {
		t = res;
		if (i % 2 == 0)
			res = bf_add(res,one);
		else
			res = bf_sub(res,one);
		bf_destroy(t);

		t = res;
		res = bf_mul(res, ymax);
		bf_destroy(t);

		t = res;
		res = bf_trunc(res, da);
		bf_destroy(t);

		t = res;
		res = bf_div_uint32_trunc(res, i, da);
		bf_destroy(t);
	}

	t = res;
	res = bf_add(res, one);
	bf_destroy(t);

	bf_destroy(one);
	return res;
}

struct bigfloat *rec_exp_e_max_frac(struct bigfloat *ymin, long long dn,
	long long da)
{
	struct bigfloat *res;
	struct bigfloat *t;

	struct bigfloat *one = bf_fromuint32(1);

	res = bf_zero();
	for (int i = ((dn/2)*2)+2; i > 0; i--) {
		t = res;
		if (i % 2 == 0)
			res = bf_add(res,one);
		else
			res = bf_sub(res,one);
		bf_destroy(t);

		t = res;
		res = bf_mul(res, ymin);
		bf_destroy(t);

		t = res;
		res = bf_round(res, da);
		bf_destroy(t);

		t = res;
		res = bf_div_uint32_round(res, i, da);
		bf_destroy(t);
	}

	t = res;
	res = bf_add(res, one);
	bf_destroy(t);

	bf_destroy(one);
	return res;
}
