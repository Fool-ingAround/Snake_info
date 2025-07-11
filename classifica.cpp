#include "classifica.hpp"

nodo classifica::A[length];

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
void classifica::inserimento(char * player, int punteggio) {   //prende in input il nome del giocatore e il suo punteggio
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
        OutFile << A[i].nome<<":"<<A[i].score<<endl;  //si scrivono su file i nodi
    }
    OutFile.close();  //si chiude il file
}
void classifica::leggi_file() {
    ifstream InFile("classifica.txt");
    bool fine = false;

    int i = 0;

    while (!fine && i < length) {
        // Legge il nome fino ai due punti
        if (!InFile.getline(A[i].nome, maxchar, ':')) {
            fine = true;
        } else {
            if (!(InFile >> A[i].score)) {
                fine = true;
            } else {
                InFile.ignore(1000, '\n');  // ignora fino a fine riga
                i++;
            }
        }
    }
    bubble_sort();
    InFile.close();
}

int classifica::stampa_file(WINDOW* leader) {

    noecho();
    nodelay(leader, false);
    box(leader, 0, 0);
    keypad(leader, true);

    int y_ldrbrd = alt / 15 - 1;
    int x_ldrbrd = larg / 7;

    const char* ldrbrd[] = {
        "    __                   __          __                         __",
        "   / /   ___  ____ _____/ /__  _____/ /_  ____  ____ __________/ /",
        "  / /   / _ \\/ __ `/ __  / _ \\/ ___/ __ \\/ __ \\/ __ `/ ___/ __  / ",
        " / /___/  __/ /_/ / /_/ /  __/ /  / /_/ / /_/ / /_/ / /  / /_/ /  ",
        "/_____/\\___/\\__,_/\\__,_/\\___/_/  /_.___/\\____/\\__,_/_/   \\__,_/   "
    };

    for (int i = 0; i < 5; ++i) {
        int pair_id = 21 + i;
        wattron(leader, COLOR_PAIR(pair_id));
        mvwprintw(leader, y_ldrbrd + i, x_ldrbrd, "%s", ldrbrd[i]);
        wattroff(leader, COLOR_PAIR(pair_id));
    }

    leggi_file();
    scrivi_file();

    // Filtra solo giocatori validi (nome non vuoto e score > 0)
    nodo validi[length];
    int validCount = 0;
    for (int i = 0; i < length && validCount < length; ++i) {
        if (strlen(A[i].nome) > 0 && A[i].score > 0) {
            validi[validCount++] = A[i];
        }
    }

    // ---------- PODIO ----------
    int podium_y = y_ldrbrd + 7;
    int center_x = larg / 2;

    if (validCount >= 1)
        mvwprintw(leader, podium_y, center_x - 10, "1. %s: %d", validi[0].nome, validi[0].score);
    if (validCount >= 2)
        mvwprintw(leader, podium_y + 2, center_x - 30, "2. %s: %d", validi[1].nome, validi[1].score);
    if (validCount >= 3)
        mvwprintw(leader, podium_y + 2, center_x + 10, "3. %s: %d", validi[2].nome, validi[2].score);

    // ---------- COLONNE VERTICALI ----------
    int start_y = podium_y + 6;
    int col_width = larg / 3;

    int col_x[3] = { 2, col_width + 2, 2 * col_width + 2 };
    int col_y[3] = { start_y, start_y, start_y };

    for (int i = 3; i < validCount && i < length; ++i) {
        int pos = i + 1;
        int col = (pos - 4) / 7; // colonne: 0 (pos 4–10), 1 (11–17), 2 (18–23)
        if (col >= 3) break;

        mvwprintw(leader, col_y[col], col_x[col], "%2d. %-20s %4d", pos, validi[i].nome, validi[i].score);
        col_y[col] += 2;  // Aggiungi 2 per una riga vuota tra ciascun nome
    }

    wrefresh(leader);

    int ch;
    while ((ch = wgetch(leader)) != 27) {}

    return 0;   //ritorna 0 quando esce dal while ovvero quando si preme ESC
}
