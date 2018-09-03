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

Una possibile soluzione per la costruzione del DAG potrebbe basarsi su una coda (un semplice array) in cui gli elementi da processare sono nodi del DAG in costruzione, disposti in ordine decrescente rispetto all'intero corrispondente all'insieme; un algoritmo potrebbe, iterativamente, prendere il primo nodo della coda, calcolarne i discendenti in ordine decrescente e con un merge lineare (ottimizzabile) creare i nodi dei discendenti se non esistono, aggiungere i dovuti archi e inserirli nella coda, fino ad esaurimento.

### Approssimazione della variante della codifica di Ackermann
Considerazioni:
 - una soluzione pratica, alternativa al concetto di errore, potrebbe basarsi sul concetto di intervallo: sapendo che un numero reale sta in un determinato intervallo (i cui estremi possono essere inclusi come no), questo numero presenta come prime cifre binarie il prefisso in comune dei due estremi (se esiste)?
 - la propagazione e l'introduzione di errori di calcolo può rivelarsi insidiosa: anche una semplice somma di dati così approssimati può portare ad incertezza su un numero di cifre difficilmente stimabile prima di effettuare la computazione (alcune serie di interi interessanti a riguardo sono la A001511, cioè la lunghezza massima del suffisso di soli 0 nella rappresentazione binaria, la A091090, numero di cifre binarie modificate dall'incremento unitario);
 - una volta ottenuto il DAG, conoscendo l'errore massimo relativo tollerato si potrebbe stimare e tenere sotto controllo il propagarsi dell'errore dalle foglie alla radice; in caso risulti complesso o impratico, la computazione sul DAG va strutturata in modo da poter migliorare la precisione iterativamente, fino a poter assegnare ad ogni nodo un valore differente.

## Scaletta
