//
// Created by utente on 02/07/2025.
//

#ifndef LIVELLI_H
#define LIVELLI_H

struct listab {
    int val;
    listab *next;
    listab *prev;
};

typedef listab * plistb;

class livelli {
    protected:
    plistb head;
public:
    livelli () {
        head = nullptr;
    }

    static plistb crea_livelli();

    static int get_val(plistb current);
};



#endif //LIVELLI_H
