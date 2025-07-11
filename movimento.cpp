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
    for (i=0; i<altezza; i++) {
        for (j=0; j<larghezza; j++) {
            matrix[i][j]=false;    //la matrice booleana viene inizializzata tutta a false cioè è vuota
        }
    }
    int n;
    head=nullptr;
    for (n=lserp-1; n>0; n--) {
        head=crea_corpo(head, larghezza/2-n, altezza/2, 'S');    //si costruisce il corpo del serpente attraverso la funzione crea_corpo che inserisce i pezzi in testa
        matrix[altezza/2][larghezza/2-n]=true;   //i punti in cui si trova il serpente vengono marcati con true
    }   //di default si è deciso di creare il serpente con la testa al centro della matrice che guarda verso destra
    head=crea_corpo(head, larghezza/2-n, altezza/2, '@');    //si differenzia la testa dal corpo
    matrix[altezza/2][larghezza/2-n]=true;
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
    wrefresh(win);
}
void snake::UP() {
    dequeue();
    head->corpo = 'o';
    int new_y = (head->y - 1 == 0) ? altezza - 2 : head->y - 1;
    int new_x = head->x;
    head = crea_corpo(head, new_x, new_y, '@');
    matrix[new_y][new_x] = true;
}

void snake::DOWN() {
    dequeue();
    head->corpo = 'o';
    int new_y = (head->y + 1 == altezza - 1) ? 1 : head->y + 1;
    int new_x = head->x;
    head = crea_corpo(head, new_x, new_y, '@');
    matrix[new_y][new_x] = true;
}

void snake::LEFT() {
    dequeue();
    head->corpo = 'o';
    int new_x = (head->x - 1 == 0) ? larghezza - 2 : head->x - 1;
    int new_y = head->y;
    head = crea_corpo(head, new_x, new_y, '@');
    matrix[new_y][new_x] = true;
}

void snake::RIGHT() {
    dequeue();
    head->corpo = 'o';
    int new_x = (head->x + 1 == larghezza - 1) ? 1 : head->x + 1;
    int new_y = head->y;
    head = crea_corpo(head, new_x, new_y, '@');
    matrix[new_y][new_x] = true;
}

double snake::speed(int difficulty) {
    return tempo/(double(difficulty));   //funzione che calcola la velocità di movimento dello snake
}
bool snake::inversione(int a, int b) {   //controlla se accade un'inversione cioè si decide di cambiare la direzione del serpente in maniera opposta alla direzione in cui stava procedendo
    return (a==KEY_UP && b==KEY_DOWN)|| (a==KEY_LEFT && b==KEY_RIGHT) || (a==KEY_DOWN && b==KEY_UP) || (a==KEY_RIGHT && b==KEY_LEFT);
}
int snake::movements(int last, int input) {
    nodelay(win, true);
    int ultima_azione = last;
    int prossimo_verso = last;
    if (input != ERR &&
        (input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT) &&    //controlla che non vengano inseriti input inutli come inversioni, 
        !inversione(input, last) && input != last) {                                           //input che non riguardano le frecce e azioni che indicano la stessa 
        prossimo_verso = input;                                                               //direzione in cui il serpente sta già andando
        }       //salvo la prossima azione
    ultima_azione = prossimo_verso;   
    switch (ultima_azione) {
        case KEY_UP: UP(); break;
        case KEY_DOWN: DOWN(); break;
        case KEY_LEFT: LEFT(); break;
        case KEY_RIGHT: RIGHT(); break;
    }
    napms(speed(difficulty));
    return ultima_azione;
}

pos snake::get_head() {   //funzione che restituisce solo la posizione della testa del serpente
    pos tmp;
    tmp.i = head->y;
    tmp.j = head->x;
    return tmp;
}

bool snake::isoccupied(int y, int x) {    //restituisce un bool relativo alla casella: true se occupata, false altrimenti
    return matrix[y][x];
}

