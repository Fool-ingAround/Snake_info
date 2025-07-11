#ifndef CLASSIFICA_HPP
#define CLASSIFICA_HPP
#include <iostream>
#include <cstring>
#include <fstream>
#include <pdcurses.h>

using namespace std;
constexpr int length=24;   //lunghezza dell'array di nodi
constexpr int maxchar=100;  //lunghezza massima del nome che un giocatore può inserire
constexpr int start_x=0;  //coordinata x della finestra della classifica
constexpr int start_y=0;  //coordinata x della finestra della classifica
constexpr int alt=30;    //altezza della finestra
constexpr int larg=100;  //larghezza della finestra
struct nodo {          //struttura nodo che ha al suo intenro il campo nome, quello inserito dal giocatore, e score ovvero il suo punteggio
    char nome[21];
    int score;
};
class classifica {
protected:
    static nodo A[length];    //array di nodi
public:
    classifica();

    static void init();
    static void scambianodo(nodo &x, nodo &y);
    static bool isempty();
    static void bubble_sort();
    static void right_shift(int i);
    static void inserimento(const char player[], int punteggio);

    static void scrivi_file();
    static void leggi_file();
    static int stampa_file(WINDOW* leader);
};
#endif //CLASSIFICA_HPP
