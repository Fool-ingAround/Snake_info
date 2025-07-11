#include "menu.hpp"
#include <pdcurses.h>
#include "livelli.hpp"
#include <ctime>
#include "timer.hpp"
#include <cstring>
#include "classifica.hpp"
#include "Grid.hpp"
#include "movimento.hpp"

int menu::state;
int menu::difficulty;
bool menu::not_played[10];
int menu::level_scores[10];
char menu::player_name[21];
int menu::final_player_score;

void menu::colorsetup() {            // funzione da chiamare nel main subito dopo initscr();
    init_color(11, 470, 1000, 470);  // mela verde
    init_pair(11, 11, -1);

    init_color(12, 230, 770, 271);   // snake verde
    init_pair(12, 12, -1);

    init_color(13, 1000, 675, 349);  // snake logo arancione
    init_pair(13, 13, -1);

    init_color(21, 984, 965, 322); // vaporwave1
    init_pair(21, 21, -1);

    init_color(22, 906, 545, 408); // vaporwave2
    init_pair(22, 22, -1);

    init_color(23, 867, 341, 420); // vaporwave3
    init_pair(23, 23, -1);

    init_color(24, 831, 24, 510);  // vaporwave4
    init_pair(24, 24, -1);

    init_color(25, 98, 678, 682);  // vaporwave5
    init_pair(25, 25, -1);

    init_color(26, 156, 328, 525);   // vaporwave6
    init_pair(26, 26, -1);

    init_color(27, 988, 870, 592); // torsolo mela
    init_pair(27, 27, -1);

    init_color(28, 874, 141, 141); // colore player
    init_pair(28, 28, -1);

    init_color(29, 350, 350, 400); // colore livello non disponibile
    init_pair(29, 29, -1);

    init_color(72, 1000, 1000, 1000);

    init_color(100, 188, 278, 153);

    init_pair(99, 72, 100); // testo con sfondo verde

    init_pair(100, 12, 100);
    init_pair(101, 11, 100);
    init_pair(102, 21, 100);
    init_pair(103, 28, 100);

    int Level_colors[10][3] = { // gradiente da azzurro a rosso
        {98, 678, 682},  // azzurro
        {164, 574, 739},
        {251, 458, 755},
        {379, 348, 716},
        {511, 246, 642},
        {642, 155, 548},
        {759, 96, 426},
        {858, 69, 295},
        {942, 52, 158},
        {979,25,49}    // rosso acceso
    };

    for (int i = 0; i < 10; i++) {  // ciclo per evitare di inizializzre i colori del gradiente individualmente
        int color_id = 30 + i;
        init_color(color_id, Level_colors[i][0], Level_colors[i][1], Level_colors[i][2]);
        init_pair(30 + i, color_id, -1);
    }

}

menu::menu() {
    player_name[0] = '\0';
    final_player_score = 0;
    state = 0;
    difficulty = 1;
    not_played[10];
    for (int i = 0; i < 10; i++) {
        not_played[i] = true;
        level_scores[i] = 0;
    }
}

void menu::start_up() {

    initscr();

    if (has_colors()) {
        start_color();
        use_default_colors();
    }

    classifica::init();
    WINDOW* leaderboard = newwin(30, 100, 0, 0);
    box(leaderboard, 0, 0);
    wrefresh(leaderboard);
    menu::colorsetup();

    cbreak();
    keypad(stdscr, true);
    noecho();
    curs_set(0);


    while (state != 4) {
        switch (state) {
            case 0:
                player_name[0] = '\0';
                final_player_score = 0;
                for (int i = 0; i < 10; i++) {
                    not_played[i] = true;
                    level_scores[i] = 0;
                }
                state = menu::main_menu();
            break;
            case 1:
                classifica::leggi_file();
                state = classifica::stampa_file(leaderboard);
                classifica::scrivi_file();

                break;
            case 2:
                if (player_name[0] == '\0') {
                    final_player_score = 0;
                    if (menu::player_select(player_name) == 0) {
                        state = 0;
                        break;
                    }
                }

                difficulty = menu::level_select(player_name, not_played);
                if (difficulty == 0) {
                    state = 0;
                } else {
                    state = 3;
                }

            break;
            case 3:
                state = menu::new_game(difficulty);
            break;
        }
    }
    delwin(leaderboard);
    endwin();
}

/*
int menu::victory() {

}

int menu::game_over() {

}
*/


