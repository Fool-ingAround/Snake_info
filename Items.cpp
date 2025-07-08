#include "Items.h"
#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <string>
#include <cstdlib>
#include "timer.h"

#include "Grid.h"
using namespace std;

Items::Items() { //Inizializzo il puntatore a vuoto
    head = nullptr;
}

void Items::newitem(int difficulty, int count) { //questa viene chiamata alla creazione di un item
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
                head->id = count;
                head->item = item;
                head->points = points;
                head->timeleft = timeleft;
                head->position.y = y;
                head->position.x = x;
            }
            else { //nel caso vi siano già oggetti in campo metto il nuovo ogetto nella head
                tmp = new itemlist;
                tmp->next = head;
                tmp->prev = nullptr;
                tmp->id = count;
                tmp->item = item;
                tmp->points = points;
                tmp->timeleft = timeleft;
                tmp->position.y = y;
                tmp->position.x = x;
                head->prev = tmp;
                head = tmp;
            }
        }

void Items::removeitem() {

}

void getitem(int id) {

}