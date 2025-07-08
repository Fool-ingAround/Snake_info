#include <cstdlib>
#include <ctime>
#ifndef ITEMS_H
#define ITEMS_H

struct pos{
    int y;
    int x;
};
struct itemlist {
    pos position; //posizione riga-colonna (i = y, j = x)
    int id; //serve a differenziare gli item in modo che ognuno sia unico
    char item; //tipo di item (A, B, C)
    int points; //questa probabilmente è inutile
    int timeleft; //tempo rimasto, potrebbe essere inutile
    itemlist *next; //lista bidirezionale
    itemlist *prev;
};
typedef itemlist *pitemlist;

class Items {
protected:
    pitemlist head;
public:
    Items();
    void newitem(int difficulty=1, int id=0);
    void removeitem();
    char getitem(); //queste e quelle dopo servono per accedere ai valori
    int getpoints();
    int gettime();
    pos getposition();
};
#endif //ITEMS_H
//Inizio a implementare questa classe appena ho caricato tutto su GitHub