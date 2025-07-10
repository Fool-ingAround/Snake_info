#include "timer.hpp"
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
    int points;
    bool type;
    timer itemtimer;
    itemlist *next; //lista bidirezionale
    itemlist *prev;
};
typedef itemlist *pitemlist;

class Items {
protected:
    static pitemlist head;
    static int difficulty;
public:
    Items(int difficulty = 1);
    static void newitem(int difficulty=1, int id=0, bool type = false); //creo un nuovo item
    static void removeitem(int id); //rimuovo un item
    static bool checkid(int id); //contorllo se un item è ancora nella lista (cioè non è mangiato o andato a male)
    static pos changepos(int id); //cambio la posizione di un item (nel caso abbia cercato di spawnarlo in un punto già occupato)
    static void pausealltimers(); //se il gioco viene messo in pausa anche tutti i timer vengono messi in pausa
    static void resumealltimers(); //quando il gico riparte ripartono tutti i timer
    static char getitem(int id); //queste e quelle dopo servono per accedere ai campi degli item
    static int getpoints(int id);
    static pos getposition(int id);
    static bool gettimer(int id);
    static bool returntype(int id);
};
#endif //ITEMS_HPP