int menu::player_select(char* name) {
    int win_h = 30;         // altezza finestra
    int win_w = 100;        // larghezza finestra

    WINDOW* input_win = newwin(win_h, win_w, 0, 0);

    box(input_win, 0, 0);
    curs_set(1);
    nodelay(input_win, false);      // settato a true fa lampeggiare il cursore, da lasciare false
    noecho();

    int y_entername = win_h/6;      // coordinate per stampare in alto e al centro la scritta enter your name
    int x_entername = win_w/16;

    const char* entername[] = {
        "    ______      __                                                                     ",
        "   / ____/___  / /____  _____   __  ______  __  _______   ____  ____ _____ ___  ___ ",
        "  / __/ / __ \\/ __/ _ \\/ ___/  / / / / __ \\/ / / / ___/  / __ \\/ __ `/ __ `__ \\/ _ \\",
        " / /___/ / / / /_/  __/ /     / /_/ / /_/ / /_/ / /     / / / / /_/ / / / / / /  __/   ",
        "/_____/_/ /_/\\__/\\___/_/      \\__, /\\____/\\__,_/_/     /_/ /_/\\__,_/_/ /_/ /_/\\___/  ",
        "                             /____/                                                    "
    };

    for (int i = 0; i < 6; ++i) {       // ciclo per stampare ogni riga di entername[] con un colore diverso
        int pair_id = 0;                // variabile da settare al numero di coppia corrispondente ad ogni caso dello switch
        switch (i) {
            case 0: pair_id = 21;
            break;
            case 1: pair_id = 22;
            break;
            case 2: pair_id = 23;
            break;
            case 3: pair_id = 24;
            break;
            case 4: pair_id = 25;
            break;
            case 5: pair_id = 26;
            break;
            default: pair_id = 0;
            break;
        }
        if (pair_id) {
            wattron(input_win, COLOR_PAIR(pair_id));
        }
        mvwprintw(input_win, y_entername + i, x_entername, "%s", entername[i]);     // stampaggiamentazione dell'entername[]
        if (pair_id) {
            wattroff(input_win, COLOR_PAIR(pair_id));
        }
    }

    int y_offset = win_h/2 + win_h/5;       // coordinate della text bubble
    int x_offset = win_w/3 + win_w/25;
    const char* textbubble[] = {
        "  ,-----------------------------_",
        " /   Ssssurely you know what      >",
        "(  your name isss, at leassst... /",
        " \\       ... right?             |",
        "  `----------------------------`"
    };

    for (int i = 0; i < 5; i++) {
        mvwprintw(input_win, y_offset + i, x_offset, "%s", textbubble[i]);  // stampa la text bubble
    }

    const char* apple_with_snake[] = {
        "@   ,--./,-.",
        "S  /,-._.--~\\",
        "SSSS S}  {S",
        "   SSS-._,S>-,",
        "    `._,.._,'"
    };

    for (int i = 0; i < 5; i++) {               // ciclo per stampare la mela e il serpente con colori diversi. Il check si basa sul numero di rows (5)
        int len = strlen(apple_with_snake[i]);
        for (int j = 0; j < len; ++j) {         // secondo ciclo che scorre carattere per carattere ogni riga. Il check stavolta si basa sulla lunghezza della riga
            char ch = apple_with_snake[i][j];
            if (ch == 'S' || ch == '@' || ch == '>') {      // se il carattere incontrato è una "S" o una "@", è parte dello snake, quindi setta colore verde
                wattron(input_win, COLOR_PAIR(12));
            } else if (ch == '{' || ch == '}') {            // stesso concetto per il torsolo
                wattron(input_win, COLOR_PAIR(27)); // letteralmente due caratteri lol
            } else {                                        // se non è snake e non è torsolo, allora è la mela. Setta colore a rosso
                wattron(input_win, COLOR_PAIR(28));
            }
            mvwaddch(input_win, y_offset+1 + i, (x_offset + win_w/3 + win_w/25) + j, ch);   // siccome controlla carattere per carattere ho banalmente usato mvwaddch() invece di mvwprintw
            wattroff(input_win, COLOR_PAIR(28));
            wattroff(input_win, COLOR_PAIR(12));        //toglie attributi
            wattroff(input_win, COLOR_PAIR(27));
        }
    }

    int max_len = 20;       // lunghezza massima del nome
    int pos = 0;            // indice
    char buffer[21] = {};   // array di caratteri vuoto
    bool selecting_name = true;
    bool printable = true;
    while (selecting_name) {
        if (printable) {
            mvwprintw(input_win, win_h/2-1, win_w/2-1, "%-20s", buffer);    // inserisce la stringa buffer in quella posizione. Il "%-20s" riserva 20 caratteri di spazio, allineati a sinistra (-)
        }
        wmove(input_win, win_h/2-1, win_w/2-1 + pos);           // sposta il cursore di pos slot, effettivamente mostrando la posizione in cui verrà inserito il prossimo carattere del nome
        wrefresh(input_win);

        int ch = wgetch(input_win);     // prende un carattere alla volta in input. Se avessi usato mvwgetstr() non sarei riuscito ad implementare il caso in cui il player decida di uscire (ESC) dalla schermata di selezione del nome per tornare al menù principale (non necessario, ma più comodo)
        switch (ch) {
            case 27:  // ESC
                name[0] = '\0';
                noecho();
                curs_set(0);
                wclear(input_win);
                wrefresh(input_win);
                delwin(input_win);
                return 0;

            case 10:                    // ENTER
                if (buffer[0] == '\0') {
                    break;              // se premo invio quando il nome è vuoto esco dallo switch, non cambia nulla e il ciclo continua
                } else {
                    selecting_name = false;    // se premo invio quando il nome non è vuoto, esco dal while(selecting_name)
                    break;
                }

            case 8:                     // BACKSPACE
                if (pos > 0) {          // se mi trovo in indice magggiore di zero vuol dire che almeno un carattere è stato inserito. Se pos è zero semplicemente esco dallo switch
                    pos--;              // vado ad indice - 1 (l'indice in sé è "vuoto" e non ha alcun carattere, quindi premendo backspace devo rminuovere l'ultimo carattere inserito, che si trova ad indice - 1 nell'array buffer)
                    buffer[pos] = '\0'; // rimpiazzo indice - 1 con il carattere '\0'
                }
                break;

            default:
                if (ch >= 32 && ch <= 126 && pos < max_len) {   // check dei caratteri consentiti + check lungezza nome inserito che non deve essere maggiore di 20 caratteri
                    buffer[pos] = ch;                           // l'elemento dell'array in indice + 1 diventa il carattere inserito
                    pos++;                                      // indice + 1
                    buffer[pos] = '\0';                         // setto indice + 1 a '\0'
                    printable = true;                           // se è un carattere consentito, setto il booleano a true. Viene usato come guardia all'inizio del ciclo per mostrare a schermo il nome del player
                } else if ((ch < 32 || ch > 126)){
                    printable = false;                          // se non è un carattere consentito/il numero di caratteri inseriti supera il numero massimo di caratteri consentito, non è stampabile
                }
            break;
        }
    }

    strncpy(name, buffer, max_len + 1); // copio il nome salvato in buffer[] su name, che viene aggiornato. Uso il limite di 20 caratteri salvato in max_len + 1 slot per il carattere '\0'
    noecho();           // l'input dell'user non serve più
    curs_set(0);        // nascondo puntatore
    wclear(input_win);
    wrefresh(input_win);
    delwin(input_win);
    return 2;           // attiva il case 2 nello switch del main, chiama la funzione level select
}


