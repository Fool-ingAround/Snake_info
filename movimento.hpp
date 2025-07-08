#ifndef MOVIMENTO_H
#define MOVIMENTO_H
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
const double tempo=200;
const int alt=25;
const int larg=100;
int start_y=0;
int start_x=0;
int lserp=10;
class snake {
protected:
    plista head;
    bool matrix[alt][larg];
    WINDOW * win;
    int difficulty;
public:
    plista crea_corpo(plista head, int ax, int  ord, char c);
    snake(int diff=1);
    void stampa_snake();
    void dequeue();
    void schermata_iniziale();
    void UP();
    void DOWN();
    void LEFT();
    void RIGHT();
    double speed(int difficulty);
    bool inversione(int a, int b);
    void movements();
};
#endif

