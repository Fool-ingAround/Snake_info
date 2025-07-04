//
// Created by utente on 03/07/2025.
//

#ifndef MENU_H
#define MENU_H



class menu {
public:
    menu()= default;
    static void main_menu();
    static void level_select();
    static void new_game(int difficulty);
    static void leaderboard();
    static void optionsrefresh();

    void newgame();
};



#endif //MENU_H