int menu::new_game(int difficulty) {
    srand (time(nullptr));

    int seconds = 180;
    int matrix_h = 25;              // altezza griglia di gioco
    int matrix_w = 100;             // larghezza griglia di gioco
    int info_h = 6;                 // altezza finestra del punteggio/tempo
    int info_w = 100;               // larghezza finestra del punteggo/tempo
    WINDOW * info_win = newwin(info_h, info_w, 0, 0);
    WINDOW * game_win = newwin(matrix_h, matrix_w, info_h-1, 0);
    timer t = timer(seconds); // un timer viene istanziato tramite costruttore, parte il timer interno del... timer t
    snake s = snake(difficulty, game_win);
    Grid griglia = Grid(matrix_h, matrix_w, difficulty);
    griglia.UpdateGrid(game_win);
    wborder(info_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LTEE, ACS_RTEE);
        // boxes custom per unire le due finestre visivamente
    wborder(game_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);

    bool game = true;               // booleano per il while
    mvwprintw(info_win, info_h/2-1, info_w/2-1, "3:00");        // stampo il tempo massimo in maniera manuale aspettando l'input del player
    mvwprintw(info_win, info_h/2-1, info_w/20, "Score: %i", griglia.UpdateScore());
    mvwprintw(info_win, info_h/2, info_w/20, "%s's total score: %i", player_name, final_player_score);
    mvwprintw(info_win, info_h/2-1, info_w-info_w/7, "Level %i", difficulty);
    int y = (matrix_h / 2)-1;                                   // variabili di coordinate che verranno poi aggiornate nel ciclo di gioco
    int x = (matrix_w / 2)-1;
    wattron(game_win, COLOR_PAIR(99));
    mvwprintw(game_win, y, x-10, "Press any key to start!");    // messaggio temporaneo
    wattroff(game_win, COLOR_PAIR(99));
    wattron(game_win, COLOR_PAIR(100));
    mvwprintw(game_win, y+1, x-7, "ooooooooo@");
    wattroff(game_win, COLOR_PAIR(100));
    keypad(game_win, true);
    noecho();
    curs_set(0);
    wrefresh(game_win);                                         // aggiorno entrambe le finestre subito prima di wgetch();
    wrefresh(info_win);
    int waitforinput = wgetch(game_win);                        // player input
    werase(game_win);    // rimuovo il messaggio temporaneo
    griglia.UpdateGrid(game_win);
    wattron(game_win, COLOR_PAIR(100));
    mvwprintw(game_win, y+1, x-7, "ooooooooo@");
    wattroff(game_win, COLOR_PAIR(100));
    wborder(game_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);
    wrefresh(game_win);
    int score_in_level = 0;
    int verso = KEY_RIGHT;
    nodelay(game_win, true);                                    // serve per aggiornare correttamente il timer nel ciclo di gioco. Settandolo a false il timer si aggiornerebbe esclusivamente quando il player preme qualcosa
    while (!(t.time_out()) && (game)) {                         // uso due guardie perché distinguo due casi: fine partita per timeout o per collisione

        int input = wgetch(game_win);
        switch (input) {                                        // non fa parte del codice finale e rivisitato, tutto questo è un test
            case 'p': {
                nodelay(game_win, false);                       // se premo p si apre il menu di pausa, metto il nodelay a false per stoppare il timer e chiamare l'altra funzione
                t.pause_timer();
                griglia.pausetimers(t);
                int result = pause(game_win, info_win);         // il menu di pausa ritorna 573 se il player decide di riprendere la partita, 0 se il player vuole tornare al menu principale, 2 se il player vuole tornare alla schermata di selezione del livello
                t.resume_timer();
                nodelay(game_win, true);                        // setto nuovamente il nodelay a true
                if (result == 573) {
                    break;                                      // ritorna al game
                } else {
                    level_scores[difficulty-1] = griglia.UpdateScore();
                    final_player_score = final_player_score + level_scores[difficulty-1];
                    classifica::inserimento(player_name, final_player_score);
                    classifica::scrivi_file();
                    final_player_score = 0;
                    return result;
                }
            }
            case 27:                    // ESC
                wclear(game_win);
                wclear(info_win);
                wrefresh(game_win);
                wrefresh(info_win);
                game = false;
                return 0;               // menu principale
            default:
                verso = s.movements(verso, input);

                if (griglia.isendgame()) {
                    game = false;
                    break;
                }
                if (!griglia.isendgame()) {
                    griglia.Updatemtx(s);
                }
                t.display(info_win, info_h/2-1, info_w/2-1);        // chiamo la funzione della classe timer che date la finestra e le coordinate stampa appropriamente il tempo rimanente in minuti e secondi
                mvwprintw(info_win, info_h/2-1, info_w/20, "Score: %i", griglia.UpdateScore());
                mvwprintw(info_win, info_h/2, info_w/20, "%s's total score: %i", player_name, final_player_score + griglia.UpdateScore());
                griglia.UpdateGrid(game_win);
                break;                  // include anche il case ERR. Di base esce dallo switch

        }
        score_in_level = griglia.UpdateScore();
        wrefresh(game_win);
    }
    if (game) { // di base queste linee di codice vengono lette solo una volta che si esce dal while. Quindi o vengono lette dopo un timeout (vittoria) o dopo un gameover
        level_scores[difficulty-1] = score_in_level;
        final_player_score += level_scores[difficulty-1];
        classifica::inserimento(player_name, final_player_score);
        classifica::scrivi_file();
        Items::deleteallitems();
        wclear(game_win);
        wclear(info_win);
        wrefresh(info_win);
        wrefresh(game_win);
    } else if (!game) {
        score_in_level = 0;
        level_scores[difficulty-1] = score_in_level;
        final_player_score += level_scores[difficulty-1];
        classifica::inserimento(player_name, final_player_score);
        classifica::scrivi_file();
    }
    delwin(game_win);
    delwin(info_win);

    return 2;               // ritorna 2, che nel main richiama la funzione di level select. Da modificare, qualora il giocatore fa tutti i livelli questo dovrebbe ritornare un altro intero assegnato alla schermata (ancora da implementare) di vittoria
}

