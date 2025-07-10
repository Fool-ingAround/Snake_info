#include "timer.hpp"
#include <pdcurses.h>

timer::timer(int seconds) {     // costruttore
    this->seconds = seconds;
    start = time(nullptr);
    paused_seconds = 0;
    is_paused = false;
}

void timer::pause_timer() {     // mette in pausa il timer
    if (!is_paused) {           // se non è già in pausa
        pause = time(nullptr);          // inizializza il time_t pausa
        is_paused = true;               // booleano a true
    }
}

void timer::resume_timer() {    // fa ripartire il timer
    if (is_paused) {            // se è in pausa
        time_t pause_end = time(nullptr);   //inizializza un time_t fine pausa
        paused_seconds = paused_seconds + static_cast<int>(pause_end - pause);      // aggiunge ai secondi di pausa un cast ad intero della differenza di tempo tra il time_t fine pause e il time_t pausa
        is_paused = false;              // booleano a false
    }
}

int timer::time_left()  {
    if (is_paused) {            // se è in pausa
        return seconds - static_cast<int>(pause - start - paused_seconds);          // ritorna i secondi meno un cast ad intero di pausa meno un cast ad intero di start meno i secondi totali di pausa
    } else {
        time_t current = time(nullptr);         // inizializza il time_t current
        int elapsed = static_cast<int>(current - start - paused_seconds);       // inizializza un intero secondi passati come cast ad intero di current meno start meno i secondi di pausa totali
        int remaining = seconds - elapsed;                                      // i secondi rimanenti sono uguali ai secondi meno i secondi passati
        if (remaining > 0) {                    // se rimangono più di zero secondi ritorna i secondi rimanenti
            return remaining;
        } else {
            return 0;                           // altrimenti ritorna 0
        }
    }
}

bool timer::time_out() {
    if (is_paused) {
        return false;               // se è in pausa ritorna false, non può essere time out mentre metti il gioco in pausa lol
    }
    time_t current = time(nullptr); // inizializza un time_t current
    int elapsed = static_cast<int>(current - start - paused_seconds);       // inizializza un intero secondi passati come cast ad intero di current meno start meno i secondi di pausa totali
    if (elapsed >= seconds) {                   // se i secondi passati sono più dei secondi di gioco
        return true;                            // ritorno true: è timeout
    } else {
        return false;                           // altrimenti rimangono altri secondi: non è timeout
    }
}

void timer::display(WINDOW *window, int y, int x) {         // funzione che stampa date due coordinate ed una finestra il tempo rimanente di una partita
    if (is_paused) {
        return;
    }
    int remaining = time_left();
    int minutes = remaining / 60;
    int seconds = remaining % 60;
    mvwprintw(window, y, x, "%d:%02d", minutes, seconds);
    wrefresh(window);
}

bool timer::ispaused() {
    return is_paused;
}