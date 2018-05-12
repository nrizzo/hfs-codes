#include <stdio.h>
#include <math.h>
#include <time.h>

#include "toolbox.h"

int main()
{
	long double PI = 3.14159265358979323846264338327950288419716939937510L;
	double maxerr = pow(10,-16);
	double res = reciprocal_exp2(M_PI,maxerr);

	printf("Tolleranza:  %.20f\n",maxerr);
	printf("2^(-Pi)    = %.20f\n",res);	
	printf("pow(2,-Pi) = %.20f\n",pow(2,-M_PI));	
	printf("pow(2,-Pi) = %.30Lf\n",powl(2,-PI));
	printf("Wolfram    = 0.1133147322967608730202821722133293...\n");
	printf("sedicesima cifra dopo virgola-^\n");

	printf("\n == Alcuni test ==\n");
	__clock_t t0 = clock(), t1;
	int n = 1000000;
	printf("L'esecuzione di %d pow richiede ",n);
	for (int i=0; i<n; i++)
		pow(2,-M_PI);
	t1 = clock();
	printf("%f secondi.\n",(double)(t1-t0)/(double)CLOCKS_PER_SEC);

	printf("L'esecuzione di %d reciprocal_exp2 richiede ",n);
	for (int i=0; i<n; i++)
		reciprocal_exp2(M_PI,maxerr);
	t1 = clock();
	printf("%f secondi.\n",(double)(t1-t0)/(double)CLOCKS_PER_SEC);

	printf("L'esecuzione di %d powl richiede ",n);
	for (int i=0; i<n; i++)
		powl(2,-PI);
	t1 = clock();
	printf("%f secondi.\n",(double)(t1-t0)/(double)CLOCKS_PER_SEC);
}