void draw_in_level_select(WINDOW* win, int height, int width) {     // per rendere un po' più pulito il tutto ho creato una funzione che stampa la roba grafica del menu di selezione livello
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_TTEE, ACS_LLCORNER, ACS_BTEE);

    int SL_y = height/7;
    int SL_x = width/10;
    const char* Selectlevel[] = {
        "   _____      __          __     __                   __",
        "  / ___/___  / /__  _____/ /_   / /   ___ _   _____  / /",
        "  \\__ \\/ _ \\/ / _ \\/ ___/ __/  / /   / _ \\ | / / _ \\/ / ",
        " ___/ /  __/ /  __/ /__/ /_   / /___/  __/ |/ /  __/ /  ",
        "/____/\\___/_/\\___/\\___/\\__/  /_____/\\___/|___/\\___/_/   "
    };

    for (int i = 0; i < 5; ++i) {       // a seconda della riga di Selectlevel[] in cui si trova stampa il testo in un colore diverso
        int pair_id = 0;
        switch (i) {
            case 0: pair_id = 21;
            break;
            case 1: pair_id = 22;
            break;
            case 2: pair_id = 23;
            break;
            case 3: pair_id = 24;
            break;
            case 4: pair_id = 25;
            break;
            default: pair_id = 0;
            break;
        }
        if (pair_id) {
            wattron(win, COLOR_PAIR(pair_id));
        }
        mvwprintw(win, SL_y + i, SL_x, "%s", Selectlevel[i]);
        if (pair_id) {
            wattroff(win, COLOR_PAIR(pair_id));
        }
    }

    int y_offset = (height/3)*2;
    int x_offset = width/7;
    const char* textbubble[] = {
        "       ,----------------------------_",
        "      /  The more applesss you eat,  \\",
        " @  <     the higher the sssscore!    )",              // altra roba ascii
        " S    \\    Try beating the record!   /",
        "SS     `----------------------------`",
        "S",
        "SS",
        " S",
        " S"
    };

    for (int i = 0; i < 9; i++) {
        int len = strlen(textbubble[i]);
        for (int j = 0; j < len; ++j) {
            char ch = textbubble[i][j];
            if (ch == 'S' || ch == '@' || ch == '>') {
                wattron(win, COLOR_PAIR(12));           //stampa di verde solo il serpente
            }
            mvwaddch(win, y_offset + i, x_offset + j, ch);
            wattroff(win, COLOR_PAIR(12));
        }
    }
}

