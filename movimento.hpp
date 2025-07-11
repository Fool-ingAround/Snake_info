#ifndef MOVIMENTO_H
#define MOVIMENTO_H
#include <iostream>
#include <pdcurses.h>
#include "Items.hpp"
using namespace std;
struct lista {         // il corpo dello snake è realizzato tramite una lista di caratteri e due variabili che indicano le coordinate di ogni pezzo del suo corpo
    char corpo;
    int x;
    int y;
    lista *next;
};
typedef lista *plista;
constexpr double tempo=200;   //utile per la velocità dello snake
constexpr int altezza=25;    //altezza della matrice
constexpr int larghezza=100;   //larghezza della matrice
extern int lserp;

class snake {
protected:
    plista head;    //lista usata per il serpente
    bool matrix[altezza][larghezza];    //matrice booleana che indica il campo in cui si muove il serpente
    WINDOW * win;  //variabile finestra che servirà per creare l'area di gioco
    int difficulty;   //difficoltà del livello
public:
    plista crea_corpo(plista head, int ax, int  ord, char c);
    snake(int diff, WINDOW* win);
    pos get_head();
    bool isoccupied(int y, int x);
    void stampa_snake();
    void dequeue();
    void schermata_iniziale();
    void UP();
    void DOWN();
    void LEFT();
    void RIGHT();
    double speed(int difficulty);
    bool inversione(int a, int b);
    int movements(int last, int input);
};
#endif
