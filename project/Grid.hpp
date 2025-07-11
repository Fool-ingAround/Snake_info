#ifndef GRID_HPP
#define GRID_HPP
#include <pdcurses.h>
#include "movimento.hpp"
#include "timer.hpp"

struct sgrid {
    bool occupied; //come matrice booleana
    char item; //tipo di item (corpo, head, vuoto etc.)
    int id; //id (identifica il numero identificativo dell'oggetto)
};

constexpr int rows = 25; //costanti utili
constexpr int cols = 100;

class Grid {
protected:
    sgrid matrix[rows][cols]; //matrice in cui ogni punto ha 3 informazioni: occupato/libero, che tipo di item e l'id del frutto (se c'è)
    bool endgame; //valore da controllare ogni volta, se diventa true la partita è finita
    int score; // score totale del livello
    int counter; //utilizzato per variare l'id ogni volta che viene creato un oggetto
    timer Chain; //timer che crea un nuovo oggetto ogni volta che un item del tipo chainitem viene mangiato
    timer Random; //timer che crea un oggetto ogni tanto a intervalli semiregolari
    int chaintime; //intervallo fra un chainitem e l'altro
    int randomtime1; //questo e quello dopo sono i due valori entro cui il timer random spawna l'oggetto
    int randomtime2; //^^^
    bool chainitem; //variabile che indica se è già presente l'item del chaintimer
    int difficulty;
    bool waspaused; //variabile che diventa true se il gioco è stato messo in pausa. è un ottimizzazzione

public:

    Grid(int rows, int columns, int difficulty); //costruttore
    void Updatemtx(snake snake); //aggiorna la matrice con il serpente e gli oggetti, ha in input il serpente e il timer per fermare il gioco
    void Collision(int i, int j); //viene chiamata quando vi è una collisione con un oggetto nel punto i, j
    void addItem(bool type); //aggiunge un nuovo item nel gioco (type indica se è un chainitem o no)
    void removeItem(int id, bool collision); //rimuove l'item con un certo id (collision serve a sapere se è una collisione o no)
    bool isendgame(); //ritorna true se il giocatore ha perso
    void setendgame(bool flag); //serve a cambiare il valore di endgame
    void pausetimers(timer pause); //Se il gioco viene messo in pausa viene chiamata e mette in pausa tutti i timer
    int UpdateScore(); //stampa la finestra di gioco
    void UpdateGrid(WINDOW* game_win); //stampa la finestra dello score

};

#endif //GRID_HPP