int LivelliNonGiocati(bool levelarray[10]) {
    for (int i = 0; i < 10; i++) {
        if (levelarray[i]) {
            return 1;
        }
    }
    return 0;
}

int menu::level_select(char* player_name, bool* levelarray) {    // il menu level select prende come argomenti sia il nome del player inserito nel menu player select sia un array di booleani che indica se il livello in una certa posizione è già stato giocato
    int result = LivelliNonGiocati(not_played);
    if (result == 0) {
        plistb temp_level2 = livelli::crea_livelli();
        int sum = 0;
        for (int i = 0; i<10&& temp_level2 != nullptr; i++) {
            sum = sum + level_scores[temp_level2->val-1];
            temp_level2 = temp_level2->next;
        }
        final_player_score = sum;
        classifica::leggi_file();
        classifica::inserimento(player_name, final_player_score);
        classifica::scrivi_file();
        final_player_score = 0;
        return result;
    }

    curs_set(0);
    int height = 30;
    int width = 70;
    int start_x = width / 2;
    int start_y = height / 2;
    WINDOW * menu1 = newwin(height, width, 0, 0);
    WINDOW * info_win = newwin(height, width/2 - width/15, 0, width-1);
    plistb level = livelli::crea_livelli();
    plistb current_level = level;

    draw_in_level_select(menu1, height, width); // chiamo la funzione che gestisce la grafica della finestra della selezione del livello

    wborder(info_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_TTEE, ACS_URCORNER, ACS_BTEE, ACS_LRCORNER);

    mvwprintw(info_win, 2, 2, "Player: ");      // stampo in posizione standard che permette di visualizzare tutto il nome del player (anche nel caso il nome sia di 20 caratteri).
    wattron(info_win, COLOR_PAIR(28));
    mvwprintw(info_win, 2, 10, "%s", player_name);      // stampo il nome del player di rosso
    wattroff(info_win, COLOR_PAIR(28));

    const char* Level1[] = {
        "    ___",
        "   <  /",
        "   / /",
        "  / /",
        " /_/"
    };

    const char* Level2[] = {
        "    ___",
        "   |__ \\",
        "   __/ /",
        "  / __/",
        " /____/"
    };

    const char* Level3[] = {
        "    _____",
        "   |__  /",
        "    /_ <",
        "  ___/ /",
        " /____/"
    };

    const char* Level4[] = {
        "    __ __",
        "   / // /",
        "  / // /_",
        " /__  __/",
        "   /_/"
    };

    const char* Level5[] = {
        "    ______",
        "   / ____/",
        "  /___ \\",
        " ____/ /",
        "/_____/"
    };

    const char* Level6[] = {
        "    _____",
        "   / ___/",
        "  / __ \\",
        " / /_/ /",
        " \\____/"
    };

    const char* Level7[] = {
        "    _____",
        "   /__  /",
        "     / / ",
        "    / /",
        "   /_/"
    };

    const char* Level8[] = {
        "    ____",
        "   ( __ )",
        "  / __  |",
        " / /_/ /",
        " \\____/"
    };

    const char* Level9[] = {
        "    ____",
        "   / __ \\",
        "  / /_/ /",
        "  \\__, /",
        " /____/"
    };

    const char* Level10[] = {
        "    _______ ",
        "   <  / __ \\",
        "   / / / / /",
        "  / / /_/ / ",
        " /_/\\____/  "
    };

    int info_y = 6;

    int level_y = height/2-height/10;
    int level_x = width/2-width/12;


    /*
    mvwprintw(menu1, start_y-1, start_x-6, "Select level :P");      relics of the past
    mvwprintw(menu1, start_y+1, start_x, "1");
    */


    plistb temp_level = level;          // creo un puntatore a lista temporaneo solo per stampare il val

    for (int i = 0; i < 10 && temp_level != nullptr; i++) {
        int y = info_y + i * 2;         // a ogni iterazione si sposta di due righe verso il basso
        mvwprintw(info_win, y, 2, "Level %d:  ", temp_level->val);      // stampa la stringa Level: con il valore del livello
        wmove(info_win, y, 12);         // si muove nella finestra di info per "pulire" il testo stampato in precedenza
        wclrtoeol(info_win);            // funzione che pulisce il testo in questione
        if (levelarray[temp_level->val - 1]) {      // se nell'array di booleani passato come argomento il livello (-1, perché gli array partono da indice zero e i nostri livelli da valore 1) di interesse è true vuol dire che non è ancora stato giocato, di conseguenza passa la guardia dell'if
            mvwprintw(info_win, y, 12, "no score"); // printa "no score", dato che il livello non è ancora stato giocato
        } else {
            mvwprintw(info_win, y, 12, "%i", level_scores[temp_level->val-1]);       // triviale dai
        }
        temp_level = temp_level->next;      // scorre la lista
    }


    plistb temp_level2 = level;
    int sum = 0;
    for (int i = 0; i<10&& temp_level2 != nullptr; i++) {
        sum = sum + level_scores[temp_level2->val-1];
        temp_level2 = temp_level2->next;
    }
    final_player_score = sum;
    mvwprintw(info_win, height-3, 2, "Total score: %i", final_player_score);
    wrefresh(menu1);    // refresh prima del ciclo
    wrefresh(info_win);
    noecho();
    nodelay(menu1, true);
    keypad(menu1, true);
    bool selecting = true; // booleano per il while
    while (selecting) {
        int ch = wgetch(menu1);
        switch (ch) {
            case KEY_LEFT:
                if (current_level->prev != nullptr) {       // se il player preme freccia sinistra e il prev* non è nullo, eseguo le successive tre linee di codice, altrimenti esce direttamente dallo switch
                    plistb tmp = current_level;
                    current_level = current_level->prev;    // faccio uno scambio di posizione verso "sinistra" nella lista
                    current_level->next = tmp;
                } break;
            case KEY_RIGHT:
                if (current_level->next != nullptr) {       // stesso concetto per la freccia destra
                    plistb tmp = current_level;
                    current_level = current_level->next;
                    current_level->prev = tmp;
                } break;
            case 10:                    // ENTER
                if (!levelarray[current_level->val-1]) {        // se premo enter e quel livello risulta già giocato nell'array di booleani esco dallo switch e non succede niente
                    break;
                }
                else {                                          // se premo enter e il livello non è ancora stato giocato rimuovo le due finestre e ritorno il valore del livello, che verrà utilizzato dal menu new_game come int di difficoltà
                    wclear(menu1);
                    wrefresh(menu1);
                    delwin(menu1);
                    levelarray[current_level->val-1] = false;
                    return current_level->val;
                }
            case 27:                    // ESC
                wclear(menu1);
                wrefresh(menu1);
                delwin(menu1);
                return 0;               // main menu
            default:                    // include ERR del nodelay()

                break;                  // non fa nulla
        };

        switch (current_level->val) {           // switch lunghissimo immotivato che stampa a schermo il numero del livello in ascii art basandosi sul val del livello corrente
            case 1:
                werase(menu1);                  // elimina contenuti della finestra menu1 senza eliminare bordi
                draw_in_level_select(menu1, height, width);     // richiamo la funzione che stampa l'ascii art di level select
                if (levelarray[current_level->val-1]){          // se il livello in questione non è ancora stato giocato stampa l'ascii art col colore predefinito
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(30));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level1[i]);
                        wattroff(menu1, COLOR_PAIR(30));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {               // altrimenti lo stampa in grigio
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level1[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 2:                                             // è un copia e incolla, nulla di nuovo
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]){
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(31));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level2[i]);
                        wattroff(menu1, COLOR_PAIR(31));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level2[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
                        case 3:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(32));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level3[i]);
                        wattroff(menu1, COLOR_PAIR(32));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level3[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 4:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(33));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level4[i]);
                        wattroff(menu1, COLOR_PAIR(33));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level4[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 5:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(34));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level5[i]);
                        wattroff(menu1, COLOR_PAIR(34));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level5[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 6:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(35));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level6[i]);
                        wattroff(menu1, COLOR_PAIR(35));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level6[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 7:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(36));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level7[i]);
                        wattroff(menu1, COLOR_PAIR(36));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level7[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 8:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(37));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level8[i]);
                        wattroff(menu1, COLOR_PAIR(37));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level8[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 9:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(38));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level9[i]);
                        wattroff(menu1, COLOR_PAIR(38));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level9[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 10:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]) {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(39));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level10[i]);
                        wattroff(menu1, COLOR_PAIR(39));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level10[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;

            default:
            break;
        }

        /*
        mvwprintw(menu1, start_y+1, start_x, "%d", livelli::get_val(current_level));        more relics
        */

        wborder(menu1, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_TTEE, ACS_LLCORNER, ACS_BTEE);

        wborder(info_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_TTEE, ACS_URCORNER, ACS_BTEE, ACS_LRCORNER);

        wrefresh(menu1);
        wrefresh(info_win);
    }
    delwin(menu1);
    return 0;       // di base ritorna al main menu, anche se questo è codice morto
}

