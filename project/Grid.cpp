#include "Grid.hpp"
#include "Items.hpp"
#include "movimento.hpp"
#include <iostream>
#include <pdcurses.h>
#include "timer.hpp"

using namespace std;

Grid::Grid(int rows, int columns, int diff): Chain(4), Random(13) {
    //inizializzo la griglia di gioco e tutti i suoi valori quando inizia il livello, in input ho le dimensioni del campo e la difficoltà
    endgame = false; //a inizio partita la partita solitamente non è finita :)
    score = 0;
    counter = 1;
    //counter che aumenta ogni volta che creo un nuovo oggetto; inizia da 1 (l'id dei campi NON item è sempre 0)
    waspaused = false;
    //serve a non dover controllare a ogni aggiornamento della matrice se il gioco è stato messo in pausa
    difficulty = diff;
    chainitem = false; //variabile che mi dice se l'oggetto del timer chaintimer è già in campo
    switch (difficulty) {
        //questo switch serve a decidere, in base alla difficoltà, i tempi dei timer chainitem e randomitem
        case 1:
            chaintime = 3;
            randomtime1 = 15;
            randomtime2 = 20;
            break;
        case 2:
            chaintime = 3;
            randomtime1 = 15;
            randomtime2 = 19;
            break;
        case 3:
            chaintime = 2;
            randomtime1 = 14;
            randomtime2 = 19;
            break;
        case 4:
            chaintime = 2;
            randomtime1 = 14;
            randomtime2 = 18;
            break;
        case 5:
            chaintime = 2;
            randomtime1 = 13;
            randomtime2 = 17;
            break;
        case 6:
            chaintime = 1;
            randomtime1 = 13;
            randomtime2 = 16;
            break;
        case 7:
            chaintime = 1;
            randomtime1 = 12;
            randomtime2 = 14;
            break;
        case 8:
            chaintime = 1;
            randomtime1 = 12;
            randomtime2 = 15;
            break;
        case 9:
            chaintime = 0;
            randomtime1 = 11;
            randomtime2 = 14;
            break;
        default:
            chaintime = 0;
            randomtime1 = 10;
            randomtime2 = 13;
            break;
    }
    for (int i = 0; i < rows; i++) {
        //inizializzazione matrice, setta tutti i campi come vuoti
        for (int j = 0; j < columns; j++) {
            matrix[i][j].occupied = false;
            matrix[i][j].item = 'e';
            matrix[i][j].id = 0;
        }
    }
}

void Grid::Updatemtx(snake snake, timer gametimer) { //funzione più importante, aggiorna a ogni movimento del serpente la griglia di gioco
    if (gametimer.ispaused()) { //Se il gioco è stato messo in pausa fermo anche tutti i timer
        Chain.pause_timer();
        Random.pause_timer();
        Items::pausealltimers();
        waspaused = true; //ottimizzazione
    }
    else if (waspaused == true) { //invece di far sempre "ripartire" i timer lo faccio solo se il gioco era stato messo in pausa
        Chain.resume_timer();
        Random.resume_timer();
        Items::resumealltimers();
        waspaused = false;
    }
    else { //aggiornamento effettivo della matrice
        pos head = snake.get_head(); //salvo in una variabile di tipo pos (campi pos.i, pos.j) la posizione del serpente
        for (int i = 0; i < rows; i++) { //per ogni riga
            for (int j = 0; j < cols; j++) { //per ogni colonna
                if (snake.isoccupied(i, j) == true) { //Se è true allora in quel punto c'è un pezzo del serpente
                    if (head.i == i &&  head.j == j && matrix[i][j].occupied == false) //Case: Testa va in posto vuoto
                    {
                        matrix[i][j].occupied = true;
                        matrix[i][j].item = 'h';
                    }
                    else if (head.i != i ||  head.j != j) //Case: non sto prendendo in considerazione la testa
                    {
                        matrix[i][j].occupied = true;
                        matrix[i][j].item = 's';
                    }
                    else if (head.i == i &&  head.j == j && matrix[i][j].occupied == true) //Case: Collisione o con se stesso o con un item
                    {
                        if (matrix[i][j].item == 's')
                        { //Case: serpente si è mangiato da solo, setto l'endgame a true
                            setendgame(true);
                            matrix[i][j].item = 'h';
                        }
                        else if (matrix[i][j].item == 'A' || matrix[i][j].item == 'B' || matrix[i][j].item == 'C') //Case: serpente ha mangiato un frutto nel punto (y,x)
                        {
                            Collision(i, j); //viene richiamata la funziona collision, ossia vi è stata una collisione con un oggetto
                        }
                    }
                }
                else if (snake.isoccupied(i, j) == false && matrix[i][j].occupied == true && (matrix[i][j].item == 's'||matrix[i][j].item == 'h')) { //Rimuovo l'ultimo pezzo del serpente precedente
                    matrix[i][j].occupied = false;
                    matrix[i][j].item = 'e';
                }
            }// termina l'aggiornamento del serpente nella matrice
        } //inizio controllo timer per creare / rimuovere oggetti
        if (Chain.time_out() && chainitem); //se il timer di chainitem è a 0 ma l'item è già in campo allora non faccio nulla
        else if (Chain.time_out() && !chainitem) //se il timer di chainitem è a 0 ma l'item non c'è -> devo crearlo
        {
            Chain = timer(chaintime); //riavvio il timer per i chainitems
            addItem(true); //aggiungo un nuovo item di tipo chain
            chainitem = true; //il chainitem adesso è rpesente in campo
        }
    if (Random.time_out()) { //se è finito il timer degli item random
        srand(time( nullptr));
        Random = timer(rand() % (randomtime2 - randomtime1 +1) + randomtime1); //riavvio il timer con un valore casuale nell'intervallo compreso fra i due valori
        addItem(false); //aggiungo un item di tipo random
        }
    while (Items::expiredtimers() != -1) { //expiredtimers mi restituisce l'id del primo item presente che è expired, altrimenti mi restituisce -1. il ciclo serve per fare in modo che se 2 o più item sono expired li togla entrambi
        removeItem(Items::expiredtimers(), false); //rimuovo l'item con l'id restituito da expiredtimers perchè l'item è expired (e non è una collisione)
        }
    }
    if (endgame) Items::deleteallitems();
}

