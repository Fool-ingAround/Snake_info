//#include "timer.hpp"
#ifndef ITEMS_HPP
#define ITEMS_HPP

struct pos{
    int i;
    int j;
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
    static pitemlist head;
public:
    Items();
    static void newitem(int difficulty=1, int id=0);
    static void removeitem(int id);
    static bool checkid(int id);
    static pos changepos(int id);
    //static void changetimeleft(int id);
    static char getitem(int id); //queste e quelle dopo servono per accedere ai valori
    static int getpoints(int id);
    static pos getposition(int id);
    static int gettime(int id);

};
#endif //ITEMS_HPP
