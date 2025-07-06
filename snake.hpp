#include <iostream>
#include <ncurses.h>
using namespace std;
struct lista {
    char corpo;
    int x;
    int y;
    lista *next;
};
typedef lista *plista;
const double tempo=15;
const int alt=25;
const int larg=100;
int start_y=0;
int start_x=0;
int lserp=10;
class snake {
protected:
    plista head;
    bool matrix[alt][larg];
    WINDOW * win= newwin(alt, larg, start_y, start_x);
    int difficulty;
public:
    plista crea_corpo(plista head, int ax, int  ord, char c) {
        plista tmp= new lista;
        tmp->x=ax;
        tmp->y=ord;
        tmp->corpo=c;
        tmp->next=head;
        return tmp;
    }
    snake(int diff=1) {
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
    void stampa_snake() {
        refresh();
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
    void dequeue(){
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
    void schermata_iniziale() {
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
    void UP() {
        dequeue();
        head->corpo='o';
        head=crea_corpo(head, head->x, head->y-1, '@');
        matrix[head->y][head->x]=true;
        wclear(win);
        wrefresh(win);
    }
    void DOWN() {
        dequeue();
        head->corpo='o';
        head=crea_corpo(head, head->x, head->y+1, '@');
        matrix[head->y][head->x]=true;
        wclear(win);
        wrefresh(win);
    }
    void LEFT() {
        dequeue();
        head->corpo='o';
        head=crea_corpo(head, head->x-1, head->y, '@');
        matrix[head->y][head->x]=true;
        wclear(win);
        wrefresh(win);
    }
    void RIGHT() {
        dequeue();
        head->corpo='o';
        head=crea_corpo(head, head->x+1, head->y, '@');
        matrix[head->y][head->x]=true;
        wclear(win);
        wrefresh(win);
    }
    double speed(double difficulty) {
        return tempo/(double(difficulty)/2);
    }
    void movements() {
        keypad(win, true);
        halfdelay(speed(difficulty));
        noecho();
        int azione;
        azione=KEY_RIGHT;
        while (azione==KEY_RIGHT) {
            RIGHT();
        }

    }

};

