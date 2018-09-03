/*
 * github.com/nrizzo
 *
 * Intestazione della libreria per creazione e manipolazione di intervalli di
 * razionali, aperti o chiusi; in particolare è supportata l'operazione di somma
 * e l'approssimazione della potenza di 2 con esponente la negazione di un
 * intervallo positivo.
 */

#ifndef INTGUARD
#define INTGUARD

#include "../bigfloat/bigfloat.h"

/*
 * Struct che rappresenta un intervallo di numeri razionali.
 */
struct interval{
	char incl; /* 1 se estremi inclusi, 0 se esclusi */
	struct bigfloat *min;
	struct bigfloat *max;
};

/*
 * interval_zero crea e restituisce il puntatore all'intervallo degenere
 * rappresentante lo zero; va accoppiata a interval_destroy per liberare lo
 * spazio occupato.
 */
struct interval *interval_zero();

/*
 * interval_create, dati due bigfloat min e max tali che min < max, crea e
 * restituisce il puntatore all'intervallo con estremi min e max inclusi; va
 * accoppiata a interval_destroy per liberare lo spazio occupato.
 */
struct interval *interval_create(struct bigfloat *min, struct bigfloat *max);

/*
 * interval_add, dati due intervalli X e Y (indipendenti), crea e restituisce
 * il puntatore all'interval rappresentante la somma X + Y; va accoppiata a
 * interval_destroy per liberare lo spazio occupato.
 */
struct interval *interval_add(struct interval *X, struct interval *Y);

/*
 * interval_rec_exp_2, dati l'intervallo X e gli interi de, dn e da, approssima
 * l'intervallo 2^(-X) con un polinomio di MacLaurin tale che:
 *  - l'approssimazione di X * log(2) è approssimata a de cifre;
 *  - il polinomio di MacLaurin ha grado maggiore o uguale a dn;
 *  - ad ogni passo del polinomio di MacLaurin il risultato parziale è
 *     approssimato a da cifre;
 * va accoppiata a interval_destroy per liberare lo spazio occupato.
 */
struct interval *interval_rec_exp_2(struct interval *X, long long de,
	long long dn, long long da);

/*
 * interval_print stampa gli estremi dell'intervallo X in notazione esadecimale,
 * circondati da parentesi quadre se sono inclusi, tonde se esclusi.
 */
void interval_print(struct interval *X);

/*
 * interval_print_common_prefix stampa il prefisso in comune della
 * rappresentazione binaria degli estremi di X, se questo esiste; altrimenti
 * stampa 'X'.
 */
void interval_print_common_prefix(struct interval *X);

/*
 * interval_accuracy, dato l'intervallo X, restituisce la posizione dell'ultima
 * cifra binaria in comune degli estremi di X.
 */
long long int interval_accuracy(struct interval *X);

/*
 * interval_destroy libera lo spazio occupato dall'interval X e da tutte le sue
 * componenti.
 */
void interval_destroy(struct interval *X);

#endif /* INTGUARD */
