#include "livelli.hpp"
#include <pdcurses.h>

plistb livelli::crea_livelli() {
    const plistb head = new listab;
    head->val = 1;
    head->prev = nullptr;
    head->next = nullptr;
    plistb temp = head;
    for (int i = 0; i < 10; i++) {
        if (i == 9) {
            temp->next = nullptr;
        } else {
            temp->next = new listab;
            temp->next->val = i + 2;
            temp->next->next = nullptr;
            temp->next->prev = temp->next;
        }
        temp = temp->next;
    }
    return head;
}

int livelli::get_val(plistb current) {
    return current->val;
}

