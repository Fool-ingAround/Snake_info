#include "Grid.h"
#include "Items.h"
#include <iostream>
#include <ctime>
#include <ncurses.h>
using namespace std;

Grid::Grid(int columns, int rows) { //Inizializzo la matrice ad inizio partita
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j].occupied = false;
            matrix[i][j].id = 0;
        }
    }
}

//PER TOMMASO MOLTO IMPORTANTE la funzione che devi controllare per sapere se il serpente si mangia da solo è questa
//NON ANCORA IMPLEMENTATA DEL TUTTO (manca effetto pacman)
bool Grid::addSnake(int y, int x) { //ora come ora uso i char w a s d per il movimento ANCORA DA IMPLEMENTARE PER EFFETTO PACMAN
    if (CheckGrid(y, x) && matrix[y][x].id == 1) return true; //se il serpente si muove in un punto dove era già finisce il gioco
    else if (CheckGrid(y, x)) { //se si muove in un punto già occupato ma NON dal serpente
        Collision(y, x);
        matrix[y][x].id = 1;
        return false;
    }
    else { // si muove in un punto vuoto
        matrix[y][x].occupied = true;
        matrix[y][x].id = 1;
        return false;
    }
}

void Grid::removeSnake(int y, int x) { //semplicemente rimuovo il pezzo dietro del serpente e lo setto vuoto
    matrix[y][x].occupied = false;
    matrix[y][x].id = 0;
}

void Grid::addItem(int y, int x, char item, int difficulty) { //aggiungo uno dei 3 items
    if (CheckGrid(y,x) && matrix[y][x].id >= 1 && matrix[y][x].id <= 4) { //se viene aggiunto in un punto già occupato ricreo un nuovo oggetto
        Items::newitem(difficulty); //funzione ancora da implementare
    }
    else if (!CheckGrid(y,x) && item == 'A') { //aggiungo oggetto mela (2)
        matrix[y][x].id = 2;
        matrix[y][x].occupied = true;
    }
    else if (!CheckGrid(y,x) && item == 'B') { //aggiungo oggetto Banana (3)
        matrix[y][x].id = 3;
        matrix[y][x].occupied = true;
    }
    else if (!CheckGrid(y,x) && item == 'C') { //aggiungo oggetto Ciliegia
        matrix[y][x].id = 4;
        matrix[y][x].occupied = true;
    }
}

void Grid::removeItem(int y, int x) { //rimuove semplicemente l'item (da usare SOLO quando sono expired)
    if (CheckGrid(y,x) && matrix[y][x].id != 1) { //controllo di non star provando a rimuovere un pezzo di serpente
        matrix[y][x].id = 0;
        matrix[y][x].occupied = false;
    }
}

bool Grid::CheckGrid(int y, int x) { //Controlla se un certo punto è già occupato o no
    if (matrix[y][x].occupied) {
        return true;
    }
    return false;
    }

void Grid::Collision(int y, int x) { //ancora da implementare

}

