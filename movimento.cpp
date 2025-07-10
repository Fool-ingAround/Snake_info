#include "movimento.hpp"
int lserp=10;  //lunghezza del serpente
plista snake::crea_corpo(plista head, int ax, int ord, char c) {     //funzione necessaria per creare lo snake e aggiugergli parti del coropo in testa
    plista tmp= new lista;
    tmp->x=ax;
    tmp->y=ord;
    tmp->corpo=c;
    tmp->next=head;
    return tmp;
}
snake::snake(int diff, WINDOW * w) {   //il costruttore ha come parametri la difficoltà del livello e la finestra di gioco ovvero la griglia
    difficulty=diff;
    win=w;
    int i=0;
    int j=0;
    for (i=0; i<alt; i++) {
        for (j=0; j<larg; j++) {
            matrix[i][j]=false;    //la matrice booleana viene inizializzata tutta a false cioè è vuota
        }
    }
    int n;
    head=nullptr;
    for (n=lserp-1; n>0; n--) {
        head=crea_corpo(head, larg/2-n, alt/2, 'o');    //si costruisce il corpo del serpente attraverso la funzione crea_corpo che inserisce i pezzi in testa
        matrix[alt/2][larg/2-n]=true;   //i punti in cui si trova il serpente vengono marcati con true
    }   //di default si è deciso di creare il serpente con la testa al centro della matrice che guarda verso destra
    head=crea_corpo(head, larg/2-n, alt/2, '@');    //si differenzia la testa dal corpo
    matrix[alt/2][larg/2-n]=true;
}
void snake::dequeue() {    //funzione necessaria per gestire lo spostamento del serpente: l'ultimo elemento in coda viene eliminato e ne viene aggiunto uno in testa
    if (head==nullptr) {
        return;
    }
    if (head->next==nullptr) {
        matrix[head->y][head->x]=false;
        delete head;
        head=nullptr;
        return;
    }
    plista p=head;
    while (p->next->next!=nullptr) {   //scorre tutta la lista fermandosi al penultimo elemento
        p=p->next;
    }
    plista coda=p->next;
    matrix[coda->y][coda->x]=false;      //la posizione dell'ultimo elemento in coda viene marcata come false
    delete coda;                         // e alla fine viene eliminato l'ultimo elemento
    p->next=nullptr;
}
void snake::UP() {   //funzione dedicata allo spostamento verso l'alto del serpente
    dequeue();   //esegue la funzione dequeue
    head->corpo='o';       //l'elemento in testa viene sovrascritto con il carattere 'o' dato che diventerà il secondo pezzo del corpo dello snake
    if ((head->y-1)==0) {   //controlla la testa del serpente da inserire superi il bordo della griglia di gioco. In questo caso setta la y a 1 in modo da far spuntare il serpente dal basso, avendo un effetto pacman
        head->y=alt-2;
    }
    head=crea_corpo(head, head->x, head->y-1, '@');  //viene inserita la testa
    matrix[head->y][head->x]=true;   //si marca la posizione con true
}
void snake::DOWN() {  //funzione dedicata allo spostamento verso il basso
    dequeue();  //esegue la funzione dequeue
    head->corpo='o';   //stessa cosa della funzione UP
    if ((head->y+1)==alt-1) {   //stessa cosa di UP, ma aggiorna la y in modo da far spuntare lo snake dal lato alto della griglia
        head->y=1;
    }
    head=crea_corpo(head, head->x, head->y+1, '@');  //stesso funzionamento di UP
    matrix[head->y][head->x]=true;
}
void snake::LEFT() {   //funzione dedicata allo spostamento verso sinistra
    dequeue();
    head->corpo='o';
    if ((head->x-1)==0) {   //stesso funzionamento delle altre solo che in questo caso, se la condizione è vera, aggiorna la x in modo da far spuntare il serpente dal lato destro
        head->x=larg-2;
    }
    head=crea_corpo(head, head->x-1, head->y, '@');
    matrix[head->y][head->x]=true;
}
void snake::RIGHT() { //funzione dedicata allo spostamento verso destra
    dequeue();
    head->corpo='o';
    if ((head->x+1)==larg-1) {   //stesso funzionamento delle altre solo che in questo caso, se la condizione è vera, aggiorna la x in modo da far spuntare il serpente dal lato sinistro
        head->x=1;
    }
    head=crea_corpo(head, head->x+1, head->y, '@');
    matrix[head->y][head->x]=true;
}
double snake::speed(int difficulty) {
    return tempo/(double(difficulty));   //funzione che calcola la velocità di movimento dello snake
}
bool snake::inversione(int a, int b) {   //controlla se accade un'inversione cioè si decide di cambiare la direzione del serpente in maniera opposta alla direzione in cui stava procedendo
    return (a==KEY_UP && b==KEY_DOWN)|| (a==KEY_LEFT && b==KEY_RIGHT) || (a==KEY_DOWN && b==KEY_UP) || (a==KEY_RIGHT && b==KEY_LEFT);
}
void snake::movements() {   //funzione che gestisce tutto il movimento del serpente
    keypad(win, true);   //in questo modo si posso eseguire input da tastiera
    wtimeout(win, 0);   //la finestra legge immediatamente gli input in questo modo wgetch non è bloccante, quindi il movimento è continuo
    noecho();   //non vengono visualizzati caratteri digitati per sbaglio
    int verso= KEY_RIGHT;  //si è deciso di default di far muovere il serpente all'inizio verso destra
    int prossimo_verso=verso;   //si salva la prossima direzione del serpente
    int azione;  //deciderà a direzione dello snake a seconda dell'input
    int ultima_azione=ERR;    //non c'è un'ultima azione adesso perchè è appena iniziato il gioco
    while ((azione=wgetch(win))!=ERR){   //il ciclo continua finchè mettiamo input diversi da ERR
        ultima_azione=azione;   //si memorizza l'ultima azione
        if (ultima_azione!=ERR && (ultima_azione==KEY_UP || ultima_azione==KEY_DOWN || ultima_azione==KEY_LEFT || ultima_azione==KEY_RIGHT) && !inversione(ultima_azione, verso) && ultima_azione != verso) {
            prossimo_verso=ultima_azione;  //l'if controlla che l'ultima azione non abbia ricevuto in input ERR e che abbia ricevuto un input 1 dei quattro input consentiti. Inoltre controlla che non ci siano inversioni e che l'ultima azione non sia uguale al verso in cui si sta muovendo il serpente.
        } //se tutto ciò è vero allora viene salvata l'ultima azione nel prossimo verso. In questo modo si escludono input inutili che non producono cambiamenti nel movimento dello snake
    }
    verso=prossimo_verso;  //usciti dal while si salva nel verso la prossima direzione e viene eseguito uno switch su verso
    switch (verso) {
        case KEY_UP:     //in caso di input SU si richiama UP
            UP();
            break;
        case KEY_DOWN:  //in caso di input GIÙ si richiama DOWN
            DOWN();
            break;
        case KEY_LEFT:    //in caso di input SINISTRA si richiama LEFT
            LEFT();
            break;
        case KEY_RIGHT:
            RIGHT();    //in caso di input DESTRA si richiama RIGHT
            break;
    }
    napms(speed(difficulty));     //stabilisce la velocità di movimento del serpente in ms. Più la divisione della funzione speed darà n numero piccolo più si muoverà veloce il serpente, aumentando quindi la difficoltà
}
void snake::get_head(bool matrixtemp[alt][larg]) {   //funzione che restituisce solo la posizione della testa del serpente
    for(int i=0; i<alt; i++) {
        for(int j=0; j<larg; j++) {
            if (i==head->y && j==head->x) {     //se si è arrivati alle coordinate della testa allora si imposta true
                matrixtemp[i][j] = true;
            }
            else {
                matrixtemp[i][j] = false;    //tutto il resto è false
            }
        }
    }
}
void snake::get_matrix(bool matrixtemp[alt][larg]) {   //restituisce la matrice aggiornata
    for(int i=0;i<alt;i++) {
        for(int j=0;j<larg;j++) {
            matrixtemp[i][j]=matrix[i][j];
        }
    }
}


