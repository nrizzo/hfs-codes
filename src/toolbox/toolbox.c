#include <math.h>
#include <stdio.h> /* debug */

#include "toolbox.h"

double reciprocal_exp2(double x, double tol)
{
	double fl = floor(x); /* parte intera */
	double fr = x - fl; /* parte frazionaria */

	/* 2^(-x) = 2^(-fl)*2^(-fr) */
	return pow(2,-fl) * reciprocal_exp2_aux(fr,tol);
}

/* calcn, dato x tale che 0<x<1, calcola il più piccolo grado del polinomio di
 * McLaurin per 2^(-x) necessario per garantire un errore (il resto di Lagrange)
 * più piccolo di tol. */
int calcn(double x, double tol);

double reciprocal_exp2_aux(double x, double maxtol)
{
	double res;
	double log2x = log(2)*x;

	res = 0;
	for (int i = calcn(x, maxtol); i > 0; i--) {
		if (i%2 == 0)
			res += 1;
		else
			res += -1;

		res *= log2x;
		res /= i;
	}
	res += 1;

	return res;
}

int calcn(double x, double tol)
{
	/* Il (valore assoluto del) resto di Lagrange per il polinomio di
	 * McLaurin di grado n per 2^(-x) è
	 *  |R_n (x)| = ( (log2)^(n+1) * 2^(-$) * x^(n+1) )/(n+1)!
	 *  per un $ in (0,x).
	 * 2^(-y) è una funzione decrescente quindi 2^(-$) <= 2^0 = 1, cioè
	 *  |R_n(x)| <= ( (log2 * x)^(n+1) )/(n+1)! . */
	int n = 0;
	double maxerr = log(2)*x;

	while (tol < maxerr) {
		n++;
		maxerr *= (log(2)*x)/(n+1);
	}

	return n;
}
