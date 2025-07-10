#ifndef GRID_HPP
#define GRID_HPP
#include <ncurses.h>
#include "movimento.hpp"
#include "timer.hpp"

struct sgrid {
    bool occupied; //come matrice booleana
    char item; //tipo di item (corpo, head, vuoto etc.)
    int id; //id (identifica il numero dell'oggetto)
};

constexpr int rows = 25;
constexpr int cols = 100;
constexpr int COLOR_SNAKE = 8;
constexpr int COLOR_APPLE = 9;
constexpr int COLOR_BANANA = 10;
constexpr int COLOR_CHERRY = 11;

class Grid {
protected:
    sgrid matrix[rows][cols]; //matrice in cui ogni punto ha le 3 informazioni
    bool endgame; //valore da controllare
    int score; // score totale del livello
    int counter; //lo uso per dare gli id agli oggetto
    timer Chain; //timer che crea un nuovo oggetto ogni volta
    timer Random; //timer che crea un oggetto a caso
    bool chainitem;
    int difficulty;
    bool waspaused;
public:

    Grid(int rows, int columns, int difficulty);
    void Updatemtx(snake a, timer pause); //aggiorna la matrice
    void Collision(int i, int j); //collisione con oggetto
    void addItem(bool type); //aggiunge un item (devo ancora implementare la funzione che ne aggiunga uno periodicamente)
    void removeItem(int id); //ancora da implementare
    //bool addSnake(int y, int x); FUNZIONI VECCHIE DA IGNORARE PER ORA
    //void removeSnake(int y, int x);
    //bool CheckGrid(int y, int x);
    bool isendgame(); //ritorna true se il giocatore ha perso
    void setendgame(bool flag); //serve a cambiare il valore di endgame
    void UpdateScore(WINDOW * info_win);
    void UpdateGrid(WINDOW* game_win);
    void newrandom(); //per fare ripartire il timer random?? boh
    void provagrid(int a, int b, int c, int d);

};

#endif //GRID_HPP
