#include "menu.hpp"
#include <pdcurses.h>
#include "livelli.hpp"
#include <ctime>
#include "timer.hpp"
#include <cstring>

void menu::colorsetup() {
    init_color(11, 1000, 329, 271);  // mela rossa
    init_pair(11, 11, -1);

    init_color(12, 220, 541, 294);   // snake verde
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


    int colors[10][3] = {
        {  98, 678, 682 },  // 1. Starting point — teal-cyan, soft and calm
        { 164, 574, 739 },  // 2. Shifts gently into cool sky-blue
        { 251, 458, 755 },  // 3. More violet-toned blue
        { 379, 348, 716 },  // 4. Subtle dusk-indigo
        { 511, 246, 642 },  // 5. Muted violet-magenta
        { 642, 155, 548 },  // 6. Soft magenta-rose
        { 759, 96, 426 },   // 7. Dusty raspberry
        { 858, 69, 295 },   // 8. Deep mauve-pink
        { 942, 52, 158 },   // 9. Warm plum-scarlet
        {1000,  0,   0 }    // 10. Bright red — endpoint
    };






    for (int i = 0; i < 10; i++) {
        int color_id = 30 + i; // Use custom color IDs starting from 20
        init_color(color_id, colors[i][0], colors[i][1], colors[i][2]);
        init_pair(30 + i, color_id, -1); // Foreground = custom color, background = black
    }

}

/*
int menu::victory() {

}

int menu::game_over() {

}
*/
int menu::player_select(char* name) {
    int win_h = 30;
    int win_w = 100;
    WINDOW* input_win = newwin(win_h, win_w, 0, 0);
    box(input_win, 0, 0);

    int y_entername = 5;
    int x_entername = 6;
    const char* entername[] = {
        "    ______      __                                                                     ",
        "   / ____/___  / /____  _____   __  ______  __  _______   ____  ____ _____ ___  ___ ",
        "  / __/ / __ \\/ __/ _ \\/ ___/  / / / / __ \\/ / / / ___/  / __ \\/ __ `/ __ `__ \\/ _ \\",
        " / /___/ / / / /_/  __/ /     / /_/ / /_/ / /_/ / /     / / / / /_/ / / / / / /  __/   ",
        "/_____/_/ /_/\\__/\\___/_/      \\__, /\\____/\\__,_/_/     /_/ /_/\\__,_/_/ /_/ /_/\\___/  ",
        "                             /____/                                                    "
    };

    for (int i = 0; i < 6; ++i) {
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
            case 5: pair_id = 26;
            break;
            default: pair_id = 0;
            break;
        }
        if (pair_id) {
            wattron(input_win, COLOR_PAIR(pair_id));
        }
        mvwprintw(input_win, y_entername + i, x_entername, "%s", entername[i]);
        if (pair_id) {
            wattroff(input_win, COLOR_PAIR(pair_id));
        }
    }

    wrefresh(input_win);

    nodelay(input_win, false);
    noecho();
    curs_set(1);


    int y_offset = 21;
    int x_offset = 38;
    const char* textbubble[] = {
        "  ,-----------------------------_",
        " /   Ssssurely you know what      >",
        "(  your name isss, at leassst... /",
        " \\       ... right?             |",
        "  `----------------------------`"
    };

    for (int i = 0; i < 5; i++) {
        mvwprintw(input_win, y_offset + i, x_offset, "%s", textbubble[i]);
    }

    const char* apple_art[] = {
        "@   ,--./,-.",
        "S  /,-._.--~\\",
        "SSSS S}  {S",
        "   SSS-._,S>-,",
        "    `._,.._,'"
    };

    for (int i = 0; i < 5; i++) {
        int len = strlen(apple_art[i]);
        for (int j = 0; j < len; ++j) {
            char ch = apple_art[i][j];
            if (ch == 'S' || ch == '@' || ch == '>') {
                wattron(input_win, COLOR_PAIR(12)); // Yellow for 'S' and '@'
            } else if (ch == '{' || ch == '}') {
                wattron(input_win, COLOR_PAIR(27));
            } else {
                wattron(input_win, COLOR_PAIR(11)); // Default red color
            }
            mvwaddch(input_win, y_offset+1 + i, x_offset+37 + j, ch);
            wattroff(input_win, COLOR_PAIR(11));
            wattroff(input_win, COLOR_PAIR(12));
            wattroff(input_win, COLOR_PAIR(27));
        }
    }


    int max_len = 20;
    int pos = 0;
    char buffer[21] = {};
    bool selecting_name = true;
    bool printable = true;
    while (selecting_name) {
        if (printable) {
            mvwprintw(input_win, win_h/2-1, win_w/2-1, "%-20s", buffer);
        }
        wmove(input_win, win_h/2-1, win_w/2-1 + pos);
        wrefresh(input_win);

        int ch = wgetch(input_win);
        switch (ch) {
            case 27:
                selecting_name = false;
            noecho();
            curs_set(0);
            wclear(input_win);
            wrefresh(input_win);
            delwin(input_win);
            return 0;

            case 10:
                selecting_name = false;
                break;

            case KEY_BACKSPACE:
                break;
            case 8:
                if (pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                }
            break;

            default:
                if (ch >= 32 && ch <= 126 && pos < max_len) {
                    buffer[pos++] = ch;
                    buffer[pos] = '\0';
                    printable = true;
                } else if ((ch < 32 || ch > 126)){
                    printable = false;
                    continue;
                }
            break;
        }
    }

    strncpy(name, buffer, max_len + 1); // copy result
    noecho();
    curs_set(0);
    wclear(input_win);
    wrefresh(input_win);
    delwin(input_win);
    return 2;
}


