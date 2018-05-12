# hfs-codes
Progetto di tirocinio riguardante gli insiemi ereditariamente finiti (hereditarily finite sets) e loro codifiche.

## Il programma in breve
È richiesto un programma che, dati in input due interi n e k, trovi l'insieme ereditariamente finito corrispondente a n secondo la codifica di Ackermann (e il suo insieme finito di equazioni); in seguito deve restituire il valore di questo insieme secondo la codifica alternativa, preciso fino alla k-esima cifra binaria dopo la virgola.

## Idee
Il problema si può considerare diviso in due parti: la costruzione del sistema finito di equazioni e l'approssimazione di R\_A.
### Costruzione del sistema finito di equazioni
Qualche considerazione (da dimostrare):
 - dato n, la sua rappresentazione in base 2 richiede floor(log\_2 n)+1 cifre; quindi il sistema per h\_n contiene al massimo altrettante equazioni; il minimo invece si ottiene se n è il risultato di una tetrazione (elevamento iterato a potenza) di 2 (può essere espresso in funzione dell'inversa della funzione di Ackermann? Notazione di Knuth?);
 - se rappresentiamo il sistema per h\_n come un DAG connesso (l'ordine naturale decrescente degli elementi li dispone già in un ordinamento topologico!), il numero di archi potrebbe essere O((log\_2 n) * log\_2 (log\_2 n)) (intuitivamente si può spiegare contando in base 2 da 0 a log\_2 (log\_2 n))

### Approssimazione della variante della codifica di Ackermann
L'indice di condizionamento della composizione iterata di f(x)=2^(-x) sembra stabilizzarsi (molto bene) dopo la prima iterazione ad un valore più piccolo di 3 per valori ragionevoli.
