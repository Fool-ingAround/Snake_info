#include "Items.hpp"
#include <iostream>
#include <ncurses.h>
#include "Grid.hpp"
using namespace std;

pitemlist Items::head = nullptr;

Items::Items(int diff) {
    difficulty = diff;
}

void Items::newitem(int difficulty, int id, bool type) { //questa viene chiamata alla creazione di un item
    srand(time(nullptr));
    char item;
    int y, x, timeleft, points; //salvo in delle variabili temporanee i valori in base al tipo di oggetto
    int chance = rand() % 100; //in base al valore di chance scelgo che tipo di item spawnare
        if (chance <= 59) { //Mela
            item = 'A';
            points = 100;
            timeleft = 40/difficulty+6; //tempo di permanenza della mela prima che sia expired
            y = rand() % rows; //posizione della mela
            x = rand() % cols;
            }
        else if (chance <= 84) { //Banana
            item = 'B';
            points = 150;
            timeleft = (70/difficulty+9)/2;
            y = rand() % rows;
            x = rand() % cols;
            }
        else { //Ciliegia
            item = 'C';
            points = 200;
            timeleft = 30/difficulty+3;
            y = rand() % rows;
            x = rand() % cols;
            }
        if (head == nullptr) { //se è il primo oggetto / l'unico in campo aggiorno direttamente la testa con un head insert
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
        else { //nel caso vi siano già oggetti in campo viene aggiunto un nuovo nodo alla lista con una headinsert
            pitemlist tmp = new itemlist;
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
            //non c'è bisogno di puntare tmp a nullptr perchè la funzione finisce e tmp viene eliminato automaticamente
       }
}

void Items::removeitem(int id) { //funzione che rimuove l'item con un certo id dalla lista degli item
    pitemlist tmp = head;
    while (tmp != nullptr && tmp->id != id) {
        tmp = tmp->next;
    }
    if (tmp == nullptr); //se sto cercando di eliminare un item che non esiste
    else if (tmp->id == id)
    {
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        delete tmp;
    }
}

pos Items::changepos(int id) { //funzione chiamata da Grid::newItem nel caso le coordinate generate la prima volta indicano un punto già occupato
    srand(time(nullptr));
    pitemlist tmp = head;
    while (tmp != nullptr && tmp->id != id) {
        tmp = tmp->next;
    }
    if (tmp == nullptr) return {0, 0}; //se per qualche motivo sto cercando di cambiare le variabili di un item che non esiste di default lo metto a 0 0
    else if (tmp->id == id)
    {
        tmp->position.i = rand() % rows; //genero due nuove coordinate per l'item
        tmp->position.j = rand() % cols;
    }
    return tmp->position; //ritorno la nuova posizione
}

void Items::pausealltimers() { //funzione che mette il timer di tutti gli oggetti in pausa
    if (head == nullptr);
    else {
        pitemlist tmp = head;
        while (tmp != nullptr) {
            tmp->itemtimer.pause_timer();
            tmp = tmp->next;
        }
    }
}

void Items::resumealltimers() { //funzione che fa ripartire i timer di tutti gli oggetti
    if (head == nullptr);
    else {
        pitemlist tmp = head;
        while (tmp != nullptr) {
            tmp->itemtimer.resume_timer();
            tmp = tmp->next;
        }
    }
}

int Items::expiredtimers() { //funzione che controlla se almeno un timer è expired. Se lo trova ritorna l'id dell'item, altrimenti ritorna-1
    if (head == nullptr) return -1;
    pitemlist tmp = head;
    while (tmp != nullptr) {
        if (tmp->itemtimer.time_out()) return tmp->id;
        tmp = tmp->next;
    }
    if (tmp == nullptr) return -1;
}

char Items::getitem(int id) {
if (head == nullptr) { //se cerco in qualche modo di ottenere il char di un item quando non ne esiste nessuno
    return 'e';
    }
    else
    {
        pitemlist tmp = head;
        while (tmp != nullptr && tmp->id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) //se cerco in qualche modo di ottenere il char di un item che non esiste
        {
        return 'e';
        }
        else return tmp->item; //ritorno il char dell'item con un certo id
    }
}

int Items::getpoints(int id) { //uguale a getitem ma per i punti
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

pos Items::getposition(int id) { //uguale a getitem ma per le coordinate
    if (head == nullptr) {
        return {0, 0};
    }
    else
    {
        pitemlist tmp = head;
        while (tmp != nullptr && tmp->id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr)
        {
            return {0, 0};
        }
        else return tmp->position;
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

bool Items::gettype(int id) { // come getitem ma per sapere se è un chainitem
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
