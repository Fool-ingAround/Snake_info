#include "classifica.hpp"

nodo classifica::A[length];

classifica::classifica() {}

void classifica::init() {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    leggi_file();
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
static bool normalizza_nome(const char* src, char* dst, std::size_t dstLen)
{
    bool qualcosa_di_stampabile = false;

    if (dstLen == 0) return false;
    dstLen--;                      // Riservo spazio per '\0'

    std::size_t i = 0;
    for (; src[i] != '\0' && i < dstLen; ++i)
    {
        unsigned char c = static_cast<unsigned char>(src[i]);
        if (c >= 32 && c <= 126)           // range ASCII “stampabile” per PDCurses
        {
            dst[i] = c;
            qualcosa_di_stampabile = true;
        }
        else
        {
            dst[i] = '?';                  // sostituisco con placeholder visibile
        }
    }
    dst[i] = '\0';
    return qualcosa_di_stampabile;
}

void classifica::inserimento(const char* player_in, int punteggio)
{   if (!player_in || strlen(player_in) == 0) return;

    // --- 1.  Normalizza / copia il nome in un buffer locale ---
    char nome[length];
    if (!normalizza_nome(player_in, nome, sizeof(nome)))
        return;                               // nome vuoto o totalmente “sporco”: ignoro

    // --- 2.  Aggiorna la classifica in memoria (prima la carico dal file) --
    leggi_file();     // Mantieni se vuoi ricaricare sempre la versione persistente
    bubble_sort();

    // a) C’è già il giocatore? Aggiorno se serve
    for (int i = 0; i < length; ++i)
    {
        if (strcmp(A[i].nome, nome) == 0)
        {
            if (punteggio > A[i].score)
                A[i].score = punteggio;
            bubble_sort();
            scrivi_file();
            return;
        }
    }

    // b) Giocatore nuovo: trovo posizione di inserimento
    if (!isempty() && punteggio <= A[length - 1].score)
        return;                              // non entra in classifica

    int pos = length - 1;
    for (int i = 0; i < length; ++i)
    {
        if (punteggio > A[i].score ||
            (A[i].score == 0 && A[i].nome[0] == '\0'))
        {
            pos = i;
            break;
        }
    }

    right_shift(pos);

    // Copia sicura con terminazione
    strncpy(A[pos].nome, nome, sizeof(A[pos].nome) - 1);
    A[pos].nome[sizeof(A[pos].nome) - 1] = '\0';
    A[pos].score = punteggio;

    bubble_sort();
    scrivi_file();
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
            // 🔽 Pulizia del nome da caratteri non stampabili (tipo \r o altro)
            for (char* p = A[i].nome; *p; ++p) {
                if (static_cast<unsigned char>(*p) < 32 || static_cast<unsigned char>(*p) > 126)
                    *p = '\0';
            }

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

void print_safe(WINDOW* win, int y, int x, const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        // Stampa ogni char ma convertito a unsigned char e senza attributi strani
        mvwaddch(win, y, x + i, (unsigned char)str[i] & A_CHARTEXT);
        ++i;
    }
}

int classifica::stampa_file(WINDOW* leader) {
    werase(leader);
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
        print_safe(leader, y_ldrbrd + i, x_ldrbrd, ldrbrd[i]);
        wattroff(leader, COLOR_PAIR(pair_id));
    }

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
    char buffer[64];

    if (validCount >= 1) {
        snprintf(buffer, sizeof(buffer), "1. %s: %d", validi[0].nome, validi[0].score);
        print_safe(leader, podium_y, center_x - 10, buffer);
    }
    if (validCount >= 2) {
        snprintf(buffer, sizeof(buffer), "2. %s: %d", validi[1].nome, validi[1].score);
        print_safe(leader, podium_y + 2, center_x - 30, buffer);
    }
    if (validCount >= 3) {
        snprintf(buffer, sizeof(buffer), "3. %s: %d", validi[2].nome, validi[2].score);
        print_safe(leader, podium_y + 2, center_x + 10, buffer);
    }

    // ---------- COLONNE VERTICALI ----------
    int start_y = podium_y + 6;
    int col_width = larg / 3;

    int col_x[3] = { 2, col_width + 2, 2 * col_width + 2 };
    int col_y[3] = { start_y, start_y, start_y };

    for (int i = 3; i < validCount && i < length; ++i) {
        int pos = i + 1;
        int col = (pos - 4) / 7; // colonne: 0 (4–10), 1 (11–17), 2 (18–23)
        if (col >= 3) break;

        snprintf(buffer, sizeof(buffer), "%2d. %-20s %4d", pos, validi[i].nome, validi[i].score);
        print_safe(leader, col_y[col], col_x[col], buffer);
        col_y[col] += 2;
    }

    wrefresh(leader);

    int ch;
    while ((ch = wgetch(leader)) != 27) {}

    return 0;
}
