#include "movimento.hpp"
plista snake::crea_corpo(plista head, int ax, int ord, char c) {
    plista tmp= new lista;
    tmp->x=ax;
    tmp->y=ord;
    tmp->corpo=c;
    tmp->next=head;
    return tmp;
}
snake::snake(int diff) {
    win=newwin(alt, larg, start_y, start_x);
    difficulty=diff;
    int i=0;
    int j=0;
    for (i=0; i<alt; i++) {
        for (j=0; j<larg; j++) {
            matrix[i][j]=false;
        }
    }
    int n;
    head=nullptr;
    for (n=lserp-1; n>0; n--) {
        head=crea_corpo(head, larg/2-n, alt/2, 'o');
        matrix[alt/2][larg/2-n]=true;
    }
    head=crea_corpo(head, larg/2-n, alt/2, '@');
    matrix[alt/2][larg/2-n]=true;
}
void snake::stampa_snake() {
    box(win, 0, 0);
    wrefresh(win);
    start_color();
    init_color(COLOR_GREEN, 0, 1000, 0);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    wattron(win, COLOR_PAIR(1));
    plista p=head;
    while (p!=nullptr) {
        mvwprintw(win, p->y, p->x,"%c", p->corpo);
        wrefresh(win);
        p=p->next;
    }
    wattroff(win, COLOR_PAIR(1));
}
void snake::dequeue() {
    if (head==nullptr) {
        return;
    }
    if (head->next==nullptr) {
        matrix[head->y][head->x]=false;
        delete head;
        head=nullptr;
        return;
    }
    plista p=head;
    while (p->next->next!=nullptr) {
        p=p->next;
    }
    plista coda=p->next;
    matrix[coda->y][coda->x]=false;
    delete coda;
    p->next=nullptr;
}
void snake::schermata_iniziale() {
    refresh();
    box(win, 0, 0);
    wrefresh(win);
    keypad(win, true);
    noecho();
    cbreak();
    start_color();
    init_color(COLOR_GREEN, 0, 1000, 0);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win,alt/2, larg/2-15, "press ENTER to start the game");
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
    int ch=wgetch(win);
    while (ch!=KEY_ENTER && ch!='\n') {
        ch=wgetch(win);
    }
    wclear(win);
    wrefresh(win);
    stampa_snake();
    wrefresh(win);
}
void snake::UP() {
    dequeue();
    head->corpo='o';
    if ((head->y-1)==start_y) {
        head->y=alt-2;
    }
    head=crea_corpo(head, head->x, head->y-1, '@');
    matrix[head->y][head->x]=true;
    wclear(win);
    wrefresh(win);
    stampa_snake();
    wrefresh(win);
}
void snake::DOWN() {
    dequeue();
    head->corpo='o';
    if ((head->y+1)==alt-1) {
        head->y=start_y+1;
    }
    head=crea_corpo(head, head->x, head->y+1, '@');
    matrix[head->y][head->x]=true;
    wclear(win);
    wrefresh(win);
    stampa_snake();
    wrefresh(win);
}
void snake::LEFT() {
    dequeue();
    head->corpo='o';
    if ((head->x-1)==start_x) {
        head->x=larg-2;
    }
    head=crea_corpo(head, head->x-1, head->y, '@');
    matrix[head->y][head->x]=true;
    wclear(win);
    wrefresh(win);
    stampa_snake();
    wrefresh(win);
}
void snake::RIGHT() {
    dequeue();
    head->corpo='o';
    if ((head->x+1)==larg-1) {
        head->x=start_x+1;
    }
    head=crea_corpo(head, head->x+1, head->y, '@');
    matrix[head->y][head->x]=true;
    wrefresh(win);
    wclear(win);
    stampa_snake();
    wrefresh(win);
}
double snake::speed(int difficulty) {
    return tempo/(double(difficulty));
}
bool snake::inversione(int a, int b) {
    return (a==KEY_UP && b==KEY_DOWN)|| (a==KEY_LEFT && b==KEY_RIGHT) || (a==KEY_DOWN && b==KEY_UP) || (a==KEY_RIGHT && b==KEY_LEFT);
}
void snake::movements() {
    keypad(win, true);
    wtimeout(win, 0);
    noecho();
    int verso= KEY_RIGHT;
    int prossimo_verso=verso;
    while (true) {
        int azione;
        int ultima_azione=ERR;
        while ((azione=wgetch(win))!=ERR){
            ultima_azione=azione;
            if (ultima_azione!=ERR && (ultima_azione==KEY_UP || ultima_azione==KEY_DOWN || ultima_azione==KEY_LEFT || ultima_azione==KEY_RIGHT) && !inversione(ultima_azione, verso) && ultima_azione != verso) {
                prossimo_verso=ultima_azione;
            }
        }
        verso=prossimo_verso;
        switch (verso) {
            case KEY_UP:
                UP();
                break;
            case KEY_DOWN:
                DOWN();
                break;
            case KEY_LEFT:
                LEFT();
                break;
            case KEY_RIGHT:
                RIGHT();
                break;
        }
        napms(speed(difficulty));
    }
}