int menu::new_game(int difficulty) {
    int matrix_h = 26;
    int matrix_w = 100;
    int info_h = 5;
    int info_w = 100;
    int start_x = (matrix_w / 2)-1;
    int start_y = (matrix_h / 2)-1;
    WINDOW * info_win = newwin(info_h, info_w, 0, 0);
    WINDOW * game_win = newwin(matrix_h, matrix_w, info_h-1, 0);
    wborder(info_win,
    ACS_VLINE,  // Left
    ACS_VLINE,  // Right
    ACS_HLINE,  // Top
    ACS_HLINE,  // Bottom
    ACS_ULCORNER,  // Top-left
    ACS_URCORNER,  // Top-right
    ACS_LTEE,      // Bottom-left corner becomes sideways T (┠)
    ACS_RTEE       // Bottom-right corner becomes sideways T (┨)
    );

    wborder(game_win,
    ACS_VLINE,  // Left
    ACS_VLINE,  // Right
    ' ',        // Top (blank)
    ACS_HLINE,  // Bottom
    ' ',        // Top-left (blank)
    ' ',        // Top-right (blank)
    ACS_LLCORNER, // Bottom-left
    ACS_LRCORNER  // Bottom-right
    );

    /*
    snake::init(game_win, start_y, start_x) {
        mvwprintw(game_win, start_y, start_x, "O");
        mvwprintw(game_win, start_y, start_x-1, "o")
    }
    */
    wrefresh(game_win);
    wrefresh(info_win);
    keypad(game_win, true);
    noecho();

    bool game = true;
    mvwprintw(info_win, 2, 49, "3:00");
    wrefresh(info_win);
    int y = start_y;
    int x = start_x;
    int waitforinput = wgetch(game_win);
    timer t = timer();
    nodelay(game_win, true);
    while (!(t.time_out()) && (game)) {
        wborder(info_win,
        ACS_VLINE,  // Left
        ACS_VLINE,  // Right
        ACS_HLINE,  // Top
        ACS_HLINE,  // Bottom
        ACS_ULCORNER,  // Top-left
        ACS_URCORNER,  // Top-right
        ACS_LTEE,      // Bottom-left corner becomes sideways T (┠)
        ACS_RTEE       // Bottom-right corner becomes sideways T (┨)
        );
        wborder(game_win,
        ACS_VLINE,  // Left
        ACS_VLINE,  // Right
        ACS_HLINE,        // Top (blank)
        ACS_HLINE,  // Bottom
        ACS_LTEE,        // Top-left (blank)
        ACS_RTEE,        // Top-right (blank)
        ACS_LLCORNER, // Bottom-left
        ACS_LRCORNER  // Bottom-right
        );
        /*
        snake::movement(game_win, y, x, difficulty);
        */
        t.display(info_win, 2, 49);
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
            case 'p': {
                nodelay(game_win, false);
                int result = pause(game_win, info_win);
                nodelay(game_win, true);
                if (result == 404) {
                    break;
                } else {
                    return result;
                }
            }
            case 27:
                wclear(game_win);
                wclear(info_win);
                wrefresh(game_win);
                wrefresh(info_win);
                game = false;
                return 0;
            default:

                break;
        }
        wmove(game_win, start_y, start_x);
        wprintw(game_win, "%d", difficulty);
        curs_set(0);
        wrefresh(game_win);
        wrefresh(info_win);
    }
    if (game) {
        wclear(game_win);
        wclear(info_win);
        wrefresh(info_win);
        wrefresh(game_win);
        curs_set(0);
    }
    delwin(game_win);
    delwin(info_win);

    return 2;
}

