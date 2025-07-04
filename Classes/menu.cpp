#include "menu.hpp"
#include <pdcurses.h>
#include "livelli.hpp"
#include <ctime>
#include "timer.hpp"

void menu::new_game(int difficulty) {
    int matrix_h = 20;
    int matrix_w = 60;
    int info_h = 5;
    int info_w = 60;
    int start_x = (matrix_w / 2)-1;
    int start_y = (matrix_h / 2)-1;
    WINDOW * game_win = newwin(matrix_h, matrix_w, 0, 0);
    WINDOW * info_win = newwin(info_h, info_w, matrix_h, 0);
    box(game_win, 0, 0);
    box(info_win, 0, 0);
    mvwprintw(game_win, start_y, start_x, "new game stuff...");
    wrefresh(game_win);
    wrefresh(info_win);
    keypad(game_win, true);
    nodelay(game_win, true);
    
    bool game = true;
    timer t = timer();

    while (!(t.time_out()) && (game)) {
        box(game_win, 0, 0);
        box(info_win, 0, 0);
        t.display(3, 29, info_win);
        int a = wgetch(game_win);
        switch (a) {
            case KEY_UP:
                start_y--;
                break;
            case KEY_DOWN:
                start_y++;
                break;
            case KEY_LEFT:
                start_x--;
                break;
            case KEY_RIGHT:
                start_x++;
                break;
            case 27:
                wclear(game_win);
                wclear(info_win);
                wrefresh(game_win);
                wrefresh(info_win);
                game = false;
                break;
            default:
                noecho();
                break;
        }
        wmove(game_win, start_y, start_x);
        curs_set(1);
        wrefresh(game_win);
        wrefresh(info_win);
        refresh();
    }
    if (game) {
        wclear(game_win);
        wrefresh(game_win);
        curs_set(0);
    }
}

void menu::level_select() {
    int height = 20;
    int width = 40;
    int start_x = width / 2;
    int start_y = height / 2;
    WINDOW * menu1 = newwin(height, width, 0, 0);
    plistb level = livelli::crea_livelli();
    plistb current_level = level;
    mvwprintw(menu1, start_y-1, start_x-6, "Select level :P");
    mvwprintw(menu1, start_y+1, start_x, "1");
    wmove(menu1, start_y+1, start_x);
    wrefresh(menu1);
    refresh();
    keypad(menu1, true);
    bool playing = true;

    while (playing) {
        box(menu1, 0, 0);
        wrefresh(menu1);
        int ch =wgetch(menu1);
        switch (ch) {
            case KEY_LEFT:
                if (current_level->prev != nullptr) {
                    plistb tmp = current_level;
                    current_level = current_level->prev;
                    current_level->next = tmp;
                    mvwprintw(menu1, start_y+1, start_x+1, " ");
                    wmove(menu1, start_y+1, start_x);
                } break;
            case KEY_RIGHT:
                if (current_level->next != nullptr) {
                    plistb tmp = current_level;
                    current_level = current_level->next;
                    current_level->prev = tmp;
                } break;
            case 10:
                wclear(menu1);
                wrefresh(menu1);
                refresh();
                new_game(livelli::get_val(current_level));
            case 27:
                wclear(menu1);
                wrefresh(menu1);
                playing = false;
                break;
            default:
                noecho();
                break;
        };
        mvwprintw(menu1, start_y+1, start_x, "%d", livelli::get_val(current_level));
        wmove(menu1, start_y+1, start_x);
        wrefresh(menu1);
        refresh();
    }
    wrefresh(menu1);
}

void menu::leaderboard() {
    WINDOW * leaderboard_win = newwin(20, 20, 0, 0);
    box(leaderboard_win, 0, 0);
    mvwprintw(leaderboard_win, 5, 5, "Leaderboard");
    bool lb = true;
    while (lb) {
        wrefresh(leaderboard_win);
        refresh();
        int ch = wgetch(leaderboard_win);
        switch (ch) {
            case 10:
                wclear(leaderboard_win);
                wrefresh(leaderboard_win);
                refresh();
                lb = false;
                break;
            default:
                noecho();
                break;
        }
    }
}

void menu::main_menu() {
    bool playing = true;
    int height = 20;
    int width = 40;
    int start_y = 0;
    int start_x = 0;

    WINDOW * win = newwin(height, width, start_y, start_x);
    keypad(win, true);
    curs_set(0);

    mvwprintw(win, 7, 15, "New game");
    mvwprintw(win, 11, 15, "Leaderboard");
    mvwprintw(win, 18, 18, "Press ESC to quit! :)");

    wmove(win, 7, 13);
    wprintw(win, ">");
    refresh();
    wrefresh(win);
    bool otherwin = false;
    bool ng = true;
    while (playing) {
        if (otherwin) {
            mvwprintw(win, 11, 15, "Leaderboard");
            mvwprintw(win, 7, 15, "New game");
            mvwprintw(win, 18, 18, "Press ESC to quit! :)");
            wmove(win, 7, 13);
            wprintw(win, ">");
            refresh();
            wrefresh(win);
            otherwin = false;
        }
        box(win, 0, 0);
        wrefresh(win);
        noecho();
        int ch = wgetch(win);
        if (ch == KEY_UP) {
            mvwprintw(win, 11, 13, " ");
            wmove(win, 7, 13);
            wprintw(win, ">");
            wrefresh(win);
            ng = true;
        }
        else if (ch == KEY_DOWN) {
            mvwprintw(win, 7, 13, " ");
            wmove(win, 11, 13);
            wprintw(win, ">");
            wrefresh(win);
            ng = false;
        }
        else if (ch == 10 && ng == true) {
            otherwin = true;
            wclear(win);
            wrefresh(win);
            level_select();
        }
        else if (ch == 10 && ng == false) {
            otherwin = true;
            wclear(win);
            wrefresh(win);
            leaderboard();
            ng = true;
        }
        else if (ch == 27) {
            playing = false;
            break;
        }
        else {
            noecho();
        }
    }

}