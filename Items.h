#ifndef ITEMS_H
#define ITEMS_H
#endif //ITEMS_H

struct pos{
    int x;
    int y;
};

class Items {
    protected:
    pos position;
    char item;
    int points;
    int time;
public:
   static void newitem(int difficulty);


};
//Inizio a implementare questa classe appena ho caricato tutto su GitHub