void draw_in_level_select(WINDOW* win, int height, int width) {
    wborder(win,
        ACS_VLINE, ACS_VLINE,         // left, right
        ACS_HLINE, ACS_HLINE,   // top, bottom
        ACS_ULCORNER, ACS_TTEE,      // top-left, top-right
        ACS_LLCORNER, ACS_BTEE // bottom-left, bottom-right
        );
    int SL_y = height/7;
    int SL_x = width/10;
    const char* Selectlevel[] = {
        "   _____      __          __     __                   __",
        "  / ___/___  / /__  _____/ /_   / /   ___ _   _____  / /",
        "  \\__ \\/ _ \\/ / _ \\/ ___/ __/  / /   / _ \\ | / / _ \\/ / ",
        " ___/ /  __/ /  __/ /__/ /_   / /___/  __/ |/ /  __/ /  ",
        "/____/\\___/_/\\___/\\___/\\__/  /_____/\\___/|___/\\___/_/   "
    };

    for (int i = 0; i < 5; ++i) {
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
}

int menu::level_select(char* player_name, bool* levelarray) {
    curs_set(0);
    int height = 30;
    int width = 70;
    int start_x = width / 2;
    int start_y = height / 2;
    WINDOW * menu1 = newwin(height, width, 0, 0);
    WINDOW * info_win = newwin(height, 101-width, 0, width-1);
    plistb level = livelli::crea_livelli();
    plistb current_level = level;
    draw_in_level_select(menu1, height, width);
    wborder(info_win,
    ACS_VLINE, ACS_VLINE,   // left, right
    ACS_HLINE, ACS_HLINE,   // top, bottom
    ACS_TTEE, ACS_URCORNER, // top-left, top-right
    ACS_BTEE, ACS_LRCORNER  // bottom-left, bottom-right
    );
    mvwprintw(info_win, 2, 2, "Player: ");
    wattron(info_win, COLOR_PAIR(28));
    mvwprintw(info_win, 2, 10, "%s", player_name);
    wattroff(info_win, COLOR_PAIR(28));

    int info_y = 4;

    for (int i = 0; i < 20; i=i+2) {
        mvwprintw(info_win, info_y+i, 2, "Level %d", current_level->val+(i/2));
        if (levelarray[current_level->val-1]) {
            mvwprintw(info_win, info_y+i, 10, "no score");
        } else {
            mvwprintw(info_win, info_y+i, 10, "already played");
        }
    }

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

    int level_y = height/2-height/10;
    int level_x = width/2-width/12;

    /*
    mvwprintw(menu1, start_y-1, start_x-6, "Select level :P");
    mvwprintw(menu1, start_y+1, start_x, "1");
    */
    plistb temp_level = level;
    for (int i = 0; i < 10 && temp_level != nullptr; ++i) {
        int y = info_y + i * 2;
        mvwprintw(info_win, y, 2, "Level %d:  ", temp_level->val);
        if (levelarray[temp_level->val - 1]) {
            mvwprintw(info_win, y, 12, "no score");
        } else {
            mvwprintw(info_win, y, 12, "already played");
        }
        temp_level = temp_level->next;
    }
    wrefresh(menu1);
    wrefresh(info_win);
    noecho();
    nodelay(menu1, true);
    keypad(menu1, true);
    bool selecting = true;
    while (selecting) {
        int ch = wgetch(menu1);
        switch (ch) {
            case KEY_LEFT:
                if (current_level->prev != nullptr) {
                    plistb tmp = current_level;
                    current_level = current_level->prev;
                    current_level->next = tmp;
                } break;
            case KEY_RIGHT:
                if (current_level->next != nullptr) {
                    plistb tmp = current_level;
                    current_level = current_level->next;
                    current_level->prev = tmp;
                } break;
            case 10:
                if (!levelarray[current_level->val-1]) {
                    break;
                }
                else {
                    wclear(menu1);
                    wrefresh(menu1);
                    delwin(menu1);
                    levelarray[current_level->val-1] = false;
                    return current_level->val;
                }
            case 27:
                wclear(menu1);
                wrefresh(menu1);
                delwin(menu1);
                return 0;
            default:

                break;
        };

        switch (current_level->val) {
            case 1:
                werase(menu1);
                draw_in_level_select(menu1, height, width);
                if (levelarray[current_level->val-1]){
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(30));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level1[i]);
                        wattroff(menu1, COLOR_PAIR(30));
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        wattron(menu1, COLOR_PAIR(29));
                        mvwprintw(menu1, level_y + i, level_x, "%s", Level1[i]);
                        wattroff(menu1, COLOR_PAIR(29));
                    }
                }
                break;
            case 2:
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
        mvwprintw(menu1, start_y+1, start_x, "%d", livelli::get_val(current_level));*/
        wborder(menu1,
        ACS_VLINE, ACS_VLINE,         // left, right
        ACS_HLINE, ACS_HLINE,   // top, bottom
        ACS_ULCORNER, ACS_TTEE,      // top-left, top-right
        ACS_LLCORNER, ACS_BTEE // bottom-left, bottom-right
        );

        wborder(info_win,
        ACS_VLINE, ACS_VLINE,   // left, right
        ACS_HLINE, ACS_HLINE,   // top, bottom
        ACS_TTEE, ACS_URCORNER, // top-left, top-right
        ACS_BTEE, ACS_LRCORNER  // bottom-left, bottom-right
        );
        wrefresh(menu1);
        wrefresh(info_win);
    }
    delwin(menu1);
    return 0;
}