int menu::main_menu() {
    bool playing = true;
    int height = 30;
    int width = 100;


    WINDOW * win = newwin(height, width, 0, 0);

    keypad(win, true);
    curs_set(0);

    int yNG = (height/3)*2 - height/15;
    int xNG = (width/2) - width/20;

    mvwprintw(win, yNG, xNG, "New game");
    mvwprintw(win, yNG+4, xNG, "Leaderboard");
    wattron(win, COLOR_PAIR(13));
    mvwprintw(win, height-3, (width/4)*3, "Press ESC to quit! :)");
    wattroff(win, COLOR_PAIR(13));

    const char *logo[] = {
        "        .d8888b.                                               888               ",
        "      d88P  Y88b                                              888               ",
        "     Y88b.                                                   888               ",
        "     \"Y888b.   .d8888b  .d8888b  .d8888b  88888b.   8888b.  888  888  .d88b.  ",
        "       \"Y88b. 88K      88K      88K      888 \"88b     \"88b 888 .88P d8P  Y8b ",
        "        \"888 \"Y8888b. \"Y8888b. \"Y8888b. 888  888 .d888888 888888K  88888888 ",
        " Y88b  d88P      X88      X88      X88 888  888 888  888 888 \"88b Y8b.     ",
        " \"Y8888P\"   88888P'  88888P'  88888P' 888  888 \"Y888888 888  888  \"Y8888  "
    };

    int start_y = 3;
    int start_x = width / 12;

    wattron(win, COLOR_PAIR(13));
    for (int i = 0; i < 8; i++) {
        mvwprintw(win, start_y + i, start_x, "%s", logo[i]);            // stampa il logo
    }
    wattroff(win, COLOR_PAIR(13));

    int y_offset = height/3 + height/15;
    int x_offset = 16;
    const char* ascii_apple[] = {
        "     ,_        ",
        " ,--./~'_     @",
        "|  `-- # )  SSS",
        " \\      /SSSS",
        "  `-^--'"
    };

    int x_txtbubble = 34;
    const char* text_bubble[] = {
        "  ,------------------------_",
        "<   Made with ncursssesss!  )",
        "  `------------------------`"
    };

    for (int i = 0; i < 3; ++i) {
        mvwprintw(win, y_offset + i, x_txtbubble, "%s", text_bubble[i]); // stampa solo la scritta
    }


    for (int i = 0; i < 5; ++i) {               // stampa la mela e snake di colori diversi
        int len = strlen(ascii_apple[i]);
        for (int j = 0; j < len; ++j) {
            char ch = ascii_apple[i][j];
            if (ch == 'S' || ch == '@') {
                wattron(win, COLOR_PAIR(12));
            } else {
                wattron(win, COLOR_PAIR(28));
            }
            mvwaddch(win, y_offset + i, x_offset + j, ch);
            wattroff(win, COLOR_PAIR(28));
            wattroff(win, COLOR_PAIR(12));
        }
    }

    wrefresh(win);

    wmove(win, yNG, xNG-2);
    wprintw(win, ">");          // sposto il cursore due slot a sinistra della scrita new game e stampo ">"
    noecho();
    wrefresh(win);
    bool selecting_newgame = true;      // booleano che per semplicità mi dice quale menu sto aprendo
    while (playing) {
        box(win, 0, 0);

        int ch = wgetch(win);

        switch (ch) {
            case KEY_UP:
                mvwprintw(win, yNG+4, xNG-2, " ");
                wmove(win, yNG, xNG-2);
                wprintw(win, ">");
                selecting_newgame = true;
            break;
            case KEY_DOWN:
                mvwprintw(win, yNG, xNG-2, " ");
                wmove(win,yNG+4, xNG-2);
                wprintw(win, ">");
                selecting_newgame = false;
            break;
            case 10:
                playing = false;
                if(selecting_newgame) {
                    wclear(win);
                    delwin(win);
                    return 2;
                } else {
                    wclear(win);
                    delwin(win);
                    return 1;
                }
            case 27:
                delwin(win);
                return 4;
            default: break;
        }
        wrefresh(win);
    }
    wrefresh(win);      // codice morto
    delwin(win);
    return 4;
}

