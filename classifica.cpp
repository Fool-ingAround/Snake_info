#include "classifica.hpp"
classifica::classifica(){       //il costruttore inizializza l'array di nodi come vuoto, con i punteggi a 0 e i nomi vuoti
    for (int i = 0; i < length; i++) {
        strcpy(A[i].nome, "");
        A[i].score=0;
    }
}
void classifica::scambianodo(nodo &x, nodo &y) {   //funzione che esegue lo scambio di due nodi contigui
    nodo temp=x;
    x=y;
    y=temp;
}
bool classifica::isempty() {   //controlla che ci sia almeno un posto libero nell'array
    bool vuoto=false;
    for (int i = 0; i < length && vuoto==false; i++) {   //scorre tutto l'array cercando un posto vuoto
        if (A[i].score==0 && strcmp(A[i].nome, "")==0) {
            vuoto=true;   //se viene trovato si setta il booleano vuoto a true e si esce dal ciclo
        }
    }
    return vuoto;   //ritorna true se c'è almeno un posto libero, false altrimenti
}
void classifica::bubble_sort() {   //ordina l'array in modo decrescente
    int i;
    int j;
    for (i=0; i<length; i++) {
        for (j=0; j<length-i-1; j++) {
            if (A[j].score<A[j+1].score) {
                scambianodo(A[j], A[j+1]);    //esegue scambianodo se non sono ordinati i nomi in base ai punteggi
            }
            if (A[j].score==A[j+1].score) {
                if (strcmp(A[j].nome, A[j+1].nome)>0) {   //se due persone hanno fatto lo stesso punteggio vengono ordinate in modo alfabetico crescente
                    scambianodo(A[j], A[j+1]);
                }
            }
        }
    }
}
void classifica::right_shift(int i) {   //esegue uno shift verso destra degli elementi dell'array a partire da una posizione i per permettere di fare spazio
    if (A[length-1].score!=0 && strcmp(A[length-1].nome, "") != 0) {
        A[length-1].score=0;
        strcpy(A[length-1].nome, "");
    }
    for (int j=length-1; j>i; j--) {
        A[j]=A[j-1];
    }
}
void classifica::inserimento(char player[], int punteggio) {   //prende in input il nome del giocatore e il suo punteggio
    bubble_sort();  //effettua un ordinamento
    bool trovato = false;
    int i;
    for (i = 0; i < length; i++) {
        if (strcmp(A[i].nome, player) == 0) {     //si scorre tutto l'array e se si scopre che il giocatore è lo stesso e ha fatto un punteggio migliore del suo ultimo allora viene aggiornato il punteggio
            trovato = true;
            if (punteggio > A[i].score) {
                A[i].score = punteggio;
            }
        }
    }
    if (trovato==false) {  //se non è stato trovato un nome uguale a uno già presente si scorre l'array cercando un posto per inserire il nuovo nodo
        if (isempty() || punteggio > A[length - 1].score) {   //se l'array ha spazi vuoti o il punteggio è più alto dell'ultimo in classifica
            int pos = length - 1;  //si memorizza la posizione in pos
            for (i = 0; i < length; i++) { //si scorre l'array
                if (punteggio > A[i].score ||(A[i].score == 0 && strcmp(A[i].nome, "") == 0)) { //se il punteggio è maggiore dello score in quella posizione o se si trova un nodo vuoto
                    pos = i;   //si salva la posizione raggiunta in pos
                    i = length;   // e si esce dal ciclo immediatamente
                   }
            }
            right_shift(pos);    //si esegue uno spostamento verso destra per fare spazio
            strcpy(A[pos].nome, player);   //e si inserisce il nuovo nodo in classifica
            A[pos].score = punteggio;
        }
    }
    bubble_sort();  //alla fine si riordina l'array
}
void classifica::scrivi_file() {  //funzione che scrive dati in un file.txt
    ofstream OutFile;
    OutFile.open("classifica.txt");  //si apre il file
    for (int i = 0; i < length; i++) {
        OutFile << A[i].nome<<": "<<A[i].score<<endl;  //si scrivono su file i nodi
    }
    OutFile.close();  //si chiude il file
}
void classifica::leggi_file() {   //funzione che legge da file i dati
    refresh();
    wrefresh(leader);   //esegue delle refresh della finestra per tenerla aggiornata
    ifstream InFile;
    InFile.open("classifica.txt");  //apre il file
    bool fine=false;
    if (!InFile.is_open()) {
        mvwprintw(leader, 0, 0, "Errore apertura file");  //se il file è aperto in modo non corretto si invia un messaggio di errore
        wrefresh(leader);  //aggiorno la finestra ed esco
        return;
    }
    for (int i = 0; i < length && !InFile.eof() && fine==false; i++) {  //oltre alle guardie classiche del for ci sono InFile.eof() che controlla se ci sono degli errori di lettura e il bool fine che indica la fine del file
        if (!InFile.getline(A[i].nome, maxchar, ':')) {  //se non legge esce dal ciclo perché significa che il file è terminato
            fine=true;
        }
        else {
            InFile.get();
            if (!InFile>>A[i].score) {  //stessa cosa
                fine=true;
            }
            else {
                InFile.ignore(1000, '\n');  //salta altri caratteri fino alla fine della riga
            }
        }
    }
    InFile.close();  //chiude il file
}
int classifica::stampa_file() {   //stampa la classifica
    noecho();
    nodelay(leader, false);   //aspetta l'input ESC
    wclear(leader);
    box(leader, 0, 0);  //crea il bordo della finestra
    keypad(leader, true);   //legge input da tastiera
    int class_y = start_y + 5;
    for (int i = 0; i < length && strlen(A[i].nome) != 0; i++) {
        mvwprintw(leader, class_y, (larg / 2) - 15, "%2d. %s: %d", i + 1, A[i].nome, A[i].score);  //stampa la classifica
        class_y=class_y+1;
    }
    wrefresh(leader);  //aggiorno la finestra
    int ch;
    while ((ch = wgetch(leader)) != 27) {  //non si esce dal ciclo finchè non si preme ESC
    }
    return 0;   //ritorna 0 poichè è utile per il menu
}
