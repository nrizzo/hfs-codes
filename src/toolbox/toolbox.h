/*
 * Libreria di funzioni utili.
 */

/* reciprocal_exp2, dato x>0,  calcola il risultato di f(x) = 2^(-x) con errore
 * massimo assoluto tol. */
double reciprocal_exp2(double x, double tol);

/* reciprocal_exp2_aux, dato 0<x<1, calcola il risultato di f(x) = 2^(-x), con
 * tol massimo errore assoluto. */
double reciprocal_exp2_aux(double x, double tol);