void Grid::Collision(int i, int j) { //funzione chiamata quando il serpente mangia un item
    score = score + Items::getpoints(matrix[i][j].id); //aumento lo score con il numero di punti in base all'oggetto
    removeItem(matrix[i][j].id, true); //rimuovo l'oggetto. specifico che è una collisione
}

void Grid::addItem(bool type) { //il bool mi dice se è un item di tipo chain o random
    Items::newitem(difficulty, counter, type); //creo un nuovo item con id counter e specifico se è chain o no
    pos position = Items::getposition(counter); //salvo in pos le coordinate dell'item
    while (matrix[position.i][position.i].occupied == true) { //controllo che non sia un punto già occupato
        position = Items::changepos(counter); // se il punto dove volevo aggiungere l'item è già occupato creo due nuove coordinate
    }
    matrix[position.i][position.j].occupied = true; //il nuovo posto viene segnato come occupato
    matrix[position.i][position.j].item = Items::getitem(counter); //specifico che tipo di item è
    matrix[position.i][position.j].id = counter; //salvo nella cella l'id dell'item
    counter++; //aumento il counter. il prossimo oggetto avrà id diverso
}

void Grid::removeItem(int id, bool collision) { //rimuovo un item. devo sapere se è una collisione o no
    pos coords = Items::getposition(id); //salvo la posizione dell'item con un certo id
    if (Items::gettype(id)) chainitem = false; //se l'item che rimuovo è quello chain allora chainitem non esiste più
    if (collision) { //se l'oggetto va rimosso perchè è stato mangiato
        matrix[coords.i][coords.j].occupied = true; //aggiorno la matrice in maniera adeguata
        matrix[coords.i][coords.j].item = 'h';
        matrix[coords.i][coords.j].id = 0;
        Items::removeitem(id); //una volta rimosso l'item dalla griglia devo anche eliminarlo dalla lista degli items
    }
    else { //se l'oggetto va rimosso perchè è finito il timer
        matrix[coords.i][coords.j].occupied = false;
        matrix[coords.i][coords.j].item = 'e';
        matrix[coords.i][coords.j].id = 0;
        Items::removeitem(id);
    }
}

bool Grid::isendgame() { //funzione che viene chiamata ad ogni ciclo del gioco. se è true il gioco è finito
    return endgame;
}
void Grid::setendgame(bool flag) { //funzione che setta il gameover a true
    endgame = flag;
}

void Grid::UpdateGrid(WINDOW *game_win) { //funzione che stampa nella finestra presa in input tutti i punti del campo
    wclear(game_win);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            switch (matrix[i][j].item) { //in base a cosa è presente in ogni cella stampo un certo carattere
                case 'h':
                    mvwaddch(game_win, i, j, '@' | COLOR_PAIR(12));
                    break;

                case 's':
                    mvwaddch(game_win, i, j, 'o' | COLOR_PAIR(12));
                    break;

                case 'A':
                    mvwaddch(game_win, i, j, 'A' | COLOR_PAIR(11));
                    break;

                case 'B':
                    mvwaddch(game_win, i, j, 'B' | COLOR_PAIR(21));
                    break;

                case 'C':
                    mvwaddch(game_win, i, j, 'C' | COLOR_PAIR(28));
                    break;
                default:

                    break;
            }
        }
    }
}

int Grid::UpdateScore() {
    return score;
}
