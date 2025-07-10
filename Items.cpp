#include "Items.hpp"
#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <string>
#include <cstdlib>
//#include "timer.hpp"
#include "Grid.hpp"
using namespace std;

pitemlist Items::head = nullptr;

Items::Items(int difficulty) {

    this->difficulty = difficulty;
}

void Items::newitem(int difficulty, int id, bool type) { //questa viene chiamata alla creazione di un item
    srand(time(nullptr));
    pitemlist tmp;
    char item;
    int y, x, timeleft, points; //salvo in delle variabili temporanee i valori in base al tipo di oggetto
    int chance = rand() % 100;
        if (chance <= 59) { //Mela
            item = 'A';
            points = 100;
            timeleft = (difficulty); //funzione bella ancora da fare
            y = rand() % rows;
            x = rand() % cols;
            }
        else if (chance <= 84) { //Banana
            item = 'B';
            points = 150;
            timeleft = (difficulty);
            y = rand() % rows;
            x = rand() % cols;
            }
        else { //Ciliegia
            item = 'C';
            points = 200;
            timeleft = (difficulty);
            y = rand() % rows;
            x = rand() % cols;
            }
        if (head == nullptr) { //se è il primo oggetto / l'unico in campo
            head = new itemlist;
            head->next = nullptr;
            head->prev = nullptr;
            head->type = type;
            head->id = id;
            head->item = item;
            head->points = points;
            head->itemtimer = timer(timeleft);
            head->position.i = y;
            head->position.j = x;
            }
        else { //nel caso vi siano già oggetti in campo metto il nuovo ogetto nella head
            tmp = new itemlist;
            tmp->next = head;
            tmp->prev = nullptr;
            tmp->type = type;
            tmp->id = id;
            tmp->item = item;
            tmp->points = points;
            tmp->itemtimer = timer(timeleft);
            tmp->position.i = y;
            tmp->position.j = x;
            head->prev = tmp;
            head = tmp;
       }
}

void Items::removeitem(int id) {
    pitemlist tmp = head;
    while (tmp != nullptr && tmp->id != id) {
        tmp = tmp->next;
    }
    if (tmp == nullptr) ;
    else if (tmp->id == id)
    {
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        delete tmp;
    }
}

bool Items::checkid(int id) {
    pitemlist tmp = head;
    while (tmp != nullptr && tmp->id != id) {
        tmp = tmp->next;
    }
    if (tmp == nullptr) return false;
    else return true;
}

pos Items::changepos(int id) {
    srand(time(nullptr));
    pitemlist tmp = head;
    while (tmp != nullptr && tmp->id != id) {
        tmp = tmp->next;
    }
    if (tmp != nullptr) ;
    else if (tmp->id == id)
    {
        tmp->position.i = rand() % rows;
        tmp->position.j = rand() % cols;
    }
    return tmp->position;
}
/* //Questa funzione probabilmente non serve più
void Items::changetimeleft(int id) {
    pitemlist tmp = head;
    while (tmp != nullptr && tmp->id != id) {
        tmp = tmp->next;
    }
    if (tmp != nullptr) ;
    else if (tmp->id == id) {
        tmp->timeleft = 0;
    }
}
*/
char Items::getitem(int id) { //N sta per null
if (head == nullptr) {
    return 'N';
    }
    else
    {
        pitemlist tmp = head;
        while (tmp != nullptr && tmp->id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr)
        {
        return 'N';
        }
        else return tmp->item;
    }
}

int Items::getpoints(int id) { //N sta per null
    if (head == nullptr) {
        return 0;
    }
    else
    {
        pitemlist tmp = head;
        while (tmp != nullptr && tmp->id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr)
        {
            return 0;
        }
        else return tmp->points;
    }
}

pos Items::getposition(int id) { //N sta per null
    pos tmpv;
    tmpv.i =-1;
    tmpv.j =-1;
    if (head == nullptr) {
        return (tmpv);
    }
    else
    {
        pitemlist tmp = head;
        while (tmp != nullptr && tmp->id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr)
        {
            return tmpv;
        }
        else return tmp->position;
    }
}

void Items::pausealltimers() {
    if (head == nullptr);
    else {
        pitemlist tmp = head;
        while (tmp != nullptr) {
            tmp->itemtimer.pause_timer();
            tmp = tmp->next;
        }
    }
}

void Items::resumealltimers() {
    if (head == nullptr);
    else {
        pitemlist tmp = head;
        while (tmp != nullptr) {
            tmp->itemtimer.resume_timer();
            tmp = tmp->next;
        }
    }
}


bool Items::gettimer(int id) { //ritorna true se il timer dell'item è finito
    if (head == nullptr) { //se la lista è vuota
        return false;
    }
    else
    {
        pitemlist tmp = head;
        while (tmp != nullptr && tmp->id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr)
        {
            return false;
        }
        else return tmp->itemtimer.time_out(); //ritorna true se il timer è finito
    }
}

bool Items::returntype(int id) {
    if (head == nullptr) return true;
    else {
        pitemlist tmp = head;
        while (tmp != nullptr && tmp->id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr)
        {
            return true;
        }
        else return tmp->type;
    }
}