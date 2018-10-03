# hfs-codes
Progetto di tirocinio e tesi riguardante insiemi ereditariamente finiti, grafi di appartenenza e codifiche sugli insiemi.

## Prerequisiti
Il software è stato testato su architettura x86\_64, i686, AArch32 e AArch64. Viene richiesta un'installazione di `make` e di `gcc`.

## Scaricare, compilare ed eseguire
I seguenti comandi scaricano il progetto, lo compilano, generano l'eseguibile `algoritmo_risolutivo` e ne lanciano l'esecuzione.
```
$ git clone https://github.com/nrizzo/hfs-codes
$ make
$ make clean
$ ./algoritmo_risolutivo
```

## Cosa fa
Il programma risolutivo (per ora) legge da standard input i caratteri dell'espansione binaria di un codice di Ackermann N\_A(x) (si veda `tesi.pdf`), calcola il grafo di appartenenza di x, calcola un'approssimazione di R\_A per ogni nodo del grafo di appartenenza per poi calcolare una stima di R\_A(x). Restituisce su standard output la rappresentazione del grafo in formato DOT, con etichettate le approssimazioni, e poi l'approssimazione di R\_A(x).

## Precisione
I parametri di precisione per l'approssimazione di R\_A sono (per ora) statici e si trovano in `src/main.c`: la loro modifica per avere effetto deve essere seguita da una ricompilazione.

## Esempi
```
$ ./algoritmo_risolutivo
$ 100
> digraph G_T {
> 10 [label="10\n[+80000000x(2^32)^-1, +80000000x(2^32)^-1]"];
> 1 [label="1\n[+00000001x(2^32)^0, +00000001x(2^32)^0]"];
> 0 [label="0\n[+00000000x(2^32)^0, +00000000x(2^32)^0]"];
> 10 -> 1;
> 1 -> 0;
> }
> (+B504F333 F9DE6484x(2^32)^-2, +B504F333 F9DE6485x(2^32)^-2)
```
```
$ ./algoritmo_risolutivo
$ 11000
> digraph G_T {
> 100 [label="100\n(+B504F333 F9DE6484x(2^32)^-2, +B504F333 F9DE6485x(2^32)^-2)"];
> 11 [label="11\n[+00000001 80000000x(2^32)^-1, +00000001 80000000x(2^32)^-1]"];
> 10 [label="10\n[+80000000x(2^32)^-1, +80000000x(2^32)^-1]"];
> 1 [label="1\n[+00000001x(2^32)^0, +00000001x(2^32)^0]"];
> 0 [label="0\n[+00000000x(2^32)^0, +00000000x(2^32)^0]"];
> 100 -> 10;
> 11 -> 0;
> 11 -> 1;
> 10 -> 1;
> 1 -> 0;
> }
> (+F7526068 B670F2C5x(2^32)^-2, +F7526068 B670F2D1 80000000x(2^32)^-3)
```
