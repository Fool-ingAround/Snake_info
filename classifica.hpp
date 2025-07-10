#ifndef CLASSIFICA_HPP
#define CLASSIFICA_HPP
#include <iostream>
#include <cstring>
#include <fstream>
#include <ncurses.h>
using namespace std;
constexpr int length=23;   //lunghezza dell'array di nodi
constexpr int maxchar=100;  //lunghezza massima del nome che un giocatore può inserire
constexpr int start_x=0;  //coordinata x della finestra della classifica
constexpr int start_y=0;  //coordinata x della finestra della classifica
constexpr int alt=30;    //altezza della finestra
constexpr int larg=100;  //larghezza della finestra
struct nodo {          //struttura nodo che ha al suo intenro il campo nome, quello inserito dal giocatore, e score ovvero il suo punteggio
    char nome[100];
    int score;
};
class classifica {
protected:
    WINDOW *leader=newwin(alt, larg, start_x, start_y);     //viene creata la finestra
    nodo A[length];    //array di nodi
public:
    classifica();
    void scambianodo(nodo &x, nodo &y);
    bool isempty();
    void bubble_sort();
    void right_shift(int i);
    void inserimento(char player[], int punteggio);
    void scrivi_file();
    void leggi_file();
    int stampa_file();
};
#endif //CLASSIFICA_HPP
