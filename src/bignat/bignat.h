/*
 * github.com/nrizzo
 *
 * Intestazione della libreria per creazione e operazioni aritmetiche di numeri
 * naturali di grandezza arbitraria.
 */

#ifndef BNGUARD /* guardia */
#define BNGUARD

#include <inttypes.h> /* uint32_t */

/*
 * Struct che rappresenta un numero naturale di grandezza arbitraria; le cifre
 * di u sono salvate dalla meno significativa alla più significativa, e la
 * rappresentazione è valida se n >= 1 e la cifra più significativa di u non è
 * nulla.
 */
struct bignat {
	uint32_t  n; /* numero di cifre in base 2^32 */
	uint32_t *u; /* puntatore all'array di uint32_t */
};

/*
 * bn_zero crea e restituisce il puntatore al bignat rappresentante lo zero;
 * va accoppiata a bn_destroy per liberarne lo spazio occupato.
 */
struct bignat *bn_zero();

/*
 * bn_fromuint32 crea e restituisce il puntatore al bignat rappresentato
 * dall'argomento i; va accoppiata a bn_destroy per liberare lo spazio occupato.
 */
struct bignat *bn_fromuint32(uint32_t i);

/*
 * bn_fromuint32 crea e restituisce il puntatore al bignat rappresentato
 * dall'argomento i; va accoppiata a bn_destroy per liberare lo spazio occupato.
 */
struct bignat *bn_fromuint64(uint64_t i);

/*
 * bn_fromstring crea e restituisce il puntatore al bignat rappresentato dalla
 * stringa s; non modifica s, va accoppiata a bn_destroy per liberare lo spazio
 * occupato.
 */
struct bignat *bn_fromstring(char *s);

/*
 * bn_fromstring32 crea e restituisce il puntatore al bignat rappresentato dalla
 * stringa s, aggiungendo eventuali zeri fino a raggiungere un multiplo di 32;
 * non modifica s, va accoppiata a bn_destroy per liberare lo spazio
 * occupato.
 */
struct bignat *bn_fromstring32(char *s);

/*
 * bn_copy crea e restituisce una copia del bignat passato come argomento; va
 * accoppiata a bn_destroy per liberare lo spazio occupato.
 */
struct bignat *bn_copy(struct bignat *u);

/*
 * bn_bits restituisce il minor numero di cifre binarie necessarie a
 * rappresentare il naturale rappresentato dall'argomento.
 */
uint64_t bn_bits(struct bignat *u);

/*
 * bn_get restituisce l'(i+1)-esima cifra meno significativa della
 * rappresentazione in base 2 di u.
 */
char bn_get(struct bignat *u, uint64_t i);

/*
 * bn_cmp, dati due bignat u e v, restituisce 1 se u > v, 0 se u = v e -1 se
 * u < v.
 */
int bn_cmp(struct bignat *u, struct bignat *v);

/*
 * bn_iszero, dato il bignat u, restituisce 1 se questo rappresenta lo zero,
 * 0 altrimenti.
 */
char bn_iszero(struct bignat *u);
/* 
 * bn_succ e bn_pred, dato il puntatore ad un bignat, creano e restituiscono
 * il bignat che rappresenta il numero naturale rispettivamente successivo e
 * precedente all'argomento, ammesso questo esista (altrimenti il funzionamento
 * non è definito); va accoppiata a bn_destroy per liberarne lo
 * spazio.
 */
struct bignat *bn_succ(struct bignat *u);
struct bignat *bn_pred(struct bignat *v);

/*
 * bn_add, dati i puntatori a due bignat, crea e restituisce il puntatore
 * al bignat rappresentante la somma dei due argomenti; non modifica gli
 * argomenti e va accoppiata a bn_destroy per liberarne lo spazio.
 */
struct bignat *bn_add(struct bignat *u, struct bignat *v);

/*
 * bn_sub, dati due bignat rappresentanti u e v tali che u >= v, crea e
 * restituisce il bignat rappresentante la differenza di u con v; non modifica
 * gli argomenti e va accoppiata a bn_destroy per liberarne lo spazio.
 */
struct bignat *bn_sub(struct bignat *u, struct bignat *v);

/*
 * bn_mul, dati due bignat rappresentati u e v, crea e restituisce il bignat
 * rappresentante il prodotto di u per v; non modifica gli argomenti e va
 * accoppiata a bn_destroy per liberarne lo spazio.
 */
struct bignat *bn_mul(struct bignat *u, struct bignat *v);

/*
 * bn_div_uint32, dato il bignat u e l'intero i, crea e restituisce il bignat
 * rappresentante la divisione intera di u / i; non modifica u e va accoppiata a
 * bn_destroy per liberarne lo spazio.
 */
struct bignat *bn_div_uint32(struct bignat *u, uint32_t n);

/*
 * bn_remcheck_uint32, dato il bignat u e l'intero i, restituisce 1 se il bignat
 * la divisione intera di u / i ha resto diverso da 0, 0 altrimenti; non
 * modifica u.
 */
char bn_remcheck_uint32(struct bignat *u, uint32_t n);

/*
 * bn_rem_uint32, dato il bignat u e l'intero i, crea e restituisce il bignat
 * rappresentante il resto della divisione intera di u / i; non modifica u e va
 * accoppiata a bn_destroy per liberarne lo spazio.
 */
uint32_t bn_rem_uint32(struct bignat *u, uint32_t n);
/*
 * bn_print stampa sullo standard output la rappresentazione in base 2 del
 * bignat passato come argomento.
 */
void bn_print(struct bignat *u);

/*
 * bn_print_hex stampa sullo standard output la rappresentazione in base 16 del
 * bignat passato come argomento.
 */
void bn_print_hex(struct bignat *u);

/*
 * bn_scan legge lo standard input per una rappresentazione binaria di un intero
 * e crea il bignat corrispondente.
 */
struct bignat *bn_scan();

/*
 * bn_destroy libera lo spazio occupato dal bignat u.
 */
void bn_destroy(struct bignat *u);

#endif /* BNGUARD */
