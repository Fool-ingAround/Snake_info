#ifndef LIVELLI_H
#define LIVELLI_H

struct listab {
    int val;
    listab *next;
    listab *prev;
};

typedef listab * plistb;

class livelli {
private:
    plistb head;
public:
    livelli();
    static plistb crea_livelli();
    static int get_val(plistb current);
};



#endif //LIVELLI_H