int menu::pause(WINDOW * game_win, WINDOW * info_win) {

    int pmenu_h = 15;
    int pmenu_w = 50;

    WINDOW * pmenu = newwin(pmenu_h, pmenu_w, pmenu_h/2+pmenu_h/4, pmenu_w/2); // stampo il menu di pausa al centro dello schermo

    box(pmenu, 0, 0);

    int y_options = pmenu_h/3;
    int x_options = pmenu_w/2-3;

    mvwprintw(pmenu, y_options, x_options, "Resume :D");
    mvwprintw(pmenu, y_options+2, x_options, "Quit level ;(");          // stampo le tre opzioni
    mvwprintw(pmenu, y_options+4, x_options, "Main menu :'(");

    int cursor_x = x_options-2;

    mvwprintw(pmenu, y_options, cursor_x, ">");
    int selection = 1;
    wrefresh(pmenu);
    keypad(pmenu, true);
    noecho();
    nodelay(pmenu, false);
    bool paused = true;
    while (paused) {
        int ch = wgetch(pmenu);
        switch (ch) {           // questo switch aggiorna selection
            case KEY_UP:
                if (selection == 1) {
                    break;
                } else {
                    selection--;
                    y_options = y_options - 2;
                    break;
                }
            case KEY_DOWN:
                if (selection == 3) {
                    break;
                } else {
                    selection++;
                    y_options = y_options + 2;
                    break;
                }
            case 27:        // ESC
                wclear(pmenu);
                wrefresh(pmenu);
                delwin(pmenu);
                paused = false;
                return 573;     // premere esc è come scegliere l'opzione resume
            case 10:
                switch (selection) {
                    case 1:         // resume
                        paused = false;
                        return 573;     // torna al gioco
                    case 2:             // quit level
                        wclear(pmenu);
                        wclear(game_win);
                        wclear(info_win);
                        wrefresh(pmenu);
                        wrefresh(game_win);
                        wrefresh(info_win);

                        // aggiungere funzione che salva lo score fatto fino ad ora

                        return 2; // torna a level select
                    case 3:
                        wclear(pmenu);
                        wclear(game_win);
                        wclear(info_win);
                        wrefresh(pmenu);
                        wrefresh(game_win);
                        wrefresh(info_win);

                        return 0; // torna al main menu
                    default:

                        break;
                }
            break;
        }
        switch (selection) {        // questo switch serve solo a stampare correttamente il cursore ">"
            case 1:
                mvwprintw(pmenu, y_options, cursor_x, ">");
                mvwprintw(pmenu, y_options+2, cursor_x, " ");
                mvwprintw(pmenu, y_options+4, cursor_x, " ");
                break;
            case 2:
                mvwprintw(pmenu, y_options-2, cursor_x, " ");
                mvwprintw(pmenu, y_options, cursor_x, ">");
                mvwprintw(pmenu, y_options+2, cursor_x, " ");
                break;
            case 3:
                mvwprintw(pmenu, y_options-4, cursor_x, " ");
                mvwprintw(pmenu, y_options-2, cursor_x, " ");
                mvwprintw(pmenu, y_options, cursor_x, ">");
                break;
        }
        wrefresh(pmenu);
    }
    delwin(pmenu);
    return 0;
}