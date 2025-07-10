#include "Grid.hpp"
#include "Items.hpp"
#include "movimento.hpp"
#include <iostream>
#include <ncurses.h>
using namespace std;

Grid::Grid(int rows, int columns) { //Inizializzo la matrice a inizio partita
    endgame = false; //questo campo diventa true solo se il serpente si mangia o finisce il tempo, ci si accede con endgame(bool flag)
    score = 0; //inizializzo anche lo score
    counter = 1; //counter che aumenta ogni volta che creo un nuovo oggetto (in pratica è l'id di ogni oggetto)
    //   allid = nullptr;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j].occupied = false;
            matrix[i][j].item = 'e';
            matrix[i][j].id = 0;
        }
    } //qui inizializzo tutti i colori e le coppie belle
    init_color(COLOR_SNAKE,   0, 1000, 0);
    init_color(COLOR_APPLE,     1000, 200, 200);
    init_color(COLOR_BANANA,  1000, 1000, 0);
    init_color(COLOR_CHERRY, 1000, 0, 0);
    init_pair(1, COLOR_SNAKE, COLOR_BLACK);   // Colore dello snake
    init_pair(2, COLOR_APPLE, COLOR_BLACK);     // Colore Mela
    init_pair(3, COLOR_BANANA, COLOR_BLACK);  // Colore Banana
    init_pair(4, COLOR_CHERRY, COLOR_BLACK); // Colore Pesca
}

void Grid::Updatemtx(snake snake) { //a ogni ciclo aggiorno la mia matrice
    pos head = snake.get_head();
    for (int i = 0; i < rows; i++) { //per ogni riga
        for (int j = 0; j < cols; j++) { //per ogni colonna
            if (snake.snake::isoccupied(i, j) == true) { //Se è un pezzo del serpente
                if (head.i == i &&  head.j == j && matrix[i][j].occupied == false) //Case: Testa va in posto vuoto
                {
                    matrix[i][j].occupied = true;
                    matrix[i][j].item = 'h';
                }
                else if (head.i != i &&  head.j != j) //Case: non sto prendendo in considerazione la testa
                {
                    matrix[i][j].occupied = true;
                    matrix[i][j].item = 's';
                }
                else if (head.i != i &&  head.j != j && matrix[i][j].occupied == true) //Case: Collisione
                {
                    if (matrix[i][j].item == 's')
                    { //Case;: serpente si è mangiato da solo, setto l'endgame a true
                        setendgame(true);
                        matrix[i][j].item = 'h';
                    }
                    else if (matrix[i][j].item == 'a' || matrix[i][j].item == 'b' || matrix[i][j].item == 'c') //Case: serpente ha mangiato qualcosa nel punto (y,x)
                    {
                    Collision(i, j);
                    }
                }
            }
            else if (snake.isoccupied(i, j) == false && matrix[i][j].occupied == true && matrix[i][j].item == 's') { //Rimuovo l'ultimo pezzo del serpente precedente
                matrix[i][j].occupied = false;
                matrix[i][j].item = 'e';
            }
        }
    }
}

void Grid::Collision(int i, int j) { //la implemento appena ho finito con le funzioni di Items
    pos cell;
    cell.i = i;
    cell.j = j;
    score = score + Items::getpoints(matrix[i][j].id);
    Grid::removeItem(matrix[i][j].id);
}

void Grid::addItem(int difficulty) {
    Items::newitem(difficulty, counter); //creo un nuovo item con id counter
    pos position = Items::getposition(counter); //salvo in pos le coordinate dell'item
    while (matrix[position.i][position.i].occupied == true) { //controllo che non sia un punto già occupato
        position = Items::changepos(counter); //creo due nuove coordinate
    }
    matrix[position.i][position.j].occupied = true; //il nuovo posto viene segnato come occupato
    matrix[position.i][position.j].item = Items::getitem(counter); //specifico che tipo di item è
    matrix[position.i][position.j].id = counter; //salvo nella cella l'id dell'item
    //          /Timer::starttimer(Items::gettime(counter))
    counter++;
}

void Grid::removeItem(int id) {
    if (Items::checkid(id)) { //se l'oggetto è ancora in campo e va rimosso
        pos coords = Items::getposition(id);
        if (Items::gettime(id) == 0) { //se l'oggetto va rimosso perchè il timer è expired
            matrix[coords.i][coords.j].occupied = false;
            matrix[coords.i][coords.j].item = 'e';
            matrix[coords.i][coords.j].id = 0;
            Items::removeitem(id);
        }
        else if (Items::gettime(id) != 0){
            matrix[coords.i][coords.j].occupied = true;
            matrix[coords.i][coords.j].item = 'h';
            matrix[coords.i][coords.j].id = 0;
            Items::removeitem(id);
        }
    }
 //se il timer ha chiamato la funzione per eliminare un oggetto già mangiato non bisogna fare nulla
}

bool Grid::isendgame() { //funzione che tommaso deve chiamare a ogni ciclo
    return endgame;
}
void Grid::setendgame(bool flag) { //funzione che segnala la fine del gioco
    endgame = flag;
}

void Grid::UpdateGrid(WINDOW *game_win) {
    wclear(game_win);
    wrefresh(game_win);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            switch (matrix[i][j].item) {
                case 'h':
                    mvwaddch(game_win, i, j, '@' | COLOR_PAIR(1));
                    break;

                case 's':
                    mvwaddch(game_win, i, j, 'o' | COLOR_PAIR(1));
                    break;

                case 'A':
                    mvwaddch(game_win, i, j, 'A' | COLOR_PAIR(2));
                    break;

                case 'B':
                    mvwaddch(game_win, i, j, 'B' | COLOR_PAIR(3));
                    break;

                case 'C':
                    mvwaddch(game_win, i, j, 'C' | COLOR_PAIR(4));
                    break;
                default:
                    //mvwaddch(game_win, i, j, ' ');
                    break;
            }
        }
    }
    wrefresh(game_win);
}

void Grid::UpdateScore(WINDOW * info_win) {
mvwprintw(info_win, 0, 0, "%i", score);
    wrefresh(info_win);
}

/*
void Grid::UpdateGrid(WINDOW *game_win) {
    char stampa;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            switch (matrix[i][j].item) {
                case 'h':
                    stampa = '@';
                    mvprintw(i, j, "%c", stampa);
                    break;

                case 's':
                    stampa = 'o';
                    mvprintw(i, j, "%c", stampa);
                    break;

                case 'A':
                    stampa = 'A';
                    mvprintw(i, j, "%c", stampa);
                    break;

                case 'B':
                    stampa = 'B';
                    mvprintw(i, j, "%c", stampa);
                    break;

                case 'C':
                    stampa = 'C';
                    mvprintw(i, j, "%c", stampa);
                    break;
                default:
            }
        }
    }
}
*/
//mvaddch(y, x, matrix[y][x]);
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
void Grid::provagrid(int a, int b, int c, int d) {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 100; j++) {
            if (i == a && j >= b && j < c) {
                matrix[i][j].occupied = true;
                matrix[i][j].item = 's';
            }
            if (i == a && j == c) {
                matrix[i][j].occupied = true;
                matrix[i][j].item = 'h';
            }
            else if (i == 3 && j == d){
            matrix[i][j].occupied = true;
                matrix[i][j].item = 'A';
                matrix[i][j].id = 1;
            }
        }
    }
}