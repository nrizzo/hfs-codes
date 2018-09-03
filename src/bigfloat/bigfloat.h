/*
 * github.com/nrizzo
 *
 * Intestazione della libreria per creazione, operazioni aritmetiche e
 * operazioni di approssimazione su numeri razionali di precisione arbitraria.
 */

#ifndef BFGUARD
#define BFGUARD

#include <inttypes.h> /* uint32_t */
#include "../bignat/bignat.h"

#define LOG2 "10110001011100100001011111110111110100011100111101111001101010111100100111100011101100111001100000000011111100101111011010101111010000001111001101000011001001100111001010011000101101100010110110001010000011010001011101011011100010111010101011111010001010111110011110111000011101100010000001101101111010111010110010011000010101011001010101010010111110110100101011111010000110110001000011101101001011101010111000110101110000010011100000100001010001000010011101010111001110110010100100010001011010011011100000100101001111101001011011001010000101100010001001001010111010001100010100011010110010111101101000010001001100010111110000111000011111101011100111101010100110111100001110110001001101100110000000111011001001010110111110100000111011000111011001010111111101110100101101110010110011101000011110110001100111010110010101001000110010101111010111011111101001101011110100111000001100000011001001001000011001010101111110100001100001110010111100100000111000111010001011011010001011011001011111000101000011110011111111010101110001" /* approssimazione per difetto delle cifre dopo la virgola di log(2) (base 2) */

/*
 * Struct che rappresenta un numero razionale di precisione arbitraria, salvato
 * come esponente e mantissa; la mantissa è rappresentata da un bignat ed
 * assieme a segno ed esponente rappresenta il numero razionale
 * (sign) * m * (2^32)^e. Per evitare ambiguità nella rappresentazione la cifra
 * meno significativa (in base 2^32) di m deve essere diversa da 0; per
 * convenzione lo zero ha segno positivo ed esponente pari a 0.
 */
struct bigfloat {
	signed char sign; /* 1 o -1 */
	struct bignat *m; /* mantissa */
	int64_t e; /* esponente */
};

/*
 * bf_zero crea e restituisce il puntatore al bigfloat rappresentante lo zero;
 * va accoppiata a bn_destroy per liberarne lo spazio.
 */
struct bigfloat *bf_zero();

/*
 * bf_fromuint32 crea e restituisce il puntatore al bigfloat rappresentante il
 * numero naturale passato come argomento; va accoppiata a bn_destroy per
 * liberarne lo spazio.
 */
struct bigfloat *bf_fromuint32(uint32_t i);

/*
 * bf_copy crea e restituisce una copia (deep) del bignat passato come
 * argomento; va accoppiata a bf_destroy per liberarne lo spazio.
 */
struct bigfloat *bf_copy(struct bigfloat *f);

/*
 * bf_log2_min e bf_log2_max creano e restituiscono un bigfloat che approssima
 * rispettivamente per difetto e per difetto la costante log(2); ogni chiamata
 * va accoppiata a bf_destroy per liberarne lo spazio.
 */
struct bigfloat *bf_log2_min();
struct bigfloat *bf_log2_max();

/*
 * bf_cmp, dati i puntatori a due bigfloat f e g, restituisce 1 se f > g,
 * -1 se f < g, 0 se f = g.
 */
signed char bf_cmp(struct bigfloat *f, struct bigfloat *g);

/*
 * bf_add, dati i puntatori a due bigfloat f e g, crea e restituisce il bigfloat
 * rappresentante la somma f + g; va accoppiata a bf_destroy per liberarne lo
 * spazio occupato.
 */
struct bigfloat *bf_add(struct bigfloat *f, struct bigfloat *g);

/*
 * bf_sub, dati i puntatori a due bigfloat f e g, crea e restituisce il bigfloat
 * rappresentante la differenza f - g; va accoppiata a bf_destroy per liberarne
 * lo spazio occupato.
 */
struct bigfloat *bf_sub(struct bigfloat *f, struct bigfloat *g);

/*
 * bf_mul, dati i puntatori a due bigfloat f e g, crea e restituisce il bigfloat
 * rappresentante il prodotto f * g; va accoppiata a bf_destroy per liberarne lo
 * spazio occupato.
 */
struct bigfloat *bf_mul(struct bigfloat *f, struct bigfloat *g);

/*
 * bf_trunc, dato il puntatore ad un bigfloat f, crea e restituisce il bigfloat
 * rappresentante l'approssimazione per difetto di f (anche se questo è
 * negativo) precisa fino alla cifra della potenza (2^32)^d; cioè rappresenta
 * una frazione con denominatore (2^32)^d e l'errore commesso è minore di
 * (2^32)^d; va accoppiata a bf_trunc per liberarne lo spazio occupato.
 */
