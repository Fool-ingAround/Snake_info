#ifndef GRID_HPP
#define GRID_HPP
#include <ncurses.h>

struct sgrid {
    bool occupied; //come matrice booleana
    char item; //tipo di item (corpo, head, vuoto etc.)
    int id; //id (identifica il numero dell'oggetto)
};

const int rows = 25;
const int cols = 100;
const int COLOR_SNAKE = 8;
const int COLOR_APPLE = 9;
const int COLOR_BANANA = 10;
const int COLOR_CHERRY = 11;

class Grid {
protected:
    sgrid matrix[rows][cols]; //matrice in cui ogni punto ha le 3 informazioni
    bool endgame; //valore da controllare
    int score; // score totale del livello
    int counter; //lo uso per dare gli id agli oggetti
public:

    Grid(int columns, int rows);
    void Updatemtx(bool *mtx[][100], bool *mtH[][100]); //aggiorna la matrice
    void Collision(int i, int j); //collisione con oggetto
    void addItem(int difficulty); //aggiunge un item (devo ancora implementare la funzione che ne aggiunga uno periodicamente)
    void removeItem(int id); //ancora da implementare
    //void PrintGrid(bool *mtx); //quella che stamperà la finestra
    //bool addSnake(int y, int x); FUNZIONI VECCHIE DA IGNORARE PER ORA
    //void removeSnake(int y, int x);
    //bool CheckGrid(int y, int x);
    bool isendgame(); //ritorna true se il giocatore ha perso
    void setendgame(bool flag); //serve a cambiare il valore di endgame
    void UpdateScore(WINDOW * info_win);
    void UpdateGrid(WINDOW* game_win);
};

#endif //GRID_HPP
