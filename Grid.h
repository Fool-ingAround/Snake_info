#ifndef GRID_H
#define GRID_H
struct sgrid {
    bool occupied; //come matrice booleana
    char item; //tipo di item (corpo, head, vuoto etc.)
    int id; //id (identifica il numero dell'oggetto)
};
const int rows = 25;
const int cols = 100;
class Grid {
protected:
    sgrid matrix[rows][cols]; //matrice in cui ogni punto ha le 3 informazioni
    bool endgame; //valore da controllare
    int score; // score totale del livello
public:

    Grid(int columns, int rows);
    void addItem(int y, int x, char item, int difficulty); //aggiunge un item (devo ancora implementare la funzjone che ne aggiunga uno periodicamente)
    void removeItem(int y, int x); //ancora da implementare
    void Updatemtx(bool *mtx[][100], bool *mtH[][100]); //aggiorna la patrice
    void PrintGrid(bool *mtx); //quella che stamperà la finestra
    //bool addSnake(int y, int x); FUNZIONI VECCHIE DA IGNORARE PER ORA
    //void removeSnake(int y, int x);
    //bool CheckGrid(int y, int x);
    void Collision(int i, int j); //collisione con oggetto
    void setendgame(bool flag); //serve a cambiare il valore di endgame
    bool isendgame(); //ritorna true se il giocatore ha perso
};
#endif //GRID_H