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
 - l'errore che ci interessa è assoluto, non relativo; dato y che approssima x, esso è preciso fino a k cifre dopo la virgola se y=x+e, con 0\<e\<2^(-k); in altri termini, dato x' = x + 2^-(k+1), y = x'+err, con |err|<2^(k+1);
 - va studiata la propagazione e l'introduzione di errori (assoluti) nel calcolo della variante della codifica di Ackermann (errore inerente, analitico e algoritmico?); in particolare bisognerà studiare le operazioni atomiche (somma, moltiplicazione, divisione/reciproco) e il troncamento della serie di Taylor per 2^(-x);
 - una volta ottenuto il DAG, conoscendo l'errore massimo relativo tollerato si potrebbe stimare e tenere sotto controllo il propagarsi dell'errore dalle foglie alla radice; in caso risulti complesso o impratico, la computazione sul DAG va strutturata in modo da poter migliorare la precisione iterativamente, fino a poter assegnare ad ogni nodo un valore differente.
