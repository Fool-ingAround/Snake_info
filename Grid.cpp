#include "Grid.h"
#include "Items.h"
#include <iostream>
#include <ctime>
#include <ncurses.h>
using namespace std;

Grid::Grid(int columns, int rows) { //Inizializzo la matrice a inizio partita
    endgame = false; //questo campo diventa true solo se il serpente si mangia o finisce il tempo, ci si accede con endgame(bool flag)
    score = 0; //inizializzo anche lo score
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j].occupied = false;
            matrix[i][j].item = 'e';
        }
    }
}

void Grid::Updatemtx(bool *mtx[][100], bool *mtH[][100]) { //ad ogni ciclo aggiorno la mia matrice
    for (int i = 0; i < rows; i++) { //per ogni riga
        for (int j = 0; j < cols; j++) { //per ogni colonna
            if (*mtx[i][j] == true) { //Se è un pezzo del serpente
                if (*mtH[i][j] == true && matrix[i][j].occupied == false) //Case: Testa va in posto vuoto
                {
                    matrix[i][j].occupied = true;
                    matrix[i][j].item = 'h';
                }
                else if (*mtH[i][j] == false) //Case: non sto prendendo in considerazione la testa
                {
                    matrix[i][j].occupied = true;
                    matrix[i][j].item = 's';
                }
                else if (*mtH[i][j] == true && matrix[i][j].occupied == true) //Case: Collisione
                {
                    if (matrix[i][j].item == 's')
                    { //Case;: serpente si è mangiato da solo, setto l'endgame a true
                        Grid::setendgame(true);
                        matrix[i][j].item = 'h';
                    }
                    else if (matrix[i][j].item == 'a' || matrix[i][j].item == 'b' || matrix[i][j].item == 'c') //Case: serpente ha mangiato qualcosa nel punto (y,x)
                    {
                    Grid::Collision(i, j);
                    }
                }
            }
            else if (*mtx[i][j] == false && matrix[i][j].occupied == true && matrix[i][j].item == 's') { //Rimuovo l'ultimo pezzo del serpente precedente
                matrix[i][j].occupied = false;
                matrix[i][j].item = 'e';
            }
        }
    }
}

bool Grid::isendgame() { //funzione che tommaso deve chiamare ad ogni ciclo
    return endgame;
}
void Grid::setendgame(bool flag) {
    endgame = flag;
}

void Grid::Collision(int i, int j) { //la implemento appena ho finito con le funzioni di Items
    if (matrix[i][j].occupied == true && matrix[i][j].item == 'a')
    {

    }
}

/*   TUTTE FUNZIONI VECCHIE CHE POI ELIMINO
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

*/
