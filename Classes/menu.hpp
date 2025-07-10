//
// Created by utente on 03/07/2025.
//
#include <pdcurses.h>

#ifndef MENU_H
#define MENU_H



class menu {
private:
    static char player_name[21];
    static int state;
    static int difficulty;
    static bool not_played[10];
public:
    menu();
    static void start_up();
    static void colorsetup();
    static int main_menu();
    static int player_select(char* name);
    static int level_select(char* name, bool * levelarray);
    static int new_game(int difficulty);
    static int leaderboard();
    static int pause(WINDOW * game_win, WINDOW * info_win);
    static int game_over();
    static int victory();
};



#endif //MENU_H