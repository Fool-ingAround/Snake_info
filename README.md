# Snake_info
Progetto di programmazione 2024-2025.

Partecipanti:
- Tommaso Lalicata: livelli, menu e timer
- Carlo Cataldo Panariello: snake, movimento e leaderboard
- Luca Pinardi: oggetti, collisioni e punteggio

(IL GIOCO È STATO TESTATO QUASI TOTALMENTE IN AMBIENTE WINDOWS)

Per far partire il gioco nel main basta includere "menu.hpp" e chiamare il metodo start_up(), segue un esempio:

#include "menu.hpp"

int main() {
    menu::start_up();
    return 0;
}

NOTE:
- Lo scopo del gioco è fare più punti possibili (e fin qui);
- Se il serpente si mangia la coda in un livello, lo score di quel livello è 0;
- Se in qualunque momento, nel level select screen si decida di premere esc, verrà salvato in leaderboard il punteggio
  totalizzato fino a quel punto;
- Stesso vale per il new game;
- Quando sono stati giocati tutti i livelli, si tornerà automaticamente al il main menu ed il punteggio sarà salvato in leaderboard;


Buon divertimento :)