struct bigfloat *bf_trunc(struct bigfloat *f, long long int d);

/*
 * bf_round, dato il puntatore ad un bigfloat f, crea e restituisce il bigfloat
 * rappresentante l'approssimazione per eccesso di f (anche se questo è
 * positivo) precisa fino alla cifra della potenza (2^32)^d; cioè rappresenta
 * una frazione con denominatore (2^32)^d e l'errore commesso è minore di
 * (2^32)^d; va accoppiata a bf_trunc per liberarne lo spazio occupato.
 */
struct bigfloat *bf_round(struct bigfloat *f, long long int d);

/*
 * bf_tobignat, dato il bigfloat f, crea e restituisce il bignat rappresentante
 * la parte intera di |f|; va accoppiata a bn_destroy per liberarne lo spazio
 * occupato.
 */
struct bignat *bf_tobignat(struct bigfloat *f);

/*
 * bf_frac, dato il bigfloat f, crea e restituisce il bignat rappresentante la
 * parte frazionaria di |f|; va accoppiata a bf_destroy per liberarne lo spazio
 * occupato.
 */
struct bigfloat *bf_frac(struct bigfloat *f);

/*
 * bf_div_uint32_trunc, dati il bigfloat f, il numero naturale i e l'intero d,
 * crea e restituisce l'approssimazione del bigfloat rappresentante la divisione
 * di f per i precisa fino alla cifra della potenza (2^32)^d, cioè è una
 * frazione con denominatore (2^32)^d e l'errore commesso è minore di (2^32)^d.
 */
struct bigfloat *bf_div_uint32_trunc(struct bigfloat *f, uint32_t i,
	long long int d);

/*
 * bf_div_uint32_round, dati il bigfloat f, il numero naturale i e l'intero d,
 * crea e restituisce l'approssimazione per eccesso (anche quando f è negativo)
 * del bigfloat rappresentante la divisione di f per i precisa fino alla cifra
 * della potenza (2^32)^d, cioè è una frazione con denominatore (2^32)^d e
 * l'errore commesso è minore di (2^32)^d.
 */
struct bigfloat *bf_div_uint32_round(struct bigfloat *f, uint32_t i,
	long long int d);

/*
 * bf_rshift e bf_lshift, dati il bigfloat f e l'intero d, creano e
 * restituiscono il bigfloat rappresentante rispettivamente la divisione e la
 * moltiplicazione di f per (2^32)^d; vanno accoppiate a bf_destroy per
 * liberarne lo spazio occupato.
 */
struct bigfloat *bf_rshift(struct bigfloat *f, long long int d);
struct bigfloat *bf_lshift(struct bigfloat *f, long long int d);

/*
 * bf_div_pow2_bn, dati il bigfloat f e il bignat n, crea e restituisce il
 * bigfloat rappresentante il numero razionale f / 2^n; va accoppiata a
 * bf_destroy per liberarne lo spazio occupato.
 */
struct bigfloat *bf_div_pow2_bn(struct bigfloat *f, struct bignat *n);

/*
 * bf_scan legge lo standard input e restituisce il bigfloat rappresentato dalla
 * sequenza di cifre binarie lette, come se queste fossero le cifre dopo la
 * virgola di un numero in notazione binaria.
 */
struct bigfloat *bf_scan();

/*
 * bf_print, dato il puntatore ad un bigfloat f, ne stampa su standard output
 * la rappresentazione della mantissa, in notazione binaria, e della potenza di
 * di (2^32) elevata all'esponente, in base decimale.
 */
void bf_print(struct bigfloat *f);

/*
 * bf_print_hex, dato il puntatore ad un bigfloat f, ne stampa su standard
 * output la rappresentazione della mantissa, in notazione esadecimale, e della
 * potenza di (2^32) elevata all'esponente, in base decimale.
 */
void bf_print_hex(struct bigfloat *f);

/*
 * bf_print_common_prefix, dati i puntatori ai bigfloat f e g, ne stampa su
 * standard output il prefisso in comune, se esiste, con la potenza di (2^32)
 * corrispondente; altrimenti stampa 'X'.
 */
void bf_print_common_prefix(struct bigfloat *f, struct bigfloat *g);

/*
 * bf_accuracy, dati i puntatori ai bigfloat f e g, restituisce l'indice
 * della potenza di due meno significativa in comune alla rappresentazione
 * binaria di f e g.
 */
long long int bf_accuracy(struct bigfloat *f, struct bigfloat *g);

/*
 * bf_destroy libera lo spazio occupato da una delle chiamate che creano
 * bigfloat.
 */
void bf_destroy(struct bigfloat *f);

#endif /* BFGUARD */
