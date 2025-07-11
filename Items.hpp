#ifndef ITEMS_HPP
#define ITEMS_HPP
#include "timer.hpp"
struct pos{ //struttura che salva in una sola variabile due coordinate (y, x)
    int i;
    int j;
};
struct itemlist { //lista bidirezionale con diversi campi necessari agli oggetti
    pos position; //posizione riga-colonna (i = y, j = x)
    int id; //serve a differenziare gli item in modo che ognuno sia unico
    char item; //tipo di item (Apple, Banana, Cherry)
    int points; //ogni variabile vale un numero diverso di punti (100, 150 e 200)
    bool type; //identifica se è l'oggetto chain o no
    timer itemtimer; //timer personale di ogni item che segna quanto manca prima che esso "vada a male", ossia sia expired
    itemlist *next; //lista bidirezionale
    itemlist *prev;
};
typedef itemlist *pitemlist;

class Items {
protected:
    static pitemlist head; //utilizzo una lista per salvare gli items in modo che soia necessario un solo oggetto della classe item per salvarli tutti
     int difficulty;
public:
    Items(int difficulty = 1); //costruttore della classe
    static void newitem(int difficulty=1, int id=1, bool type = false); //creo un nuovo item; type indica se è chainitem o no
    static void removeitem(int id); //rimuovo l'item con un certo id
    static pos changepos(int id); //cambio la posizione di un item (nel caso abbia cercato di spawnarlo in un punto già occupato)
    static void pausealltimers(); //se il gioco viene messo in pausa anche tutti i timer vengono messi in pausa
    static void resumealltimers(); //quando il gioco riparte ripartono tutti i timer
    static int expiredtimers(); //funzione che controlla se vi è almeno un oggetto che va rimosso perchè andato a male. ritorna l'id del primo item expired che trova o -1 se nessun item è expired
    static void deleteallitems();
    static char getitem(int id); //queste e quelle dopo servono per accedere ai campi degli item
    static int getpoints(int id);
    static pos getposition(int id);
    static bool gettimer(int id);
    static bool gettype(int id);
};
#endif //ITEMS_HPP