int menu::leaderboard() {
    int lb_h = 30;
    int lb_w = 100;
    WINDOW * leaderboard_win = newwin(lb_h, lb_w, 0, 0);

    noecho();
    bool lb = true;
    while (lb) {
        box(leaderboard_win, 0, 0);
        mvwprintw(leaderboard_win, 3, 45, "Leaderboard");
        int ch = wgetch(leaderboard_win);
        switch (ch) {
            case 27:
                wclear(leaderboard_win);
                wrefresh(leaderboard_win);
                refresh();
                lb = false;
                return 0;
            default:

                break;
        }
        wrefresh(leaderboard_win);
    }
    delwin(leaderboard_win);
    return 0;
}

int menu::main_menu() {
    bool playing = true;
    int height = 30;
    int width = 100;


    WINDOW * win = newwin(height, width, 0, 0);
    keypad(win, true);
    curs_set(0);


    mvwprintw(win, 18, 45, "New game");
    mvwprintw(win, 22, 45, "Leaderboard");
    wattron(win, COLOR_PAIR(13));
    mvwprintw(win, 27, 75, "Press ESC to quit! :)");
    wattroff(win, COLOR_PAIR(13));

    const char *ascii_art[] = {
        "        .d8888b.                                               888               ",
        "      d88P  Y88b                                              888               ",
        "     Y88b.                                                   888               ",
        "     \"Y888b.   .d8888b  .d8888b  .d8888b  88888b.   8888b.  888  888  .d88b.  ",
        "       \"Y88b. 88K      88K      88K      888 \"88b     \"88b 888 .88P d8P  Y8b ",
        "        \"888 \"Y8888b. \"Y8888b. \"Y8888b. 888  888 .d888888 888888K  88888888 ",
        " Y88b  d88P      X88      X88      X88 888  888 888  888 888 \"88b Y8b.     ",
        " \"Y8888P\"   88888P'  88888P'  88888P' 888  888 \"Y888888 888  888  \"Y8888  "
    };

    int num_lines = sizeof(ascii_art) / sizeof(ascii_art[0]);
    int start_y = 3;  // Center vertically
    int start_x = (width) / 12;          // Center horizontally assuming 75 chars wide

    // Print each line
    wattron(win, COLOR_PAIR(13)); // Green
    for (int i = 0; i < num_lines; ++i) {
        mvwprintw(win, start_y + i, start_x, "%s", ascii_art[i]);
    }
    wattroff(win, COLOR_PAIR(13));

    int y_offset = 12;
    int x_offset = 16;
    const char* apple_art[] = {
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
        mvwprintw(win, y_offset + i, x_txtbubble, "%s", text_bubble[i]);
    }


    for (int i = 0; i < 5; ++i) {
        int len = strlen(apple_art[i]);
        for (int j = 0; j < len; ++j) {
            char ch = apple_art[i][j];
            if (ch == 'S' || ch == '@') {
                wattron(win, COLOR_PAIR(12)); // Yellow for snake
            } else {
                wattron(win, COLOR_PAIR(11)); // Red for apple
            }
            mvwaddch(win, y_offset + i, x_offset + j, ch);
            wattroff(win, COLOR_PAIR(11));
            wattroff(win, COLOR_PAIR(12));
        }
    }
    wrefresh(win);


    wmove(win, 18, 43);
    wprintw(win, ">");
    noecho();
    wrefresh(win);
    bool ng = true;
    while (playing) {
        box(win, 0, 0);
        wrefresh(win);
        int ch = wgetch(win);
        if (ch == KEY_UP) {
            mvwprintw(win, 22, 43, " ");
            wmove(win, 18, 43);
            wprintw(win, ">");
            wrefresh(win);
            ng = true;
        }
        else if (ch == KEY_DOWN) {
            mvwprintw(win, 18, 43, " ");
            wmove(win, 22, 43);
            wprintw(win, ">");
            wrefresh(win);
            ng = false;
        }
        else if (ch == 10 && ng == true) {
            wclear(win);
            wrefresh(win);
            return 2;
        }
        else if (ch == 10 && ng == false) {
            wclear(win);
            wrefresh(win);
            return 1;
        }
        else if (ch == 27) {
            delwin(win);
            return 4;
        }
    }
    delwin(win);
    return 4;
}

