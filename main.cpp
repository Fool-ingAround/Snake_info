#include <iostream>
#include <pdcurses.h>
#include "livelli.hpp"
#include "menu.hpp"
#include <ctime>
#include <cstring>

int main() {
    /*
    initscr();
    */
/*
    menu::main_menu();

    endwin();
    return 0;*/

    initscr();
    if (has_colors()) {
        start_color();
        use_default_colors();
    }

    menu::colorsetup();

    cbreak();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    char player_name[21] = "";
    int state = 0;
    int difficulty = 1;
    bool not_played[10];
    for (int i = 0; i < 10; ++i) {
        not_played[i] = true;
    }
    while (state != 4) {
        switch (state) {
                case 0:
                    player_name[0] = '\0';
                    for (int i = 0; i < 10; i++) {
                        not_played[i] = true;
                    }
                    state = menu::main_menu();
                    break;
                case 1:
                    state = menu::leaderboard();
                    break;
                case 2:
                    if (player_name[0] == '\0') {
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

    endwin();
    return 0;
}