int menu::pause(WINDOW * game_win, WINDOW * info_win) {
    int pmenu_h = 15;
    int pmenu_w = 50;
    WINDOW * pmenu = newwin(pmenu_h, pmenu_w, 7, 25);
    box(pmenu, 0, 0);
    mvwprintw(pmenu, 5, 22, "Resume :D");
    mvwprintw(pmenu, 7, 22, "Quit level ;(");
    mvwprintw(pmenu, 9, 22, "Main menu :'(");

    int cursor_x = 20;
    int cursor_y = 5;

    mvwprintw(pmenu, cursor_y, cursor_x, ">");
    int selection = 1;
    wrefresh(pmenu);
    keypad(pmenu, true);
    noecho();
    nodelay(pmenu, false);
    bool paused = true;
    while (paused) {
        int ch = wgetch(pmenu);
        switch (ch) {
            case KEY_UP:
                if (selection == 1) {
                    break;
                } else {
                    selection--;
                    cursor_y = cursor_y - 2;
                    break;
                }
            case KEY_DOWN:
                if (selection == 3) {
                    break;
                } else {
                    selection++;
                    cursor_y = cursor_y + 2;
                    break;
                }
            case 27:
                wclear(pmenu);
                wrefresh(pmenu);
                delwin(pmenu);
                paused = false;
                return 404;
            case 10:
                switch (selection) {
                    case 1:
                        paused = false;
                        return 404;
                        break;
                    case 2:
                        wclear(pmenu);
                        wclear(game_win);
                        wclear(info_win);
                        wrefresh(pmenu);
                        wrefresh(game_win);
                        wrefresh(info_win);
                        paused = false;
                        return 2;
                    case 3:
                        wclear(pmenu);
                        wclear(game_win);
                        wclear(info_win);
                        wrefresh(pmenu);
                        wrefresh(game_win);
                        wrefresh(info_win);
                        paused = false;
                        return 0;
                    default:

                        break;
                }
            break;
        }
        switch (selection) {
            case 1:
                mvwprintw(pmenu, cursor_y, cursor_x, ">");
                mvwprintw(pmenu, cursor_y+2, cursor_x, " ");
                mvwprintw(pmenu, cursor_y+4, cursor_x, " ");
                break;
            case 2:
                mvwprintw(pmenu, cursor_y-2, cursor_x, " ");
                mvwprintw(pmenu, cursor_y, cursor_x, ">");
                mvwprintw(pmenu, cursor_y+2, cursor_x, " ");
                break;
            case 3:
                mvwprintw(pmenu, cursor_y-4, cursor_x, " ");
                mvwprintw(pmenu, cursor_y-2, cursor_x, " ");
                mvwprintw(pmenu, cursor_y, cursor_x, ">");
                break;
        }
        wrefresh(pmenu);
    }
    delwin(pmenu);
    return 0;
}